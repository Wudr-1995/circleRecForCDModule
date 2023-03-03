#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main(int argc, char** argv) {
	string aimUWB(argv[1]);
	string aimCnt(argv[2]);
	string path = aimUWB + "-" + aimCnt;
	ifstream pid2mid("./MID_PID");
	ifstream cnt2pid("./Cnt2PID");
	ifstream cntInfo("./cntInfos");
	ofstream dstp2m(path + "/MID_PID.txt");
	ofstream dstc2p(path + "/pmtInput");
	ofstream dstCnt(path + "/cntInfo");
	ofstream info(path + "/info.txt");

	string pid, mid, uwb, cnt, ch, type;

	map<string, string> p2m;
	while (pid2mid.good()) {
		pid2mid >> mid >> pid >> pid;
		if (!pid2mid.good())
			break;
		p2m[pid] = mid;
	}
	pid2mid.close();

	info << "PID\t"
		 << "MID\t"
		 << "Type\t"
		 << "CID\t"
		 << "UWB ID\t"
		 << "Connector ID" << endl;
	while (cnt2pid.good()) {
		cnt2pid >> pid >> uwb >> cnt >> ch >> type;
		if (!cnt2pid.good())
			break;
		if (aimUWB == uwb && aimCnt == cnt) {
			dstp2m << pid << "\t"
				   << p2m[pid] << "\t"
				   << type << "\t"
				   << ch << "\t"
				   << uwb << "\t"
				   << cnt << endl;
			info << pid << "\t"
				   << p2m[pid] << "\t"
				   << type << "\t"
				   << ch << "\t"
				   << uwb << "\t"
				   << cnt << endl;
			dstc2p << "S" + pid << endl;
		}
	}

	int ax;
	while (cntInfo.good()) {
		cntInfo >> uwb >> ax >> cnt;
		if (!cntInfo.good())
			break;
		if (aimUWB == uwb && aimCnt == cnt) {
			dstCnt << uwb << "\t"
				 << ax << "\t"
				 << cnt << endl;
		}
	}

	cnt2pid.close();
	dstp2m.close();
	dstc2p.close();
	dstCnt.close();
	info.close();
	cntInfo.close();
}
