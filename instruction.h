#ifndef INSTRUCT_H
#define INSTRUCT_H 1
#include <vector>
#include <string>

using namespace std;

const unsigned I_DATA_FLAG = 0x8000; // a.k.a. the highest bit of a 16-bit word

const unsigned NOP   = 0;
const unsigned MOV   = 1;
const unsigned LOAD  = 2;
const unsigned SAVE  = 3;
const unsigned ADD   = 4;
const unsigned SUB   = 5;
const unsigned MUL   = 6;
const unsigned DIV   = 7;
const unsigned NOT   = 8;
const unsigned AND   = 9;
const unsigned OR    = 0xA;
const unsigned XOR   = 0xB;
const unsigned JMP   = 0xC;
const unsigned JCNXZ  = 0xD;
const unsigned PUSH  = 0xE;
const unsigned POP   = 0xF;
const unsigned INT   = 0x10;

vector<string> instruct_repr = {
	"NOP",
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
	"JCNXZ",
	"PUSH",
	"POP",
	"INT",
};
#endif
