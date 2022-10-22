#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
	string input(argv[1]);
	size_t len = input.length();
	int result = 0;
	for (int i = 0; i < len; i ++) {
		int tmp = input[i] - 48;
		result *= 4;
		result += tmp;
	}
	cout << result << endl;
	return 1;
}
