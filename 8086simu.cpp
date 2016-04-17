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
    word()
    {}
    word(int v)
    {
        low = v & 0xF;
        high = v >> 8;
    }
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

int load(unsigned pos) {
    if (pos >= memory_size) {
        throw "read memory out";
    }
    if (pos >= memory.size()) {
        int new_size = pos*2 > memory_size ? memory_size : pos*2;
        memory.resize(new_size);
    }
    int v = 0;
    // printf("loadm (%X) %02X%02X\n", pos, memory[pos*2], memory[pos*2+1]);
    v  = memory[pos*2] << 8;
    v |= memory[pos*2+1] & 0xFF;
    return v;
}
void store_word(unsigned pos, word w) {
    if (pos >= memory_size) {
        throw "write memory out";
    }
    if (pos >= memory.size()) {
        int new_size = pos*2 > memory_size ? memory_size : pos*2;
        memory.resize(new_size);
    }
    memory[pos*2] = w.low;
    memory[pos*2+1] = w.high;
}
void store(unsigned pos, int v) {
    word w(v);
    store_word(pos, w);
}

unsigned get_pos(unsigned seg, unsigned reg)
{
    return (regs[seg] << 1) + regs[reg];
}
void do_ins(unsigned ins)
{
    unsigned i1    = (ins >> 12) & 0xF;
    unsigned i2    = (ins >>  8) & 0xF;
    unsigned reg1  = (ins >>  4) & 0xF;
    unsigned reg2  = (ins >>  0) & 0xF;
    int      idata = (ins >>  8) & 0xFF;
    idata = idata & 0x80 ? idata - 0xFF - 1 : idata; // to signed
    bool is_idata = i1 != 0xF;
    unsigned basic_instr = is_idata ? i1 : i2;
    // printf("instruction: %X %X\n", in, data&0xFF);
    // printf("reg1: %X, reg1 %X\n", reg1, reg2);
    switch (basic_instr) {
    case MOV:
        unsigned pos;
        printf("MOV ");
        if (is_idata)
        {
            // instant data
            regs[reg1] = idata;
            printf("%s,%d\n", reg_repr[reg1].c_str(), idata);
        } else {
            // reg1 = reg2
            regs[reg1] = regs[reg2];
            printf("%s,%s\n", reg_repr[reg1].c_str(), reg_repr[reg2].c_str());
        }
        regs[IP] += 1;
        break;
    case LOAD:
        // load
        pos = get_pos(DS, reg2);
        regs[reg1] = load(pos);
        printf("LOAD %s,[%s]\n", reg_repr[reg1].c_str(), reg_repr[reg2].c_str());
        break;
    case SAVE:
        // store
        pos = get_pos(DS, reg1);
        store(pos, regs[reg2]);
        printf("SAVE [%s],%s\n", reg_repr[reg1].c_str(), reg_repr[reg2].c_str());
        break;
    case ADD:
        cout<<"ADD"<<endl;
        regs[reg1] = regs[reg1] + (is_idata ? idata : regs[reg2]);
        regs[IP] += 1;
        break;
    case INC:
        cout<<"INC"<<endl;
        regs[reg1] = regs[reg1] + 1;
        regs[IP] += 1;
        break;
    case SUB:
        cout<<"SUB"<<endl;
        regs[reg1] = regs[reg1] - (is_idata ? idata : regs[reg2]);
        regs[IP] += 1;
        break;
    case MUL:
        cout<<"MUL"<<endl;
        regs[reg1] = regs[reg1] * regs[reg2];
        regs[IP] += 1;
        break;
    case DIV:
        cout<<"DIV"<<endl;
        regs[AX] = regs[reg1] / regs[reg2];
        regs[DX] = regs[reg1] % regs[reg2];
        regs[IP] += 1;
        break;
    case AND:
        cout<<"AND"<<endl;
        regs[reg1] = regs[reg1] & regs[reg2];
        regs[IP] += 1;
        break;
    case OR:
        cout<<"OR"<<endl;
        regs[reg1] = regs[reg1] | regs[reg2];
        regs[IP] += 1;
        break;
    case JCXZ:
        printf("JCXZ :%X?\n", regs[CX]);
        if (regs[CX] == 0) {
            regs[IP] += 1;
            break;
        }
    case JMP:
        printf("JMP ");
        if (is_idata)
        {
            regs[IP] += idata;
            printf("%d\n", idata);
        } else {
            regs[IP] = regs[reg1];
            printf("[%s]\n", reg_repr[reg1].c_str());
        }
        // char ccc ; cin>>ccc;
        break;
    case INT:
        switch (regs[idata]) {
            case 0:
                runing = false;
                break;
        }
    case NOP:
        regs[IP] += 1;
        break;
    }
}
void cpu_run()
{
    instruct ins;
    char c = 'y';
    while (runing && c == 'y') {
        int w = load(get_pos(CS, IP));
        printf("------------\nI: %04X (%X)\n",
            w&0xFFFF,get_pos(CS, IP));
        cin >> c;
        // printf("%d\n", c);
        ins.ins = w >> 8;
        ins.data = w & 0xFF;
        do_ins(ins);
        for (int i = 0; i < reg_repr.size(); ++i)
        {
            printf("%s:%X ", reg_repr[i].c_str(), regs[i]);
        }
        printf("\n");
    }
}
void run_file(const char * file_name)
{
    ifstream is;
    is.open(file_name, ifstream::in);
    if (is.bad())
    {
        cout<<"not good file: "<<file_name<<cout;
        exit(1);
    }
    char c;
    word w;
    printf("load:\n");
    int i = 0;
    while (is.good()) {
        if (is.get(c))
        {
            if (is.good())
            {
                memory.push_back(c);
                if (i % 2)
                    printf("|%02X|\n",c&0xFF);
                else
                    printf("|%02X",c&0xFF);
            } else {
                break;
            }
        }
        i++;
    };
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