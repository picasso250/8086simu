#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <tuple>

#include "instruction.h"
#include "register.h"
#include "parse_ins.h"

using namespace std;

int main(int argc, char const *argv[])
{
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	string line;
	while (ifs.good()) {
		getline(ifs, line);
		printf("----------\n%s\n", line);
		if (line[0] == ';') {
			continue;
		}
		int in, pi;
		auto v = parse_ins(line);
		ofs.put((char)(v[0]&0xFF));
		ofs.put((char)(v[1]&0xFF));
	}

	return 0;
}
