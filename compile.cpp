#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>

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

const unsigned memory_size = 64*1024*1024; // 64M

bool runing = true;

int read_word(char * buf, int &i, char *word)
{
	while (buf[i] == ' ' || buf[i] == ',') i++;
	int j;
	for (j = 0; buf[i] &&
			buf[i] != ',' && buf[i] != ' ' &&
			buf[i] != '\n' && buf[i] != '\r'; ++i,++j)
	{
		// printf("copy %c\n", buf[i]);
		word[j] = buf[i];
	}
	word[j] = 0;
	// printf("word:%s\n", word);
	return i;
}
bool is_int(char *str)
{
	return str[0] == '+' ||
		str[0] == '-' ||
		isdigit(str[0]);
}
int main(int argc, char const *argv[])
{
	for (int i = 0; i < instruct_repr.size(); ++i)
	{
		instruct_table[instruct_repr[i]] = i;
	}
	char buf[222];
	ifstream ifs(argv[1]);
	ofstream ofs(argv[2]);
	char inst[22];
	char param1[22];
	char param2[22];
	while (ifs.good()) {
		ifs.getline(buf, 222);
		printf("----------\n%s\n", buf);
		if (buf[0] == ';')
		{
			continue;
		}
		int i = 0;
		i = read_word(buf, i, inst);
		if (strlen(inst) == 0)
		{
			continue;
		}
		// inst is instruction
		// printf("i:%s(%d)\n", inst, strlen(inst));
		unsigned basic_instr = instruct_table[inst];
		// printf("basic_instr: %x\n", basic_instr);

		// read param1
		i = read_word(buf, i, param1);
		// printf("A:%s\n", param1);
		if (*(buf+i)) {
			i = read_word(buf, i, param2);
			// printf("B:%s\n", param2);
		}
		char *p1 = param1;
		char *p2 = param2;
		int p1i = 0, p2i = 0;
		int idata = 0;
		bool is_idata = false;
		switch (basic_instr) {
			case LOAD:
			case SAVE:
				if (param1[0] == '[')
				{
					p1++;
					p1[strlen(p1)-1] = 0;
				}
				p1i = reg_table[p1];
				printf("%s => %x\n", p1, p1i);
				if (p2[0] == '[')
				{
					p2++;
					p2[strlen(p2)-1] = 0;
					p2i = reg_table[p2];
				} else {
					p2i = reg_table[p2];
					printf("%s => %x\n", p2, p2i);
				}
				printf("p1:%x,p2:%x\n", p1i, p2i);
				break;
			case MOV:
			case ADD:
			case SUB:
				p1i = reg_table[p1];
				if (is_int(p2)) {
					is_idata = true;
					idata = atoi(p2);
				} else {
					p2i = reg_table[p2];
				}
				break;
			case INC:
				p1i = reg_table[p1];
				// pi = (p1i << 4) | p2i;
				break;
			case MUL:
			case DIV:
			case AND:
			case OR:
				p1i = reg_table[p1];
				p2i = reg_table[p2];
				// pi = (p1i << 4) | p2i;
				break;
			case JCXZ:
			case JMP:
			case INT:
				if (is_int(p1))
				{
					is_idata = true;
					idata = atoi(p1);
					// printf("'%s' => %x\n", p1, pi);
				} else {
					p1i = reg_table[p1];
					// pi = (p1i << 4) | p2i;
				}
				break;
		}
		unsigned in, pi;
		if (is_idata)
		{
			in = (basic_instr << 4) | p1i;
			pi = idata & 0x80 ? idata - 0xFF - 1 : idata;
		} else {
			in = 0xF0 | basic_instr;
			pi = (p1i << 4) | p2i;
		}
		printf("## %02X|%02X\n", in, pi&0xFF);
		ofs.put((char)(in&0xFF));
		ofs.put((char)(pi&0xFF));
	}

	return 0;
}