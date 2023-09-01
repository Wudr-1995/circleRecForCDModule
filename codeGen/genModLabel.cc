#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

Mat genTextPart(string mid, string pid, string mop) {
	Mat dstImage(400, 475, CV_8UC3, Scalar::all(255));
	putText(dstImage, "MID:", Point(35, 110), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	putText(dstImage, mid, Point(35, 180), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	putText(dstImage, "PID:", Point(35, 250), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	putText(dstImage, pid, Point(35, 320), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	// if (mop == "Module")
	// 	putText(dstImage, mop, Point(280, 60), FONT_HERSHEY_TRIPLEX, 1.5, Scalar::all(0), 4, 8, 0);
	// else
	// 	putText(dstImage, mop, Point(340, 60), FONT_HERSHEY_TRIPLEX, 1.5, Scalar::all(0), 4, 8, 0);
	return dstImage;
}

int main(int argc, char** argv) {
	string path(argv[1]);
	string idPath = path + "MID_PID.txt";

	ifstream idFile(idPath);
	// ifstream codes(codeList);

	vector<string> mids;
	vector<string> pids;
	vector<string> mcodes;
	vector<string> pcodes;

	string mid, pid, mcode;
	int nan;
	while (idFile.good()) {
		idFile >> mid >> pid;
		if (!idFile.good())
			break;
		// codes >> mcode >> pcode;

		mcode = path + "Module/M" + pid;
		mcode = mcode + ".jpg";

		Mat codeModule = imread(mcode);

		string tmpMid = mid;
		string tmpPid = pid;

		cout << tmpMid << "\t"
			 << tmpPid << endl;

		Mat text0 = genTextPart(tmpMid, tmpPid, "Module");
		cvtColor(text0, text0, CV_RGB2GRAY);

		cvtColor(codeModule, codeModule, CV_RGB2GRAY);

		Mat tmp0(290, 35, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp1(290, 35, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp2(55, 325, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp3(55, 325, CV_8UC3, Scalar(255, 255, 255));

		cvtColor(tmp0, tmp0, CV_RGB2GRAY);
		cvtColor(tmp1, tmp1, CV_RGB2GRAY);
		cvtColor(tmp2, tmp2, CV_RGB2GRAY);
		cvtColor(tmp3, tmp3, CV_RGB2GRAY);

		hconcat(codeModule, tmp0, codeModule);
		// hconcat(tmp1, codeModule, codeModule);
		vconcat(codeModule, tmp2, codeModule);
		vconcat(tmp3, codeModule, codeModule);

		hconcat(text0, codeModule, codeModule);

		putText(codeModule, "Module", Point(300, 50), FONT_HERSHEY_TRIPLEX, 1.5, Scalar::all(0), 4, 8, 0);

		for (int i = 0; i < mid.size(); i ++) {
			if (mid[i] == 47)
				mid[i] = '_';
		}
		// mid[11] = '_';
		mcode = path + "resModule/M" + mid;
		mcode = mcode + ".jpg";

		cout << mcode << endl;

		imwrite(mcode, codeModule);
	}

	idFile.close();

	return 1;
}
