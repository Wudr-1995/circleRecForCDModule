#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

string genMID(string NoS, int floor, int nOrd) {
	string res = NoS;
	string floorId("00");
	string ord("00");
	if (floor < 10) {
		floorId[1] += floor;
	}
	else {
		floorId[1] += floor % 10;
		floorId[0] += floor / 10;
	}
	floorId = floorId + "-";

	if (nOrd < 10)
		ord[1] += nOrd;
	else {
		ord[1] += nOrd % 10;
		ord[0] += nOrd / 10;
	}

	res = res + floorId + ord;
	return res;
}

int main(void) {
	ifstream infile("./entryInFloor");
	string tmp;
	int count;
	infile >> tmp;
	// cout << tmp << endl;
	while (infile.good()) {
		if (tmp[0] == 78) {
			int ord = 0;
			infile >> count;
			// cout << count << endl;
			int tot = 0;
			bool done = false;
			while (infile.good()) {
				infile >> tmp;
				if (tmp[0] == 78)
					break;
				int id = stoi(tmp);
				if (id && !done)
					ord ++;
				else {
					done = true;
					// cout << count << "\t" << tmp << "\t" << ord << endl;
				}
				tot ++;
				// cout << tmp << endl;
			}
			// cout << "out: " << count << "\t" << tmp << "\t" << ord << endl;
			ord ++;
			string mid = genMID("GJ-N", count, 1);
			mid = mid + "(";
			mid = mid + to_string(ord);
			mid = mid + "/";
			mid = mid + to_string(tot);
			mid = mid + ")";
			cout << mid << endl;
			// cout << tmp << endl;
			// cout << count << "\t" << ord << endl;
		}
	}
	infile.close();
	return 1;
}
