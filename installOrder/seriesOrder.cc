#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

struct FLOOR {
	vector<string> uwbs;
	vector<int> cnts;
	vector<int> lens;
	int floor;
};

int getLayer(string pid) {
	int res;
	res = (pid[2] - 48) * 10 + (pid[3] - 48);
	return res;
}

int main(void) {
	ifstream infile("./NorthPMT");
	string uwbId, pid;
	int cntId, chId;
	map<string, int> cnts;
	FLOOR FLOORS[60];
	for (int i = 0; i < 60; i ++)
		FLOORS[i].floor = i + 1;
	while (infile.good()) {
		infile >> uwbId >> cntId >> chId >> pid;
		if (!infile.good())
			break;
		int floor = getLayer(pid);
		string tmp = uwbId + to_string(cntId);
		cout << tmp << endl;
		if (!cnts[tmp]) {
			cnts[tmp] ++;
			FLOORS[floor - 1].cnts.push_back(cntId);
			FLOORS[floor - 1].uwbs.push_back(uwbId);
			if (cntId <= 4)
				FLOORS[floor - 1].lens.push_back(5);
			else
				FLOORS[floor - 1].lens.push_back(10);
			cout << cnts[tmp] << endl;
		}
	}
	cout << "Outputing" << endl;
	infile.close();
	ofstream outfile("./NorthInstallOrder");
	for (int i = 59; i >= 0; i --) {
		size_t len = FLOORS[i].cnts.size();
		for (int j = 0; j < len; j ++) {
			outfile << i + 1 << "\t"
					<< FLOORS[i].uwbs[j] << "\t"
					<< FLOORS[i].cnts[j] << "\t"
					<< FLOORS[i].lens[j] << endl;
		}
	}
	outfile.close();
	return 1;
}
