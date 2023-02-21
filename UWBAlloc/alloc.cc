#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

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
	ofstream onlyAvi("./forLabels");
	ifstream axis("./axis");
	string loc;
	int n, len;
	string id;

	map<int, int> req;
	map<int, int> ord;
	int lens[8] = {90, 87, 82, 77, 72, 67, 62, 57};
	ord[90] = 0;
	ord[87] = 1;
	ord[82] = 2;
	ord[77] = 3;
	ord[72] = 4;
	ord[67] = 5;
	ord[62] = 6;
	ord[57] = 7;

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
		if (!n)
			req[len] ++;
	}
	require.close();

	map<string, int> loced;

	for (int i = 0; i < size; i ++) {
		int aimLen = getLength(ids[i]);
		if (!req[getLength(ids[i])]) {
			int j = ord[getLength(ids[i])];
			j ++;
			while (!lens[j]) {
				j ++;
			}
			aimLen = lens[j];
		}
		require.open("./input");
		while (require.good()) {
			require >> len >> loc >> n;
			if (!require.good())
				break;
			if (len == aimLen && !n) {
				UWBXLoc tmp;
				tmp.length = len;
				tmp.loc = loc;
				tmp.id = ids[i];
				loced[loc] ++;
				locs.push_back(tmp);
				break;
			}
		}
		require.close();
	}

	require.open("./input");
	while (require.good()) {
		require >> len >> loc >> n;
		if (!require.good())
			break;
		if (loced[loc]) {
			outFile << len << "\t"
					<< loc << "\t"
					<< "" << endl;
		}
	}

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

	map<string, int> axs;
	int ax;
	for (int i = 0; i < 200; i ++) {
		axis >> loc >> ax;
		axs[loc] = ax;
	}

	for (int i = 0; i < size; i ++) {
		if (locs[i].id.size() > 1) {
			onlyAvi << axs[locs[i].loc] << "\t"
					<< locs[i].loc << "\t"
					<< locs[i].id << endl;
		}
	}

	require.close();
	avi.close();
	outFile.close();
	onlyAvi.close();
	axis.close();
}
