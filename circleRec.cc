#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

struct coor {
	coor(double a, double b, double c, bool d) : x(a), y(b), r(c), t(d){}
	double x;
	double y;
	double r;
	bool t;
};

int main(int argc, char** argv) {
	string path(argv[1]);
	string name(argv[2]);
	string outPath(argv[3]);

	bool s = true;

	path = path + name + ".png";
	cout << path << endl;

	Mat src;
	src = imread(path.c_str());
	vector<Mat> rgb;
	split(src, rgb);
	src = rgb[0];

	if (src.empty()) {
		clog << "Can not load the image!" << endl;
		return 0;
	}

	Mat kernel;
	kernel = getStructuringElement(1, Size(3, 3));

	// cvtColor(src, src, CV_RGB2GRAY);
	threshold(src, src, 125, 255, THRESH_BINARY_INV);
	// erode(src, src, kernel);
	// medianBlur(src,src,3);

	vector<Vec3f> circles0;
	vector<Vec3f> circles00;
	vector<Vec3f> circles1;
	vector<Vec4i> lines;

	int h = src.rows;
	int w = src.cols;
	// cout << h << "\t"
	// 	 << w << endl;
	cv::Rect select0, select1;

	// select0 = Rect(0, 0, w - 1, h / 2);
	// select1 = Rect(0, h / 2, w - 1, h - 1);

	// select0 = Rect(0, 0, 1329, 150);
	// select1 = Rect(0, 0, 200, 200);

	Mat ROI0 = src.rowRange(0, 150).clone();
	Mat ROI1 = src.rowRange(151, 299).clone();

	// Mat ROI0 = src(select0);
	// Mat ROI1 = src(select1);

	// cout << "Processing..." << endl;

	HoughLinesP(src, lines, 1, CV_PI / 180, 80, 20, 10);
	HoughCircles(ROI0, circles0, CV_HOUGH_GRADIENT, 1, 60, 20, 15, 9, 14);
	HoughCircles(ROI1, circles00, CV_HOUGH_GRADIENT, 1, 60, 20, 15, 9, 14);
	HoughCircles(src, circles1, CV_HOUGH_GRADIENT, 1, 60, 40, 15, 30, 40);
	vector<coor> circs;
	// cout << "sizes: " << endl
	// 	 << circles0.size() << "\t"
	// 	 << circles00.size() << "\t"
	// 	 << circles1.size() << endl;

	cvtColor(src, src, CV_GRAY2RGB);
	for (int i = 0; i < circles0.size(); i ++) {
		Vec3f c = circles0[i];
		// cout << "(" << c[0] << ", " << c[1] << ")" << endl;
		// cout << c[2] << endl;
		circle(src, Point(c[0], c[1]), c[2], Scalar(0,255,255), 3, CV_AA);
		coor tmp(c[0], c[1], c[2], true);
		circs.push_back(tmp);
	}

	for (int i = 0; i < circles00.size(); i ++) {
		Vec3f c = circles00[i];
		circle(src, Point(c[0], c[1] + 150), c[2], Scalar(0,255,255), 3, CV_AA);
		coor tmp(c[0], c[1], c[2], true);
		circs.push_back(tmp);
	}

	for (int i = 0; i < circles1.size(); i ++) {
		Vec3f c = circles1[i];
		// cout << "(" << c[0] << ", " << c[1] << ")" << endl;
		// cout << c[2] << endl;
		circle(src, Point(c[0], c[1]), c[2], Scalar(0,255,255), 3, CV_AA);
		coor tmp(c[0], c[1], c[2], true);
		circs.push_back(tmp);
	}

	// for (int i = 0; i < lines.size(); i ++) {
	// 	Vec4i p = lines[i];
	// 	line(src, Point(p[0], p[1]), Point(p[2], p[3]), Scalar(0, 255, 255), 3, CV_AA);
	// }

	sort(circs.begin(), circs.end(), [](coor a, coor b){return a.x < b.x;});

	int cc = 0;
	int lc = 0;
	int rc = 0;
	int ccount = 0;
	for (int i = 0; i < circs.size(); i ++) {
		coor tmp = circs[i];
		if ((tmp.x > 370 && tmp.x < 500) || (tmp.x > 950 && tmp.x < 1100))
			cc ++;
		if (tmp.x < 380)
			lc ++;
		if (tmp.x > 1100)
			rc ++;
		if (tmp.x > 500 && tmp.x < 950)
			ccount ++;
	}

	// cout << cc << "\t"
	// 	 << lc << "\t"
	// 	 << rc << endl;

	if (!cc && (lc || rc)) {
		for (int i = 0; i < circs.size(); i ++) {
			coor tmp = circs[i];
			// cout << tmp.x << endl;
			if (tmp.x < 380 || tmp.x > 1100) {
				// cout << "Remove" << endl;
				circs[i].t = false;
			}
		}
	}

	if (!ccount && !(lc || rc) && !lines.size()) {
		s = false;
	}

	int sCount = 0;
	int lCount = 0;
	double lastX = 0;
	int code = 0;
	code ++;
	// code << 1;
	for (int i = 0; i < circs.size(); i ++) {
		coor tmp = circs[i];
		if (!tmp.t)
			continue;
		if (tmp.r > 20) {
			lCount ++;
		}
		else {
			sCount ++;
		}
		if (lastX && tmp.x - lastX < 8) {
			lastX = tmp.x;
			continue;
		}
		lastX = tmp.x;
		if (tmp.r > 20) {
			code = code << 1;
			code ++;
			// cout << "add 1 & shift: " << code << endl;
		}
		else {
			code = code << 1;
			// cout << "add 0 & shift: " << code << endl;
		}
	}

	int thrCode = 1;
	int quaCode = 1;
	lastX = 0;
	double lastY;
	int sLast = 0;

	for (int i = 0; i < circs.size(); i ++) {
		coor tmp = circs[i];
		if (!tmp.t)
			continue;
		if (tmp.r < 20) {
			sLast ++;
		}
		else {
			if (sLast == 2) {
				quaCode = (quaCode * 4 + 3) * 4;
			}
			else if (sLast == 1) {
				if (lastY < tmp.y) {
					quaCode = (quaCode * 4 + 1) * 4;
				}
				else {
					quaCode = (quaCode * 4 + 2) * 4;
				}
			}
			else {
				quaCode *= 4;
			}
			sLast = 0;
		}
		lastX = tmp.x;
		lastY = tmp.y;
	}
	if (sLast == 1) {
		if (lastY < circs[circs.size() - 1].y)
			quaCode = quaCode * 4 + 1;
		else
			quaCode = quaCode * 4 + 2;
	}
	else if (sLast == 2)
		quaCode = quaCode * 4 + 3;

	// clog << circles.size() << " in total." << endl;

	ofstream outFile("./stat", std::ios::app);
	if (s) {
		outFile << name << "\t"
				<< sCount << "\t"
				<< lCount << "\t"
				<< code << "\t"
				<< quaCode << endl;
		path = outPath + name + ".png";
		imwrite(path.c_str(), src);
	}
	outFile.close();

	return 1;
}
