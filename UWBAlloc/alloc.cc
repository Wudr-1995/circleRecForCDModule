#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct UWBXLoc {
	int length;
	string loc;
	string id;
};

int getLength(string Id) {
	int res = 0;
	res += Id[3] - 48;
	res += (Id[2] - 48) * 10;
	return res;
}

int main(void) {
	ifstream require("./input");
	ifstream avi("./aviliable");
	ofstream outFile("./output");
	string loc;
	int n, len;
	string id;

	vector<string> ids;
	vector<int> aled;

	while (avi.good()) {
		avi >> id;
		if (!avi.good())
			break;
		ids.push_back(id);
		aled.push_back(0);
	}

	vector<UWBXLoc> locs;
	int size = ids.size();
	bool pf = true;
	while (require.good()) {
		require >> len >> loc >> n;
		if (!require.good())
			break;
		cout << loc << "\t"
			 << n << endl;
		for (int i = 0; i < size; i ++) {
			if (!aled[i] && len == getLength(ids[i]) && !n) {
				UWBXLoc tmp;
				tmp.length = len;
				tmp.loc = loc;
				tmp.id = ids[i];
				locs.push_back(tmp);
				aled[i] = 1;
				pf = false;
				break;
			}
		}
		cout << pf << "\t"
			 << len << "\t"
			 << loc << endl;
		if (pf) {
			UWBXLoc tmp;
			tmp.length = len;
			tmp.loc = loc;
			tmp.id = "";
			locs.push_back(tmp);
		}
		pf = true;
	}

	size = locs.size();
	for (int i = 0; i < size; i ++) {
		outFile << locs[i].length << "\t"
				<< locs[i].loc << "\t"
				<< locs[i].id << endl;
	}

	require.close();
	avi.close();
	outFile.close();
}
