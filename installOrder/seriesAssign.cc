#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

int main(void) {
	ifstream infile("./moduleFloorRequire");
	int length;
	string floor;
	map<string, int> req10;
	map<string, int> req5;

	map<string, int> wls10;
	map<string, int> wls5;
	wls10["W1"] = 25;
	wls10["W2"] = 291;
	wls10["W3"] = 505;
	wls5["W1"] = 43;
	wls5["W2"] = 359;
	wls5["W3"] = 398;

	string wls[3] = {"W1", "W2", "W3"};
	int tot = 0;
	for (int i = 0; i < 3; i ++) {
		tot += wls10[wls[i]];
		tot += wls5[wls[i]];
	}
	cout << tot << endl;

	vector<string> fls;

	string last("0");
	while (infile.good()) {
		infile >> floor >> length;
		if (!infile.good())
			break;
		if (length == 10)
			req10[floor] ++;
		else
			req5[floor] ++;
		if (floor.compare(last)) {
			last = floor;
			fls.push_back(floor);
		}
	}
	infile.close();

	map<string, int> w1Req10;
	map<string, int> w2Req10;
	map<string, int> w3Req10;

	map<string, int> w1Req5;
	map<string, int> w2Req5;
	map<string, int> w3Req5;

	int size = fls.size();

	for (string& fl : fls) {
		int r10 = req10[fl];
		int r5 = req5[fl];
		if (wls10["W1"] && r10) {
			if (wls10["W1"] >= r10) {
				wls10["W1"] -= r10;
				w1Req10[fl] = r10;
				r10 = 0;
			}
			else if (wls10["W1"] < r10) {
				w1Req10[fl] = wls10["W1"];
				r10 -= wls10["W1"];
				wls10["W1"] = 0;
			}
		}
		if (wls10["W2"] && r10) {
			if (wls10["W2"] >= r10) {
				wls10["W2"] -= r10;
				w2Req10[fl] = r10;
				r10 = 0;
			}
			else if (wls10["W2"] < r10) {
				w2Req10[fl] = wls10["W2"];
				r10 -= wls10["W2"];
				wls10["W2"] = 0;
			}
		}
		if (wls10["W3"] && r10) {
			if (wls10["W3"] >= r10) {
				wls10["W3"] -= r10;
				w3Req10[fl] = r10;
				r10 = 0;
			}
			else if (wls10["W3"] < r10) {
				w3Req10[fl] = wls10["W3"];
				r10 -= wls10["W3"];
				wls10["W3"] = 0;
			}
		}

		if (wls5["W1"] && r5) {
			if (wls5["W1"] >= r5) {
				wls5["W1"] -= r5;
				w1Req5[fl] = r5;
				r5 = 0;
			}
			else if (wls5["W1"] < r5) {
				w1Req5[fl] = wls5["W1"];
				r5 -= wls5["W1"];
				wls5["W1"] = 0;
			}
		}
		if (wls5["W2"] && r5) {
			if (wls5["W2"] >= r5) {
				wls5["W2"] -= r5;
				w2Req5[fl] = r5;
				r5 = 0;
			}
			else if (wls5["W2"] < r5) {
				w2Req5[fl] = wls5["W2"];
				r5 -= wls5["W2"];
				wls5["W2"] = 0;
			}
		}
		if (wls5["W3"] && r5) {
			if (wls5["W3"] >= r5) {
				wls5["W3"] -= r5;
				w3Req5[fl] = r5;
				r5 = 0;
			}
			else if (wls5["W3"] < r5) {
				w3Req5[fl] = wls5["W3"];
				r5 -= wls5["W3"];
				wls5["W3"] = 0;
			}
		}
	}

	ofstream outfile("./assignment");
	cout << size << endl;
	for (string& fl : fls) {
		outfile << fl << "\t"
				<< 10 << "\t"
				<< req10[fl] << "\t"
				<< w1Req10[fl] << "\t"
				<< w2Req10[fl] << "\t"
				<< w3Req10[fl] << endl;
		outfile << fl << "\t"
				<< 5 << "\t"
				<< req5[fl] << "\t"
				<< w1Req5[fl] << "\t"
				<< w2Req5[fl] << "\t"
				<< w3Req5[fl] << endl;
	}
	outfile.close();
}
