#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

struct relation {
	map<string, int> pidAllowed;
	map<string, string> pid2mid;
	map<string, string> pid2type;

	map<string, int> uwb2uwbPos;
	map<string, int> uwb2room;
	map<string, int> uwb2row;
	map<string, int> uwb2rack;
	map<string, int> uwb2device;
	map<string, int> uwbCnt2length;
	map<string, string> uwb2bellow;
	map<string, string> uwbCntCh2pid;
	map<string, string> uwbCnt2cntSn;
	map<string, string> uwbCnt2type;

	map<string, string> bellow2recep1;
	map<string, string> bellow2recep2;
	map<string, string> bellow2recep3;
	map<string, string> bellow2recep4;
	map<string, string> bellow2recep5;
	map<string, string> bellow2recep6;
	map<string, string> bellow2recep7;
	map<string, string> bellow2recep8;
	map<string, string> bellow2gcu;
	map<string, string> bellow2abc;
	map<string, string> bellow2hvsa;
	map<string, string> bellow2hvsb;
	map<string, string> bellow2mac;

	map<string, string> cnt2pmt;
};

int main(void) {
	ifstream file_pid2mid("./PID2MID");
	ifstream file_pid2type("./PID2TYPE");
	ifstream file_uwb2pos("./UWB2UWBPOS");
	ifstream file_uwb2room("./UWB2ROOM");
	ifstream file_uwb2bellow("./UWB2BELLOW");
	ifstream file_uwbCntCh2pid("./UWBCNTCH2PID");
	ifstream file_uwbCnt2lenSn("./UWBCNT2LENGTHCNTSN");
	ifstream file_bellow2gcuAbcHvs("./BELLOW2GCUABCHVS");
	ifstream file_bellow2recep("./BELLOW2RECEPTACLES");
	ifstream file_cnt2pmt("./CNTCH2PMT");

	relation allMatch;
	vector<string> uwbIds;

	int elecRoom;
	int elecRow;
	int elecRack;
	int elecDevice;
	int uwbPos;
	int cntId;
	int chId;
	int length;

	int ifExist;

	string uwbId;
	string bellowSn;
	string recepSn1;
	string recepSn2;
	string recepSn3;
	string recepSn4;
	string recepSn5;
	string recepSn6;
	string recepSn7;
	string recepSn8;
	string gcuSn;
	string abcSn;
	string hvsaSn;
	string hvsbSn;
	string mac;
	string cntSn;
	string pmtSn;
	string pmtPid;
	string pmtMid;
	string pmtCableLength;
	string pmtCableType;

	clog << "Loading PID2MID" << endl;
	while (file_pid2mid.good()) {
		file_pid2mid >> pmtPid >> pmtMid;
		if (!file_pid2mid.good())
			break;
		allMatch.pid2mid[pmtPid] = pmtMid;
	}
	file_pid2mid.close();

	clog << "Loading PID2TYPE" << endl;
	while (file_pid2type.good()) {
		file_pid2type >> pmtPid >> pmtCableType;
		if (!file_pid2type.good())
			break;
		allMatch.pidAllowed[pmtPid] = 1;
		allMatch.pid2type[pmtPid] = pmtCableType;
	}
	file_pid2type.close();

	clog << "Loading UWB2UWBPOS" << endl;
	while (file_uwb2pos.good()) {
		file_uwb2pos >> uwbId >> uwbPos;
		if (!file_uwb2pos.good())
			break;
		uwbIds.push_back(uwbId);
		allMatch.uwb2uwbPos[uwbId] = uwbPos;
	}
	file_uwb2pos.close();

	clog << "Loading UWB2ROOM" << endl;
	while (file_uwb2room.good()) {
		file_uwb2room >> uwbId
					  >> elecRoom
					  >> elecRow
					  >> elecRack
					  >> elecDevice;
		if (!file_uwb2room.good())
			break;
		allMatch.uwb2room[uwbId] = elecRoom;
		allMatch.uwb2row[uwbId] = elecRow;
		allMatch.uwb2rack[uwbId] = elecRack;
		allMatch.uwb2device[uwbId] = elecDevice;
	}
	file_uwb2room.close();

	clog << "Loading UWB2BELLOW" << endl;
	while (file_uwb2bellow.good()) {
		file_uwb2bellow >> uwbId >> ifExist;
		if (!file_uwb2bellow.good())
			break;
		if (ifExist) {
			file_uwb2bellow >> bellowSn;
			allMatch.uwb2bellow[uwbId] = bellowSn;
		}
	}
	file_uwb2bellow.close();

	clog << "Loading UWBCNTCH2PID" << endl;
	while (file_uwbCntCh2pid.good()) {
		file_uwbCntCh2pid >> uwbId
						  >> cntId
						  >> chId
						  >> pmtPid;
		if (!file_uwbCntCh2pid.good())
			break;
		string tmp = uwbId;
		tmp = tmp + "-"
				  + to_string(cntId)
				  + "-"
				  + to_string(chId);
		allMatch.uwbCntCh2pid[tmp] = pmtPid;
	}
	file_uwbCntCh2pid.close();

	clog << "Loading UWBCNT2LENGTHCNTSN" << endl;
	while (file_uwbCnt2lenSn.good()) {
		file_uwbCnt2lenSn >> uwbId
						  >> cntId
						  >> length
						  >> ifExist;
		// clog << uwbId << "\t" << cntId << endl;
		if (!file_uwbCnt2lenSn.good())
			break;
		string tmp = uwbId;
		tmp = tmp + "-" + to_string(cntId);
		allMatch.uwbCnt2length[tmp] = length;
		if (ifExist) {
			file_uwbCnt2lenSn >> cntSn;
			allMatch.uwbCnt2cntSn[tmp] = cntSn;
			// clog << allMatch.uwbCnt2cntSn[tmp] << endl;
		}
	}
	file_uwbCnt2lenSn.close();

	clog << "Loading BELLOW2GCUABCHVS" << endl;
	while (file_bellow2gcuAbcHvs.good()) {
		file_bellow2gcuAbcHvs >> bellowSn
							  >> abcSn
							  >> ifExist;
		// clog << bellowSn << endl;
		if (!file_bellow2gcuAbcHvs.good())
			break;
		allMatch.bellow2abc[bellowSn] = abcSn;
		// clog << "==================== " << bellowSn << " ====================" << endl;
		if (ifExist) {
			file_bellow2gcuAbcHvs >> hvsaSn;
			allMatch.bellow2hvsa[bellowSn] = hvsaSn;
			// clog << allMatch.bellow2hvsa[bellowSn] << endl;
		}
		file_bellow2gcuAbcHvs >> ifExist;
		if (ifExist) {
			file_bellow2gcuAbcHvs >> hvsbSn;
			allMatch.bellow2hvsb[bellowSn] = hvsbSn;
			// clog << allMatch.bellow2hvsb[bellowSn] << endl;
		}
		file_bellow2gcuAbcHvs >> gcuSn >> ifExist;
		allMatch.bellow2gcu[bellowSn] = gcuSn;
		if (ifExist) {
			file_bellow2gcuAbcHvs >> mac;
			allMatch.bellow2mac[bellowSn] = mac;
			// clog << allMatch.bellow2mac[bellowSn] << endl;
		}
	}
	file_bellow2gcuAbcHvs.close();

	clog << "Loading BELLOW2RECEPTACLES" << endl;
	while (file_bellow2recep.good()) {
		string tmp0, tmp1, tmp2, tmp3;
		file_bellow2recep >> bellowSn
						  >> cntId
						  >> tmp0
						  >> tmp1
						  >> tmp2
						  >> tmp3;
		recepSn1 = tmp0 + tmp1 + tmp2 + tmp3;

		file_bellow2recep >> bellowSn
						  >> cntId
						  >> tmp0
						  >> tmp1
						  >> tmp2
						  >> tmp3;
		recepSn2 = tmp0 + tmp1 + tmp2 + tmp3;

		file_bellow2recep >> bellowSn
						  >> cntId
						  >> tmp0
						  >> tmp1
						  >> tmp2
						  >> tmp3;
		recepSn3 = tmp0 + tmp1 + tmp2 + tmp3;

		file_bellow2recep >> bellowSn
						  >> cntId
						  >> tmp0
						  >> tmp1
						  >> tmp2
						  >> tmp3;
		recepSn4 = tmp0 + tmp1 + tmp2 + tmp3;

		file_bellow2recep >> bellowSn
						  >> cntId
						  >> tmp0
						  >> tmp1
						  >> tmp2
						  >> tmp3;
		recepSn5 = tmp0 + tmp1 + tmp2 + tmp3;

		file_bellow2recep >> bellowSn
						  >> cntId
						  >> tmp0
						  >> tmp1
						  >> tmp2
						  >> tmp3;
		recepSn6 = tmp0 + tmp1 + tmp2 + tmp3;

		file_bellow2recep >> bellowSn
						  >> cntId
						  >> tmp0
						  >> tmp1
						  >> tmp2
						  >> tmp3;
		recepSn7 = tmp0 + tmp1 + tmp2 + tmp3;

		file_bellow2recep >> bellowSn
						  >> cntId
						  >> tmp0
						  >> tmp1
						  >> tmp2
						  >> tmp3;
		recepSn8 = tmp0 + tmp1 + tmp2 + tmp3;

		allMatch.bellow2recep1[bellowSn] = recepSn1;
		allMatch.bellow2recep2[bellowSn] = recepSn2;
		allMatch.bellow2recep3[bellowSn] = recepSn3;
		allMatch.bellow2recep4[bellowSn] = recepSn4;
		allMatch.bellow2recep5[bellowSn] = recepSn5;
		allMatch.bellow2recep6[bellowSn] = recepSn6;
		allMatch.bellow2recep7[bellowSn] = recepSn7;
		allMatch.bellow2recep8[bellowSn] = recepSn8;
	}
	file_bellow2recep.close();

	clog << "Loading CNTCH2PMT" << endl;
	while (file_cnt2pmt.good()) {
		file_cnt2pmt >> cntSn >> chId >> pmtSn;
		if (!file_cnt2pmt.good())
			break;
		string tmp = cntSn + "-" + to_string(chId);
		allMatch.cnt2pmt[tmp] = pmtSn;
	}
	file_cnt2pmt.close();

	ofstream outFile("./allRelationships");
	for (string& uid : uwbIds) {
		bellowSn = allMatch.uwb2bellow[uid];
		for (int i = 1; i <= 8; i ++) {
			string uwbCnt = uid + "-" + to_string(i);
			cntSn = allMatch.uwbCnt2cntSn[uwbCnt];
			for (int j = 1; j <= 16; j ++) {
				string uwbCntCh = uwbCnt + "-" + to_string(j);
				string cntSnCh = cntSn + "-" + to_string(j);
				outFile << allMatch.uwb2room[uid] << "\t"
						<< allMatch.uwb2row[uid] << "\t"
						<< allMatch.uwb2rack[uid] << "\t"
						<< allMatch.uwb2device[uid] << "\t"
						<< "" << "\t" // left for elec_channel
						<< uid << "\t"
						<< allMatch.uwb2uwbPos[uid] << "\t"
						<< bellowSn << "\t";
				switch (i) {
					case 1:
						outFile << allMatch.bellow2recep1[bellowSn] << "\t";
						break;
					case 2:
						outFile << allMatch.bellow2recep2[bellowSn] << "\t";
						break;
					case 3:
						outFile << allMatch.bellow2recep3[bellowSn] << "\t";
						break;
					case 4:
						outFile << allMatch.bellow2recep4[bellowSn] << "\t";
						break;
					case 5:
						outFile << allMatch.bellow2recep5[bellowSn] << "\t";
						break;
					case 6:
						outFile << allMatch.bellow2recep6[bellowSn] << "\t";
						break;
					case 7:
						outFile << allMatch.bellow2recep7[bellowSn] << "\t";
						break;
					case 8:
						outFile << allMatch.bellow2recep8[bellowSn] << "\t";
						break;
				}
				outFile << allMatch.bellow2gcu[bellowSn] << "\t"
						<< allMatch.bellow2abc[bellowSn] << "\t"
						<< allMatch.bellow2hvsa[bellowSn] << "\t"
						<< allMatch.bellow2hvsb[bellowSn] << "\t"
						<< allMatch.bellow2mac[bellowSn] << "\t"
						<< "" << "\t"
						<< i << "\t"
						<< allMatch.uwbCnt2cntSn[uwbCnt] << "\t"
						<< j << "\t"
						<< allMatch.cnt2pmt[cntSnCh] << "\t"
						<< allMatch.uwbCntCh2pid[uwbCntCh] << "\t"
						<< allMatch.pid2mid[allMatch.uwbCntCh2pid[uwbCntCh]] << "\t"
						<< allMatch.uwbCnt2length[uwbCnt] << "\t"
						<< allMatch.pid2type[allMatch.uwbCntCh2pid[uwbCntCh]] << endl;
			}
		}
	}
	outFile.close();

	return 1;
}
