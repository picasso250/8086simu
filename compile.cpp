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


map<string,unsigned> instruct_table = {
	{"MOV",   MOV},
	{"ADD",   ADD},
	{"INC",   INC},
	{"SUB",   SUB},
	{"MUL",   MUL},
	{"DIV",   DIV},
	{"AND",   AND},
	{ "OR",    OR},
	{"JMP",   JMP},
	{"JCXZ", JCXZ},
	{"INT",   INT},
	{"NOP",   NOP}
};
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
			break;
		}
		// inst is instruction
		printf("i:%s(%d)\n", inst, strlen(inst));
		unsigned basic_instr = instruct_table[inst];
		printf("basic_instr: %x\n", basic_instr);

		// read param1
		i = read_word(buf, i, param1);
		printf("A:%s\n", param1);
		if (*(buf+i)) {
			i = read_word(buf, i, param2);
			printf("B:%s\n", param2);
		}
		unsigned modifier = 0;
		char *p1 = param1;
		char *p2 = param2;
		int p1i = 0, p2i = 0, pi = 0;
		switch (basic_instr) {
			case MOV:
				if (param1[0] == '[')
				{
					modifier = 1;
					p1++;
					p1[strlen(p1)-1] = 0;
				}
				p1i = reg_table[p1];
				printf("%s => %x\n", p1, p1i);
				if (p2[0] == '[')
				{
					modifier = 2;
					p2++;
					p2[strlen(p2)-1] = 0;
					p2i = reg_table[p2];
				} else
				if (is_int(p2)) {
					modifier = 3;
					p2i = atoi(p2);
				} else {
					p2i = reg_table[p2];
					printf("%s => %x\n", p2, p2i);
				}
				printf("p1:%x,p2:%x\n", p1i, p2i);
				pi = (p1i << 4) | p2i;
				break;
			case ADD:
			case SUB:
				p1i = reg_table[p1];
				if (is_int(p2)) {
					modifier = 1;
					p2i = atoi(p2);
				} else {
					p2i = reg_table[p2];
				}
				pi = (p1i << 4) | p2i;
				break;
			case INC:
				p1i = reg_table[p1];
				pi = (p1i << 4) | p2i;
				break;
			case MUL:
			case DIV:
			case AND:
			case OR:
				p1i = reg_table[p1];
				p2i = reg_table[p2];
				pi = (p1i << 4) | p2i;
				break;
			case JCXZ:
			case JMP:
				if (is_int(p1))
				{
					modifier = 1;
					pi = atoi(p1);
					printf("'%s' => %x\n", p1, pi);
				} else {
					p1i = reg_table[p1];
					pi = (p1i << 4) | p2i;
				}
				break;
			case INT:
				pi = atoi(p1);
				break;
		}
		int in = (basic_instr << 4) | modifier;
		printf("## %x|%x\n", in, pi&0xFF);
		ofs.put((char)in);
		ofs.put((char)(pi&0xFF));
	}

	return 0;
}