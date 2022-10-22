#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <string>
#include <stb_image.h>
#include <stb_image_write.h>

using namespace std;
using namespace cv;

void alloc(unsigned char* &data, int len) {
	data = (unsigned char*)malloc(len);
	memset(data, 0, len);
}

struct vc2 {
	int ux;
	int uy;
	int dx;
	int dy;
};

int main(int argc, char** argv) {
	string path(argv[1]);
	string name(argv[2]);
	string outPath(argv[3]);
	path = path + name + ".jpg";

	int iw, ih, n;
	unsigned char* im = stbi_load(path.c_str(), &iw, &ih, &n, 0);

	int bw = 275;
	int bh = 56;

	// int bw = 1350;
	// int bh = 240;

	vc2 a0, a1, a2, a3;

	a0.ux = 770; a0.uy = 286;
	a1.ux = 2292; a1.uy = 286;
	a2.ux = 770; a2.uy = 2183;
	a3.ux = 2292; a3.uy = 2183;

	// a0.ux = 210; a0.uy = 640;
	// a1.ux = 1800; a1.uy = 640;
	// a2.ux = 210; a2.uy = 2640;
	// a3.ux = 1800; a3.uy = 2640;

	a0.dx = a0.ux + bw; a0.dy = a0.uy + bh;
	a1.dx = a1.ux + bw; a1.dy = a1.uy + bh;
	a2.dx = a2.ux + bw; a2.dy = a2.uy + bh;
	a3.dx = a3.ux + bw; a3.dy = a3.uy + bh;


	int len = bw * bh * n;

	// cout << bw << "\t" << bh << "\t" << n << endl;
	// cout << len << endl;

	unsigned char* im0 = NULL;
	unsigned char* im1 = NULL;
	unsigned char* im2 = NULL;
	unsigned char* im3 = NULL;

	alloc(im0, len);
	alloc(im1, len);
	alloc(im2, len);
	alloc(im3, len);

	cout << "Processing..." << endl;

	int count = 0;
	for (int i = 0; i < ih; i ++) {
		for (int j = 0; j < iw; j ++) {
			for (int k = 0; k < n; k ++) {
				if (i >= a0.uy && i < a0.dy && j >= a0.ux && j < a0.dx) {
					int tmp = (i * iw + j) * n + k;
					im0[count] = im[tmp];
					count ++;
				}
			}
		}
	}

	count = 0;
	for (int i = 0; i < ih; i ++) {
		for (int j = 0; j < iw; j ++) {
			for (int k = 0; k < n; k ++) {
				if (i >= a1.uy && i < a1.dy && j >= a1.ux && j < a1.dx) {
					im1[count] = im[(i * iw + j) * n + k];
					count ++;
				}
			}
		}
	}

	count = 0;
	for (int i = 0; i < ih; i ++) {
		for (int j = 0; j < iw; j ++) {
			for (int k = 0; k < n; k ++) {
				if (i >= a2.uy && i < a2.dy && j >= a2.ux && j < a2.dx) {
					im2[count] = im[(i * iw + j) * n + k];
					count ++;
				}
			}
		}
	}

	count = 0;
	for (int i = 0; i < ih; i ++) {
		for (int j = 0; j < iw; j ++) {
			for (int k = 0; k < n; k ++) {
				if (i >= a3.uy && i < a3.dy && j >= a3.ux && j < a3.dx) {
					im3[count] = im[(i * iw + j) * n + k];
					count ++;
				}
			}
		}
	}

	cout << "saving..." << endl;
	path = outPath + name + "_0.png";
	stbi_write_png(path.c_str(), bw, bh, n, im0, 0);
	Mat png0 = imread(path.c_str());
	cvtColor(png0, png0, CV_RGB2GRAY);
	threshold(png0, png0, 125, 255, THRESH_BINARY_INV);
	png0 = cv::Scalar(255, 255, 255) - png0;
	imwrite(path.c_str(), png0);

	path = outPath + name + "_1.png";
	stbi_write_png(path.c_str(), bw, bh, n, im1, 0);
	Mat png1 = imread(path.c_str());
	cvtColor(png1, png1, CV_RGB2GRAY);
	threshold(png1, png1, 125, 255, THRESH_BINARY_INV);
	png1 = cv::Scalar(255, 255, 255) - png1;
	imwrite(path.c_str(), png1);

	path = outPath + name + "_2.png";
	stbi_write_png(path.c_str(), bw, bh, n, im2, 0);
	Mat png2 = imread(path.c_str());
	cvtColor(png2, png2, CV_RGB2GRAY);
	threshold(png2, png2, 125, 255, THRESH_BINARY_INV);
	png2 = cv::Scalar(255, 255, 255) - png2;
	imwrite(path.c_str(), png2);

	path = outPath + name + "_3.png";
	stbi_write_png(path.c_str(), bw, bh, n, im3, 0);
	Mat png3 = imread(path.c_str());
	cvtColor(png3, png3, CV_RGB2GRAY);
	threshold(png3, png3, 125, 255, THRESH_BINARY_INV);
	png3 = cv::Scalar(255, 255, 255) - png3;
	imwrite(path.c_str(), png3);

	cout << "free" << endl;
	stbi_image_free(im);

	cout << "return" << endl;
	return 1;
}
