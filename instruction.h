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
const unsigned CMP   = 0x11;
const unsigned JE    = 0x12;
const unsigned JNE   = 0x13;
const unsigned JG    = 0x14;
const unsigned JGE   = 0x15;
const unsigned JL    = 0x16;
const unsigned JLE   = 0x17;

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
	"CMP",
	"JE",
	"JNE",
	"JG",
	"JGE",
	"JL",
	"JLE",
};
#endif
