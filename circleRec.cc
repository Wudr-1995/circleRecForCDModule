#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>

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
	erode(src, src, kernel);
	// medianBlur(src,src,3);

	vector<Vec3f> circles0;
	vector<Vec3f> circles1;
	vector<Vec4i> lines;
	HoughLinesP(src, lines, 1, CV_PI / 180, 80, 20, 10);
	HoughCircles(src, circles0, CV_HOUGH_GRADIENT, 1, 50, 20, 15, 9, 14);
	HoughCircles(src, circles1, CV_HOUGH_GRADIENT, 1, 50, 40, 15, 30, 40);
	vector<coor> circs;

	cvtColor(src, src, CV_GRAY2RGB);
	for (int i = 0; i < circles0.size(); i ++) {
		Vec3f c = circles0[i];
		// cout << "(" << c[0] << ", " << c[1] << ")" << endl;
		// cout << c[2] << endl;
		circle(src, Point(c[0], c[1]), c[2], Scalar(0,255,255), 3, CV_AA);
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
	code << 1;
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
		if (lastX && tmp.x - lastX < 4) {
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

	// clog << circles.size() << " in total." << endl;

	ofstream outFile("./stat", std::ios::app);
	if (s) {
		outFile << sCount << "\t"
				<< lCount << "\t"
				<< code << endl;
		path = outPath + name + ".png";
		imwrite(path.c_str(), src);
	}
	outFile.close();

	return 1;
}
