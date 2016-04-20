#include <vector>
#include <string>

using namespace std;

const unsigned MOV   = 0;
const unsigned LOAD  = 1;
const unsigned SAVE  = 2;
const unsigned ADD   = 3;
const unsigned SUB   = 4;
const unsigned NOT   = 5;
const unsigned AND   = 6;
const unsigned OR    = 7;
const unsigned JMP   = 0x8;
const unsigned JCXZ  = 0x9;
const unsigned INT   = 0xA;
const unsigned NOP   = 0xB;

vector<string> instruct_repr = {
	"MOV",
	"LOAD",
	"SAVE",
	"ADD",
	"SUB",
	"NOT",
	"AND",
	"OR",
	"JMP",
	"JCXZ",
	"INT",
	"NOP"
};
