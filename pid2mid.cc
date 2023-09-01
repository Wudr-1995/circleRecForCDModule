#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct HOLE {
	bool sol;// spmt: true, lpmt: false
	int n;// the number of PMTs
	vector<string> mid;
	vector<string> pid;
	bool uod;// u: true, d: false
	string mod;
	int ns;
};

struct MODULE {
	string id;
	int code;
	vector<HOLE> holes;
	int ns;
	int nl;
};

struct FLOOR {
	vector<MODULE> modules;
	int floor;
	string entry;
};

int getLayer(string modId) {
	int res;
	res = (modId[4] - 48) * 10 + (modId[5] - 48);
	return res;
}

int getOrder(string modId) {
	int res;
	res = (modId[7] - 48) * 10 + (modId[8] - 48);
	return res;
}

int getLpmtOrd(string entry) {
	int res;
	res = entry[10] - 48;
	return res;
}

string genPID(string NoS, int floor, int nOrd) {
	string res = NoS;
	string floorId("00");
	string ord("000");
	if (floor < 10) {
		floorId[1] += floor;
	}
	else {
		floorId[1] += floor % 10;
		floorId[0] += floor / 10;
	}
	floorId = floorId + "-";

	if (nOrd < 10)
		ord[2] += nOrd;
	else if (nOrd < 100) {
		ord[2] += nOrd % 10;
		ord[1] += nOrd / 10;
	}
	else {
		ord[2] += nOrd % 10;
		nOrd /= 10;
		ord[1] += nOrd % 10;
		nOrd /= 10;
		ord[0] += nOrd % 100;
	}

	res = res + floorId + ord;
	return res;
}

string genMID(string mod, int ord, int tot) {
	string res;
	res = mod + "(" + to_string(ord) + "/" + to_string(tot) + ")";
	return res;
}

void decode(int code, string modId, vector<HOLE>& holes, int ns) {
	while (code > 1) {
		HOLE hole;
		int type = code % 4;
		code /= 4;
		if (type)
			hole.sol = true;
		else
			hole.sol = false;

		if (type < 3)
			hole.n = 1;
		else
			hole.n = 2;

		if (type == 1)
			hole.uod = true;
		else if (type == 2)
			hole.uod = false;
		else ;

		hole.mod = modId;
		hole.ns = ns;

		holes.push_back(hole);
	}
}

int getPMTIndex(vector<HOLE>& holes, int floor, int ord, int pmtOrd) {
	cout << floor << "\t"
		 << ord << "\t"
		 << pmtOrd << endl;
	int tmpOrd = 1;
	int entOrd;
	int len = holes.size();
	for (int i = 0; i < len; i ++) {
		if (floor == getLayer(holes[i].mod)
			&& ord == getOrder(holes[i].mod)
			&& !holes[i].sol) {
			if (pmtOrd == tmpOrd) {
				entOrd = i;
				break;
			}
			tmpOrd ++;
		}
	}
	return entOrd;
}

