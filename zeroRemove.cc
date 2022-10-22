#include <iostream>
#include <fstream>

using namespace std;

int main(void) {
	ifstream inFile("./stat");
	ofstream outFile("./finalStat");
	int sc, lc, code;
	while (inFile.good()) {
		inFile >> sc >> lc >> code;
		if (!inFile.good())
			break;
		if (!sc && !lc)
			continue;
		outFile << sc << "\t"
				<< lc << "\t"
				<< code << endl;
	}
	inFile.close();
	outFile.close();
	return 1;
}
