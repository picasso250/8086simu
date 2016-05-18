#ifndef INSTRUCT_H
#define INSTRUCT_H 1
#include <vector>
#include <string>

using namespace std;

const unsigned MOV   = 0;
const unsigned LOAD  = 1;
const unsigned SAVE  = 2;
const unsigned ADD   = 3;
const unsigned SUB   = 4;
const unsigned MUL   = 5;
const unsigned DIV   = 6;
const unsigned NOT   = 7;
const unsigned AND   = 8;
const unsigned OR    = 9;
const unsigned XOR   = 0xA;
const unsigned JMP   = 0xB;
const unsigned JCXZ  = 0xC;
const unsigned INT   = 0xD;
const unsigned NOP   = 0xE;

vector<string> instruct_repr = {
	"MOV",
	"LOAD",
	"SAVE",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"NOT",
	"AND",
	"OR",
	"XOR",
	"JMP",
	"JCXZ",
	"INT",
	"NOP"
};
#endif
