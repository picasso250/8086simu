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
void do_ins(instruct &ins)
{
    char in = ins.ins;
    char data = ins.data;
    // printf("instruction: %X %X\n", in, data&0xFF);
    unsigned basic_instr = (in >> 4) & 0xF;
    unsigned modifier    = in & 0xF;
    // printf("modifier %X\n", modifier);
    unsigned reg1 = (data >> 4) & 0xF;
    unsigned reg2 = data & 0xF;
    // printf("reg1: %X, reg1 %X\n", reg1, reg2);
    // char cc; cin>>cc;
    switch (basic_instr) {
    case MOV:
        unsigned pos;
        printf("MOV ");
        switch (modifier) {
            case 0:
                // reg1 = reg2
                regs[reg1] = regs[reg2];
                printf("%s,%s\n", reg_repr[reg1].c_str(), reg_repr[reg2].c_str());
                break;
            case 1:
                // store
                pos = get_pos(DS, reg1);
                store(pos, regs[reg2]);
                printf("[%s],%s\n", reg_repr[reg1].c_str(), reg_repr[reg2].c_str());
                break;
            case 2:
                // load
                pos = get_pos(DS, reg2);
                regs[reg1] = load(pos);
                printf("%s,[%s]\n", reg_repr[reg1].c_str(), reg_repr[reg2].c_str());
                break;
            case 3:
                // instant data
                regs[reg1] = reg2;
                printf("%s,%d\n", reg_repr[reg1].c_str(), reg2);
                break;
            default:
                throw "unknown modifier";
                break;
        }
        regs[IP] += 1;
        break;
    case ADD:
        cout<<"ADD"<<endl;
        regs[reg1] = regs[reg1] + (modifier ? reg2 : regs[reg2]);
        regs[IP] += 1;
        break;
    case INC:
        cout<<"INC"<<endl;
        regs[reg1] = regs[reg1] + 1;
        regs[IP] += 1;
        break;
    case SUB:
        cout<<"SUB"<<endl;
        regs[reg1] = regs[reg1] - (modifier ? reg2 : regs[reg2]);
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
        switch (modifier) {
            case 0:
                regs[IP] = regs[reg1];
                printf("[%s]\n", reg_repr[reg1].c_str());
                break;
            case 1:
                regs[IP] += (reg2 & 0x8 ? reg2 - 0xF -1 : reg2);
                printf("%d\n", (reg2 & 0x8) ? reg2 - 0xF -1 : reg2);
                break;
        }
        char ccc ; cin>>ccc;
        break;
    case INT:
        switch (regs[(int)data]) {
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
        // cin >> c;
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