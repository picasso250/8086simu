#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cassert>

#include "../instruction.h"
#include "../register.h"
#include "../parse_ins.h"

using namespace std;

#define TEST_MODE 1

const unsigned memory_size = 64*1024; // because 64K is enough for every thing

int regs[14];
vector<char> memory(memory_size);
bool runing = true;

#include "../cpu_run.h"

void run_test_file(const char * file_name)
{
    vector<string> lines = {
#include ".test.vector.h"
        "INT 0",
    };
    int i = 0;
    int code = 256;
    for (auto line : lines) {
        unsigned a, b;
        cout<<line<<endl;
        if (line.find(";assert") != string::npos) {
            line = "INT "+to_string(code++);
        } else if (line[0] == ';') {
            cout<<"skip"<<endl;
            continue;
        }
        auto v = parse_ins(line);
        a = v[0];
        b = v[1];
        store(i*2,   a);
        store(i*2+1, b);
        i++;
    }

    regs[IP] = 0;
    regs[CS] = 0;
    cpu_run();
}

int main(int argc, char const *argv[])
{
    static_assert(sizeof(char) == 1, "char is 8 bit");
    static_assert(sizeof(int)  >= 2, "int bigger than 16 bit");
    static_assert((int)(char)0xFF == -1, "char to int");
    run_test_file(argv[1]);
    return 0;
}
