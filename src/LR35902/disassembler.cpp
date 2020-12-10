#include "lr35902.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
template <typename T>

string int_to_hex(T i, string prefix)
{
    std::stringstream stream;
    stream << prefix
           << std::setfill('0') << std::setw(sizeof(T) * 2)
           << std::hex << (0xFF & i);
    return stream.str();
}

string operand_to_string(string op, uint8_t* data, size_t* i)
{
    string buf = "";

    if (!op.compare("d8")) {
        *i = *i + 1;
        buf += int_to_hex(data[*i], "$");
        return buf;
    } else if (!op.compare("r8")) {
        *i = *i + 1;
        if ((data[*i] & (0x80))) {
            buf += int_to_hex((uint8_t)((data[*i] ^ 0xFF) + 1), "-$");
        } else
            buf += int_to_hex(data[*i], "$");
        return buf;
    } else if (!op.compare("d16")) {
        *i = *i + 1;
        buf = int_to_hex(data[*i], "");
        *i = *i + 1;
        buf = int_to_hex(data[*i], "$") + buf;
        return buf;
    } else if (!op.compare("a16")) {
        *i = *i + 1;
        buf = int_to_hex(data[*i], "");
        *i = *i + 1;
        buf = int_to_hex(data[*i], "$") + buf;
        return buf;
    } else if (!op.compare("(a8)")) {
        *i = *i + 1;
        return op;
    } else if (!op.compare("(a16)")) {
        *i = *i + 2;
        return op;
    } else {
        return op;
    }
}

string lr35902::instruct_to_string(uint8_t instcode, uint8_t* data, size_t* i)
{
    instruction inst = instlist[instcode];
    string res = "";

    res += int_to_hex(instcode, "0x") + "\t" + inst.name + "\t";

    if (inst.op1.compare("")) {
        res += operand_to_string(inst.op1, data, i);
    }
    if (inst.op2.compare("")) {
        res += " , " + operand_to_string(inst.op2, data, i);
    }
    if (!inst.name.compare("PREFIX")) {
        *i = *i + 1;
    }
    return res;
}

void hexdump(uint8_t* data, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0)
            cout << "\n";
        cout << int_to_hex(data[i], "") << " ";
    }
    cout << "\n\n";
}

void lr35902::disassemble(rom_t* rom)
{
    uint8_t* data = rom->data;
    size_t romsize = rom->size;
    vector<string> res;
    string str;

    hexdump(data, romsize);

    for (size_t i = 0; i < romsize; i++) {
        str += instruct_to_string(data[i], data, &i);
        res.push_back(str);
    }

    for (auto string : res) {
        cout << string << '\n';
    }
}