int main(void) {
	ifstream idFile("./southModules");
	ifstream recoFile("./SorthRecoResult");
	ifstream firstMid("./southEntries");

	string nan;
	int ns, nl, code;
	string modId;
	string entry;

	vector<FLOOR> floors;
	int nModules = 0;
	bool bk = false;

	idFile >> modId;
	recoFile >> nan >> ns >> nl >> nan >> code;
	firstMid >> entry;
	// cout << modId << "\t" << code << endl;

	ofstream p2m("./check_southPID2MID");
	ofstream ck("./check_southCheckResult");
	ofstream lpmtP2m("./check_southLPMTPID2MID");

	while (idFile.good()) {
		FLOOR floor;
		MODULE mod;
		mod.id = modId;
		mod.code = code;
		mod.ns = ns;
		mod.nl = nl;
		decode(code, modId, mod.holes, ns);
		floor.modules.push_back(mod);
		int nlay = getLayer(modId);
		floor.floor = nlay;
		floor.entry = entry;

		while (nlay == getLayer(modId)) {
			idFile >> modId;
			recoFile >> nan >> ns >> nl >> nan >> code;
			if (!idFile.good()) {
				bk = true;
				break;
			}
			// cout << modId << "\t" << code << endl;
			if (nlay == getLayer(modId)) {
				MODULE mod;
				mod.id = modId;
				mod.code = code;
				mod.ns = ns;
				mod.nl = nl;
				decode(code, modId, mod.holes, ns);
				floor.modules.push_back(mod);
			}
		}

		floors.push_back(floor);
		firstMid >> entry;
		if (bk)
			break;
	}

	int nfloors = floors.size();
	int nLpmt = 0;
	int nSpmt = 0;
	for (int i = 0; i < nfloors; i ++) {
		cout << "====================" << floors[i].floor << "====================" << endl;
		nModules = floors[i].modules.size();
		vector<HOLE> holes;

		for (int j = 0; j < nModules; j ++) {
			// clog << floors[i].modules[j].code << "\t"
			// 	 << floors[i].modules[j].id << endl;
			decode(floors[i].modules[j].code,
				   floors[i].modules[j].id,
				   holes,
				   floors[i].modules[j].ns);
		}

		// cout << getOrder(floors[i].entry) << "\t" << getLpmtOrd(floors[i].entry) << endl;
		int st = getPMTIndex(holes, floors[i].floor,
							 getOrder(floors[i].entry),
							 getLpmtOrd(floors[i].entry));

		// assign the pid to the holes of LPMT
		int nHoles = holes.size();
		int id = 1;
		for (int j = 0; j < nHoles; j ++) {
			if (!holes[st].sol) {
				string pid = genPID("S-", floors[i].floor, id);
				holes[st].pid.push_back(pid);
				// cout << "LPMT PID: " << pid << "\t" << id << endl;
				id ++;
				nLpmt ++;
			}
			else {
				nSpmt += holes[st].n;
			}
			st ++;
			if (st >= nHoles)
				st -= nHoles;
		}

		// check the decoding process
		for (int j = 0; j < nModules; j ++) {
			vector<HOLE> tmpHole;
			decode(floors[i].modules[j].code,
				   floors[i].modules[j].id,
				   tmpHole,
				   floors[i].modules[j].ns);
			int NL = 0;
			int NS = 0;
			for (int k = 0; k < tmpHole.size(); k ++) {
				if (!tmpHole[k].sol) {
					NL ++;
				}
				else {
					NS += tmpHole[k].n;
				}
			}
			ck << NS << "\t" << NL << endl;
		}

		// assign the mid to the holes of SPMT
		id = 1;
		string lastMod = holes[0].mod;
		for (int j = 0; j < nHoles; j ++) {
			if (holes[j].mod != lastMod)
				id = 1;
			if (holes[j].sol) {
				if (holes[j].n == 2) {
					string mid0 = genMID(holes[j].mod, id, holes[j].ns);
					id ++;
					string mid1 = genMID(holes[j].mod, id, holes[j].ns);
					id ++;
					holes[j].mid.push_back(mid0);
					holes[j].mid.push_back(mid1);
					// cout << "SPMT MID: " << mid0 << "\t" << mid1 << endl;
				}
				else {
					string mid = genMID(holes[j].mod, id, holes[j].ns);
					id ++;
					holes[j].mid.push_back(mid);
				}
			}
			lastMod = holes[j].mod;
		}

		// cout << nHoles << endl
		// 	 << holes.size() << endl;
		cout << "Assign the pid to the holes of PMT" << endl;
		// assign the pid to the holes of SPMT
		for (int j = 0; j < nHoles; j ++) {
			// cout << j << endl;
			if (holes[j].sol) {
				int index = j - 1;
				if (index < 0)
					index = -- nHoles;
				// cout << index << endl;
				string pid = holes[index].pid[0];
				clog << index << "\t" << pid << "\t" << holes[j].mid[0] << endl;
				pid = pid + ".5";
				if (holes[j].n == 2) {
					string pid0 = pid + "-U";
					string pid1 = pid + "-D";
					holes[j].pid.push_back(pid0);
					holes[j].pid.push_back(pid1);
				}
				else {
					if (holes[j].uod)
						pid = pid + "-U";
					else
						pid = pid + "-D";
					holes[j].pid.push_back(pid);
				}
			}
		}

		cout << "Output..." << endl;
		for (int j = 0; j < nHoles; j ++) {
			if (holes[j].sol) {
				if (holes[j].n == 2) {
					p2m << getLayer(holes[j].mod) << "\t" << holes[j].mid[0] << "\t" << holes[j].pid[0] << endl
						<< getLayer(holes[j].mod) << "\t" << holes[j].mid[1] << "\t" << holes[j].pid[1] << endl;
				}
				else
					p2m << getLayer(holes[j].mod) << "\t" << holes[j].mid[0] << "\t" << holes[j].pid[0] << endl;
			}
			else {
				lpmtP2m << getLayer(holes[j].mod) << "\t" << holes[j].mod << "\t" << holes[j].pid[0] << endl;
			}
		}
	}

	cout << nLpmt << "\t" << nSpmt << endl;

	idFile.close();
	recoFile.close();
	firstMid.close();
	p2m.close();
	ck.close();
	lpmtP2m.close();

	return 1;
}
