#ifndef PARSE_INS_H
#define PARSE_INS_H 1

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

using namespace std;

map<string,unsigned> instruct_table;
map<string,unsigned> reg_table = {
	{ "AX",  AX},
	{ "BX",  BX},
	{ "CX",  CX},
	{ "DX",  DX},
	{ "SI",  SI},
	{ "DI",  DI},
	{ "SP",  SP},
	{ "BP",  BP},
	{ "IP",  IP},
	{ "CS",  CS},
	{ "SS",  SS},
	{ "DS",  DS},
	{ "ES",  ES},
	{"PSW", PSW}
};

vector<string> sep2by(string line, char sep)
{
	int i = 0;
	while (line[i] == ' ' || line[i] == ',') i++;
	auto j = line.find(sep, i);
	if (j == string::npos) return {line.substr(i), ""};
	return {line.substr(i, j), line.substr(j+1)};
}
bool is_int(string str)
{
	return str[0] == '+' ||
		str[0] == '-' ||
		isdigit(str[0]);
}
string trimbarackets(string word)
{
	return word.substr(1, word.size()-1);
}
vector<unsigned> parse_ins(string line)
{
	if (instruct_table.size() == 0) {
		for (int i = 0; i < instruct_repr.size(); ++i) {
			instruct_table[instruct_repr[i]] = i;
		}
	}
	if (line.size() == 0) {
		return {0,0};
	}
	auto v = sep2by(line, ' ');
	string inst = v[0];
	// inst is instruction
	// printf("i:%s(%d)\n", inst, strlen(inst));
	unsigned basic_instr = instruct_table[inst];
	// printf("basic_instr: %x\n", basic_instr);

	// read params
	v = sep2by(v[1], ',');
	string p1 = v[0];
	string p2 = v[1];
	cout<<"p1:"<<p1<<", p2:"<<p2<<"("<<p2.size()<<")"<<endl;
	int p1i = 0, p2i = 0;
	int idata = 0;
	bool is_idata = false;
	switch (basic_instr) {
		case LOAD:
		case SAVE:
			if (p1[0] == '[') {
				p1 = trimbarackets(p1);
			}
			p1i = reg_table[p1];
			printf("%s => %x\n", p1.c_str(), p1i);
			if (p2[0] == '[') {
				p2 = trimbarackets(p2);
				p2i = reg_table[p2];
			} else {
				p2i = reg_table[p2];
				// printf("%s => %x\n", p2.c_str(), p2i);
			}
			printf("p1:%x,p2:%x\n", p1i, p2i);
			break;
		case MOV:
		case ADD:
		case SUB:
		case MUL:
		case DIV:
			p1i = reg_table[p1];
			if (is_int(p2)) {
				is_idata = true;
				idata = stoi(p2);
			} else {
				p2i = reg_table[p2];
				// cout<<"'"<<p2<<"' => "<<p2i<<endl;
			}
			break;
		case NOT:
			p1i = reg_table[p1];
			// pi = (p1i << 4) | p2i;
			break;
		case AND:
		case OR:
		case XOR:
			p1i = reg_table[p1];
			if (is_int(p2)) {
				is_idata = true;
				idata = stoi(p2);
			} else {
				p2i = reg_table[p2];
			}
			// pi = (p1i << 4) | p2i;
			break;
		case JCXZ:
		case JMP:
		case INT:
			if (is_int(p1))
			{
				is_idata = true;
				idata = stoi(p1);
				// printf("'%s' => %x\n", p1, pi);
			} else {
				p1i = reg_table[p1];
				// pi = (p1i << 4) | p2i;
			}
			break;
	}
	unsigned in, pi;
	if (is_idata) {
		in = ((basic_instr & 0x8) << 8) | p1i;
		pi = idata;
	} else {
		in = basic_instr << 8;
		// cout<<":"<<p1i<<":"<<p2i<<"=="<<((p1i << 8) | p2i)<<endl;
		pi = (p1i << 8) | p2i;
	}
	printf("## %04X|%04X\n", in, pi&0xFFFF);

	return {in & 0xFFFF, (pi&0xFFFF)};
}
#endif
