#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

const unsigned MOV   = 0;
const unsigned LOAD  = 1;
const unsigned SAVE  = 2;
const unsigned ADD   = 3;
const unsigned INC   = 4;
const unsigned SUB   = 5;
const unsigned MUL   = 6;
const unsigned DIV   = 7;
const unsigned AND   = 8;
const unsigned OR    = 9;
const unsigned JMP   = 0xA;
const unsigned JCXZ  = 0xB;
const unsigned INT   = 0xC;
const unsigned NOP   = 0xD;

vector<string> instruct_repr = {
	"MOV",
	"LOAD",
	"SAVE",
	"ADD",
	"INC",
	"SUB",
	"MUL",
	"DIV",
	"AND",
	"OR",
	"JMP",
	"JCXZ",
	"INT",
	"NOP"
};