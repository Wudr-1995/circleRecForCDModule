#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	string path(argv[1]);

	Mat src;
	src = imread(path.c_str());

	if (src.empty()) {
		clog << "Can not load the image!" << endl;
		return 0;
	}

	Mat kernel;
	kernel = getStructuringElement(1, Size(3, 3));

	cvtColor(src, src, CV_RGB2GRAY);
	threshold(src, src, 125, 255, THRESH_BINARY_INV);
	erode(src, src, kernel);
	// medianBlur(src,src,3);

	vector<Vec3f> circles;
	HoughCircles(src, circles, CV_HOUGH_GRADIENT, 1, 15, 30, 20, 8, 60);

	cvtColor(src, src, CV_GRAY2RGB);
	for (int i = 0; i < circles.size(); i ++) {
		Vec3f c = circles[i];
		cout << "(" << c[0] << ", " << c[1] << ")" << endl;
		cout << c[2] << endl;
		circle(src, Point(c[0], c[1]), c[2], Scalar(0,255,255), 3, CV_AA);
	}

	clog << circles.size() << " in total." << endl;

	imwrite("./proIm0.png", src);

	return 1;
}
