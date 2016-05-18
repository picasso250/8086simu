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

void put16(ofstream &ofs, unsigned i)
{
	char a, b;
	a = i>>8;
	// printf("store (%4X) %4X\n", i, a);
	ofs.put((char)a);
	ofs.put((char)i);
}
int main(int argc, char const *argv[])
{
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	string line;
	while (ifs.good()) {
		getline(ifs, line);
		printf("----------\n%s\n", line.c_str());
		if (line[0] == ';') {
			continue;
		}
		int in, pi;
		auto v = parse_ins(line);
		printf("@  %4X|%4X\n", v[0], v[1]);
		put16(ofs, v[0]);
		put16(ofs, v[1]);
	}

	return 0;
}
