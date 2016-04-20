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

unsigned load(unsigned pos) {
    if (pos >= memory_size)
        throw "read memory out";
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
void store(unsigned pos, int w) {
    if (pos >= memory_size)
        throw "write memory out";
    if (pos >= memory.size()) {
        int new_size = pos*2 > memory_size ? memory_size : pos*2;
        memory.resize(new_size);
    }
    memory[pos*2] = (w >> 8) & 0xFF;
    memory[pos*2+1] = w & 0xFF;
}

unsigned get_pos(unsigned seg, unsigned reg)
{
    return (regs[seg] << 4) + regs[reg];
}
void do_ins(unsigned ins)
{
    unsigned i1    = (ins >> 12) & 0xF;
    unsigned i2    = (ins >>  8) & 0xF;
    unsigned reg1  = (ins >>  4) & 0xF;
    unsigned reg2  = (ins >>  0) & 0xF;
    unsigned uidt  = (ins) & 0xFF;
    // printf("uidt %X\n", uidt);
    int idata = uidt & 0x80 ? uidt - 0xFF - 1 : uidt; // to signed
    // printf("idata %d\n", idata);
    bool is_idata = i1 != 0xF;
    unsigned basic_instr = is_idata ? i1 : i2;
    unsigned reg = i2;
    // printf("instruction: %X %X\n", in, data&0xFF);
    // printf("reg1: %X, reg1 %X\n", reg1, reg2);
    switch (basic_instr) {
    case MOV:
        unsigned pos;
        printf("MOV ");
        if (is_idata)
        {
            // instant data
            regs[reg] = idata;
            printf("%s,%d\n", reg_repr[reg].c_str(), idata);
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
        regs[IP] += 1;
        break;
    case SAVE:
        // store
        pos = get_pos(DS, reg1);
        store(pos, regs[reg2]);
        printf("SAVE [%s],%s\n", reg_repr[reg1].c_str(), reg_repr[reg2].c_str());
        regs[IP] += 1;
        break;
    case ADD:
        cout<<"ADD"<<endl;
        if (is_idata) {
            regs[reg] = regs[reg] + uidt;
        } else {
            regs[reg1] = regs[reg1] + regs[reg2];
        }
        regs[IP] += 1;
        break;
    case SUB:
        cout<<"SUB"<<endl;
        if (is_idata) {
            regs[reg] = regs[reg] - uidt;
        } else {
            regs[reg1] = regs[reg1] - regs[reg2];
        }
        regs[IP] += 1;
        break;
    case NOT:
        cout<<"NOT"<<endl;
        regs[reg1] = ~(regs[reg1]);
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
    unsigned ins;
    char c = 'y';
    while (runing && c == 'y') {
        ins = load(get_pos(CS, IP));
        printf("------------\nI: %04X (%X)\n",
            ins&0xFFFF, get_pos(CS, IP));
        cin >> c;
        // printf("%d\n", c);
        do_ins(ins);
        for (int i = 0; i < reg_repr.size(); ++i)
        {
            printf("%s:%X ", reg_repr[i].c_str(), regs[i]);
            if (i && i % 8 == 0)
            {
                printf("\n");
            }
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
        printf("not good file: %s\n", file_name);
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
