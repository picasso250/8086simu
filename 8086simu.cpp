#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>

#include "instruction.h"
#include "register.h"

using namespace std;

// 16 bit
struct word
{
    char low;
    char high;
    unsigned value()
    {
        return (high << 8) | low;
    }
    word operator=(unsigned v)
    {
        low = v & 0xF;
        high = v >> 8;
    }
    unsigned operator+=(unsigned v)
    {
        v += this->value();
        low = v & 0xF;
        high = v >> 8;
        return v;
    }
};

// 16 bit
struct instruct
{
    char ins;
    char data;
    instruct operator=(word w)
    {
        ins = w.low;
        data = w.high;
        return *this;
    }
};

const unsigned memory_size = 64*1024*1024; // 64M

int regs[14];
vector<char> memory;
bool runing = true;

word load(unsigned pos) {
    if (pos >= memory_size) {
        throw "read memory out";
    }
    if (pos >= memory.size()) {
        int new_size = pos*2 > memory_size ? memory_size : pos*2;
        memory.resize(new_size);
    }
    return memory[pos];
}
void store(unsigned pos, word w) {
    if (pos >= memory_size) {
        throw "write memory out";
    }
    if (pos >= memory.size()) {
        int new_size = pos*2 > memory_size ? memory_size : pos*2;
        memory.resize(new_size);
    }
    memory[pos] = w;
}
unsigned get_pos(unsigned seg, unsigned reg)
{
    return (regs[seg].value() << 1) + regs[reg].value();
}
void do_ins(instruct &ins)
{
    char in = ins.ins;
    char data = ins.data;
    unsigned basic_instr = in >> 4;
    unsigned modifier    = in & 0xF;
    unsigned reg1 = data >> 4;
    unsigned reg2 = data & 0xF;
    switch (basic_instr) {
    case MOV:
        unsigned pos;
        cout<<"MOV"<<endl;
        switch (modifier) {
            case 0:
                // reg1 = reg2
                regs[reg1] = regs[reg2];
                break;
            case 1:
                // store
                pos = get_pos(DS, reg1);
                store(pos, regs[reg2]);
                break;
            case 2:
                // load
                pos = get_pos(DS, reg2);
                regs[reg1] = load(pos);
                break;
            case 3:
                // instant data
                regs[reg1] = reg2;
        }
        regs[IP] += 2;
        break;
    case ADD:
        cout<<"ADD"<<endl;
        regs[reg1] = regs[reg1].value() + modifier ? reg2 : regs[reg2].value();
        regs[IP] += 2;
        break;
    case INC:
        cout<<"INC"<<endl;
        regs[reg1] = regs[reg1].value() + 1;
        regs[IP] += 2;
        break;
    case SUB:
        cout<<"SUB"<<endl;
        regs[reg1] = regs[reg1].value() + modifier ? reg2 : regs[reg2].value();
        regs[IP] += 2;
        break;
    case MUL:
        cout<<"MUL"<<endl;
        regs[reg1] = regs[reg1].value() * regs[reg2].value();
        regs[IP] += 2;
        break;
    case DIV:
        cout<<"DIV"<<endl;
        regs[AX] = regs[reg1].value() / regs[reg2].value();
        regs[DX] = regs[reg1].value() % regs[reg2].value();
        regs[IP] += 2;
        break;
    case AND:
        cout<<"AND"<<endl;
        regs[reg1] = regs[reg1].value() & regs[reg2].value();
        regs[IP] += 2;
        break;
    case OR:
        cout<<"OR"<<endl;
        regs[reg1] = regs[reg1].value() | regs[reg2].value();
        regs[IP] += 2;
        break;
    case JCXZ:
        cout<<"JCXZ"<<endl;
        if (regs[CX].value() != 0) {
            regs[IP] += 2;
            break;
        }
    case JMP:
        cout<<"JMP"<<endl;
        switch (modifier) {
            case 0:
                regs[IP] = regs[reg1];
                break;
            case 1:
                regs[IP] = IP + (int)ins.data;
                break;
        }
        break;
    case INT:
        switch (regs[(int)data].value()) {
            case 0:
                runing = false;
                break;
        }
    case NOP:
        regs[IP] += 2;
        break;
    }
}
void cpu_run()
{
    instruct ins;
    int i = 1;
    while (runing && i) {
        ins = load(get_pos(CS, IP));
        printf("I:%X%X(%X)\n", ins.ins&0xFF,ins.data&0xFF,get_pos(CS, IP));
        cin>>i;
        do_ins(ins);
    }
}
void run_file(const char * file_name)
{
    auto start = begin(memory);
    ifstream is;
    is.open(file_name, ifstream::in);
    if (is.bad())
    {
        cout<<"not good file: "<<file_name<<cout;
        exit(1);
    }
    char c;
    word w;
    while (is.good()) {
        if (is.get(c))
            w.high = c;
        if (is.get(c))
            w.low = c;
        else
            break;
        printf("load |%02X|%02X|\n", w.high&0xFF, w.low&0xFF);
    }
    memory.push_back(w);
    // return;

    regs[IP] = 0;
    regs[CS] = 0;
    cpu_run();
}
int main(int argc, char const *argv[])
{
    static_assert(sizeof(char) == 1, "char is 8 bit");
    static_assert(sizeof(int)  >= 2, "int bigger than 16 bit");
    static_assert((int)(char)0xFF == -1, "char to int");
    run_file(argv[1]);
    return 0;
}