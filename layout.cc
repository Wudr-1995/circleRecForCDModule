#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Hole {
	int floor;
	int mod;
	int pos;
};

int main(void) {
	ifstream infile("./n60id");
	ifstream holes("./n60stat");
	string tmp;
	int ns, nl, code;
	int stm = 0;
	int sth = 1;

	vector<vector<int>> mods;
	vector<vector<int>> modId;
	vector<string> ids;
	vector<int> nss;
	vector<int> nls;
	while (infile.good()) {
		getline(infile, tmp);
		if (!infile.good())
			break;
		ids.push_back(tmp);
		holes >> ns >> nl >> code;
		nss.push_back(ns);
		nls.push_back(nl);
		Hole sh;
		Hole lh;
		char f1 = tmp[4];
		char f0 = tmp[5];
		char m1 = tmp[7];
		char m0 = tmp[8];
		int floor = (int)(f1 - 48) * 10 + (int)(f0 - 48);
		int mod = (int)(m1 - 48) * 10 + (int)(m0 - 48);

		if (ns % 2 != 0) {
			// cout << ns << endl;
			// cout << tmp << "\t"
			// 	 << floor << "\t"
			// 	 << mod << endl;
		}

		vector<int> modHoles;
		while (code) {
			int b = code % 2;
			code /= 2;
			if (!code) {
				break;
			}
			modHoles.push_back(b);
		}
		mods.push_back(modHoles);
	}

	int nholes = 0;
	int nlholes = 0;
	int nmods = mods.size();
	for (int i = 0; i < mods.size(); i ++) {
		nholes += mods[i].size();
		for (int j = 0; j < mods[i].size(); j ++) {
			if (mods[i][j])
				nlholes ++;
		}
	}

	int offset = 0;
	bool bflag = false;
	for (int i = 0; i < mods.size(); i ++) {
		for (int j = 0; j < mods[i].size(); j ++) {
			if (i == stm && j == sth) {
				bflag = true;
				break;
			}
			if (mods[i][j])
				offset ++;
		}
		if (bflag)
			break;
	}
	// cout << offset << endl;

	int cc = 0;
	for (int i = 0; i < mods.size(); i ++) {
		vector<int> tmpC;
		for (int j = 0; j < mods[i].size(); j ++) {
			if (mods[i][j]) {
				cc ++;
				int res = cc - offset;
				if (!res)
					res = nlholes;
				tmpC.push_back(res);
			}
			else {
				tmpC.push_back(0);
			}
		}
		modId.push_back(tmpC);
	}

	// for (int i = 0; i < mods.size(); i ++) {
	// 	cout << "i: " << i << endl;
	// 	for (int j = 0; j < mods[i].size(); j ++) {
	// 		cout << mods[i][j] << endl;
	// 	}
	// }

	int choles = 1 - offset;
	int cmods = 0;
	int mholes = 0;
	int mmods = 0;
	while (mmods < nmods) {
		// cout << mmods << "\t"
		// 	 << nmods << "\t"
		// 	 << mholes << "\t"
		// 	 << mods[mmods].size() << endl;
		mods[mmods][mholes];
		int tmpi, tmpj;
		tmpi = mmods;
		tmpj = mholes;
		if (mods[mmods][mholes] == 0) {
			if (!tmpj) {
				if (!tmpi) {
					tmpi = nmods - 1;
					tmpj = mods[tmpi].size() - 1;
				}
			}
			else {
				tmpj --;
			}
		}
		else {
			mholes ++;
			if (mholes == mods[mmods].size()) {
				mholes = 0;
				// cout << "mmods ++" << endl;
				mmods ++;
			}
			choles ++;
			continue;
		}

		string holeLoc = ids[mmods];
		holeLoc = holeLoc + "(S";
		string upLoc = holeLoc + to_string(mholes * 2 - 1);
		upLoc = upLoc + "/";
		upLoc = upLoc + to_string(nss[mmods]);
		upLoc = upLoc + ")";

		string tmp = ids[mmods];
		char f1 = tmp[4];
		char f0 = tmp[5];
		char m1 = tmp[7];
		char m0 = tmp[8];
		int floor = (int)(f1 - 48) * 10 + (int)(f0 - 48);
		int mod = (int)(m1 - 48) * 10 + (int)(m0 - 48);

		string lpmtLoc("N-");
		lpmtLoc = lpmtLoc + to_string(floor);
		lpmtLoc = lpmtLoc + "-";
		if (modId[tmpi][tmpj] < 10) {
			lpmtLoc = lpmtLoc + "00";
			lpmtLoc = lpmtLoc + to_string(modId[tmpi][tmpj]);
		}
		else if (modId[tmpi][tmpj] < 100) {
			lpmtLoc = lpmtLoc + "0";
			lpmtLoc = lpmtLoc + to_string(modId[tmpi][tmpj]);
		}
		else
			lpmtLoc = lpmtLoc + to_string(choles);

		ofstream outfile("./coorResult", std::ios::app);
		cout << upLoc << "\t" << lpmtLoc + ".5U" << endl;
		outfile << upLoc << "\t"
				<< "S" << lpmtLoc + ".5U" << "\t"
				<< "M" << lpmtLoc + ".5U" << "\t"
				<< lpmtLoc + ".5U" << endl;

		upLoc = holeLoc + to_string(mholes * 2);
		upLoc = upLoc + "/";
		upLoc = upLoc + to_string(nss[mmods]);
		upLoc = upLoc + ")";

		cout << upLoc << "\t" << lpmtLoc + ".5D" << endl;
		outfile << upLoc << "\t"
				<< "S" << lpmtLoc + ".5D" << "\t"
				<< "M" << lpmtLoc + ".5D" << "\t"
				<< lpmtLoc + ".5D" << endl;

		mholes ++;
		if (mholes == mods[mmods].size()) {
			mholes = 0;
			mmods ++;
		}
		if (mmods == mods.size()) {
			mmods = 0;
		}
	}

}
