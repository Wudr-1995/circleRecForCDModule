#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

Mat genTextPart(string mid, string pid, string mop, string cid, string loc) {
	Mat dstImage(400, 475, CV_8UC3, Scalar::all(255));
	putText(dstImage, "MID:", Point(35, 90), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	putText(dstImage, mid, Point(35, 160), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	putText(dstImage, "PID:", Point(35, 230), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	putText(dstImage, pid, Point(35, 300), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	string CID = "CID: " + cid;
	CID = CID + "  Type: ";
	CID = CID + loc;
	putText(dstImage, CID, Point(35, 370), FONT_HERSHEY_TRIPLEX, 1.3, Scalar::all(0), 4, 8, 0);
	// if (mop == "Module")
	// 	putText(dstImage, mop, Point(280, 60), FONT_HERSHEY_TRIPLEX, 1.5, Scalar::all(0), 4, 8, 0);
	// else
	// 	putText(dstImage, mop, Point(340, 60), FONT_HERSHEY_TRIPLEX, 1.5, Scalar::all(0), 4, 8, 0);
	return dstImage;
}

int main(int argc, char** argv) {
	// string codeList(argv[2]);
	string path(argv[1]);
	string idPath = path + "MID_PID.txt";

	ifstream idFile(idPath);
	// ifstream codes(codeList);

	vector<string> mids;
	vector<string> pids;
	vector<string> mcodes;
	vector<string> pcodes;

	string mid, pid, loc, pcode, cid, uwb, ax, cnt;
	while (idFile.good()) {
		idFile >> pid >> mid >> loc >> cid >> uwb >> cnt;
		if (!idFile.good())
			break;

		uwb = uwb + "-";
		uwb = uwb + cnt;

		pcode = path + "PMT/S" + pid;
		pcode = pcode + ".jpg";

		Mat codePMT = imread(pcode);

		string tmpMid = mid;
		string tmpPid = pid;

		cout << tmpMid << "\t"
			 << tmpPid << endl;

		Mat text1 = genTextPart(tmpMid, tmpPid, "PMT", cid, loc);
		cvtColor(text1, text1, CV_RGB2GRAY);

		cvtColor(codePMT, codePMT, CV_RGB2GRAY);

		Mat tmp0(290, 35, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp1(290, 35, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp2(55, 325, CV_8UC3, Scalar(255, 255, 255));
		Mat tmp3(55, 325, CV_8UC3, Scalar(255, 255, 255));

		cvtColor(tmp0, tmp0, CV_RGB2GRAY);
		cvtColor(tmp1, tmp1, CV_RGB2GRAY);
		cvtColor(tmp2, tmp2, CV_RGB2GRAY);
		cvtColor(tmp3, tmp3, CV_RGB2GRAY);

		hconcat(codePMT, tmp0, codePMT);
		// hconcat(tmp1, codePMT, codePMT);
		vconcat(codePMT, tmp2, codePMT);
		vconcat(tmp3, codePMT, codePMT);

		hconcat(text1, codePMT, codePMT);

		putText(codePMT, "SPMT", Point(340, 50), FONT_HERSHEY_TRIPLEX, 1.5, Scalar::all(0), 4, 8, 0);
		putText(codePMT, uwb, Point(490, 370), FONT_HERSHEY_TRIPLEX, 1.5, Scalar::all(0), 4, 8, 0);

		mid[11] = '_';
		pcode = path + "resPMT/S" + mid;
		pcode = pcode + ".jpg";

		cout << pcode << endl;

		imwrite(pcode, codePMT);
	}

	idFile.close();

	return 1;
}
