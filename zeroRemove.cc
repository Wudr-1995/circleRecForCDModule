#include <iostream>
#include <fstream>

using namespace std;

int main(void) {
	ifstream inFile("./stat");
	ofstream outFile("./finalStat");
	int sc, lc, code0, code1;
	string tmp;
	while (inFile.good()) {
		inFile >> tmp >> sc >> lc >> code0 >> code1;
		if (!inFile.good())
			break;
		if (!sc && !lc)
			continue;
		outFile << tmp << "\t"
				<< sc << "\t"
				<< lc << "\t"
				<< code0 << "\t"
				<< code1 << endl;
	}
	inFile.close();
	outFile.close();
	return 1;
}
