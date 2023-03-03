#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

Mat genTextPart(string uwb, string longi, string no) {
	Mat dstImage(400, 475, CV_8UC3, Scalar::all(255));
	// putText(dstImage, "UWB loc.:", Point(100, 110), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	uwb = "UWB: " + uwb;
	putText(dstImage, uwb, Point(80, 150), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	// putText(dstImage, "Longitude:", Point(100, 250), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	// longi = ": " + longi;
	// putText(dstImage, longi, Point(200, 220), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	no = "No. : " + no;
	putText(dstImage, no, Point(80, 290), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	return dstImage;
}

int main(int argc, char** argv) {
	// string codeList(argv[2]);
	string path(argv[1]);
	string idPath = path + "cntInfo";

	ifstream idFile(idPath);
	// ifstream codes(codeList);
	Mat axisLabel = imread("./axis.png");

	cvtColor(axisLabel, axisLabel, CV_RGB2GRAY);

	string qrCode, uwb, longi, no;
	while (idFile.good()) {
		idFile >> uwb >> longi >> no;
		if (!idFile.good())
			break;

		qrCode = path + "C" + uwb;
		qrCode = qrCode + ".jpg";

		Mat codeu = imread(qrCode);

		cout << uwb << "\t" << longi << "\t" << no << endl;

		Mat text1 = genTextPart(uwb, longi, no);
		cvtColor(text1, text1, CV_RGB2GRAY);

		cvtColor(codeu, codeu, CV_RGB2GRAY);

		Mat tmp0(290, 35, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp1(290, 35, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp2(55, 325, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp3(55, 325, CV_8UC3, Scalar(255, 255, 255));

		cvtColor(tmp0, tmp0, CV_RGB2GRAY);
		cvtColor(tmp1, tmp1, CV_RGB2GRAY);
		cvtColor(tmp2, tmp2, CV_RGB2GRAY);
		cvtColor(tmp3, tmp3, CV_RGB2GRAY);

		hconcat(codeu, tmp0, codeu);
		vconcat(codeu, tmp2, codeu);
		vconcat(tmp3, codeu, codeu);

		hconcat(text1, codeu, codeu);

		putText(codeu, "Connector", Point(290, 50), FONT_HERSHEY_TRIPLEX, 1.5, Scalar::all(0), 4, 8, 0);

		for (int i = 175; i < axisLabel.rows + 175; i ++) {
			for (int j = 70; j < axisLabel.cols + 70; j ++) {
				*(codeu.data + codeu.step[0] * i + codeu.step[1] * j) = *(axisLabel.data + axisLabel.step[0] * (i - 175) + axisLabel.step[1] * (j - 70));
			}
		}

		longi = ": " + longi;
		putText(codeu, longi, Point(170, 220), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);

		qrCode = path + "C" + uwb;
		qrCode = qrCode + "-";
		qrCode = qrCode + no;
		qrCode = qrCode + ".jpg";

		cout << qrCode << endl;

		imwrite(qrCode, codeu);
	}

	idFile.close();

	return 1;
}
