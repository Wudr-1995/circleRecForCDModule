#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

struct MODULE {
	string id;
	int code;
	vector<HOLE> holes;
	int ns;
	int nl;
};

int main(void) {
	ifstream recoResult("./NorthModuleRecoResult");
	ifstream selected("./NorthSelectedHoles");

	map<string, int> ifSelected;
	vector<MODULE> modules;

	string moduleId;
	string pid;
	while (recoResult.good()) {
		reco
	}
}
