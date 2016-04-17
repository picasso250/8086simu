#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>

using namespace std;

const unsigned AX  = 0x0;
const unsigned BX  = 0x1;
const unsigned CX  = 0x2;
const unsigned DX  = 0x3;
const unsigned SI  = 0x4;
const unsigned DI  = 0x5;
const unsigned SP  = 0x6;
const unsigned BP  = 0x7;
const unsigned IP  = 0x8;
const unsigned CS  = 0x9;
const unsigned SS  = 0xA;
const unsigned DS  = 0xB;
const unsigned ES  = 0xC;
const unsigned PSW = 0xD;

vector<string> reg_repr = {
	 "AX",
	 "BX",
	 "CX",
	 "DX",
	 "SI",
	 "DI",
	 "SP",
	 "BP",
	 "IP",
	 "CS",
	 "SS",
	 "DS",
	 "ES",
	"PSW"
};
