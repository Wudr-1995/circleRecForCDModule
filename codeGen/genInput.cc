#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main(int argc, char** argv) {
	string layer(argv[1]);
	int aimLay = stoi(layer);
	string outPath = "N" + layer;
	string name0 = outPath + "/MID_PID.txt";
	string name1 = outPath + "/modInput";
	cout << name0 << endl;

	string mid, pid;
	int lay;
	ifstream p2m("./pid2mid");
	ifstream cableId("./cableId");
	ofstream outfile(name0);
	ofstream of(name1);

	map<string, int> p2c;
	int cid;
	while (cableId.good()) {
		cableId >> pid >> cid;
		if (!cableId.good())
			break;
		p2c[pid] = cid;
	}
	while (p2m.good()) {
		p2m >> lay >> mid >> pid;
		if (!p2m.good())
			break;
		string bcPid = pid;
		if (lay == aimLay) {
			bcPid.erase(bcPid.begin() + 2);
			cout << bcPid << endl;
			cout << p2c[bcPid] << endl;
			if (!p2c[bcPid])
				continue;
			// outfile << aimLay << "\t"
			// 		<< mid << "\t"
			// 		<< pid << "\t"
			// 		<< p2c[bcPid] << endl;
			outfile << mid << "\t"
					<< pid << endl;
			of << "M" << pid << endl;
		}
	}

	p2m.close();
	cableId.close();
	outfile.close();
	of.close();

	return 1;
}
