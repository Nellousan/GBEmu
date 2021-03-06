#include "lr35902.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
std::string int_to_hex(T i, std::string prefix)
{
    std::stringstream stream;
    stream << prefix
           << std::setfill('0') << std::setw(sizeof(T) * 2)
           << std::hex << (0xFFFF & i);
    return stream.str();
}

std::string operand_to_string(std::string op, uint8_t* data, uint16_t* i)
{
    std::string buf = "";

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
        buf = "(" + int_to_hex(data[*i], "$") + ")";
        return buf;
    } else if (!op.compare("(a16)")) {
        *i = *i + 2;
        return op;
    } else {
        return op;
    }
}

std::string lr35902::instruct_to_string(uint8_t instcode, uint8_t* data, uint16_t* i)
{
    instruction inst = instlist[instcode];
    std::string res = "";

    if (!inst.name.compare("PREFIX")) {
        *i = *i + 1;
        inst = preflist[data[*i]];
    }

    res += inst.name + "\t";

    if (inst.op1.compare("")) {
        res += operand_to_string(inst.op1, data, i);
    }
    if (inst.op2.compare("")) {
        res += " , " + operand_to_string(inst.op2, data, i);
    }
    return res;
}

void hexdump(uint8_t* data, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        if (i % 16 == 0)
            std::cout << "\n";
        std::cout << int_to_hex(data[i], "") << " ";
    }
    std::cout << "\n\n";
}

std::string lr35902::disassemble10(uint16_t pc) 
{
    std::string str = "";

    for (uint16_t i = pc, j = 0; j < 10; i++, j++) {
        str += int_to_hex(i, "$") + "\t";
        str += instruct_to_string(bus->mem[i], bus->mem, &i) + "\n";
    }
    return str;
}

void lr35902::disassemble(rom_t* rom)
{
    uint8_t* data = rom->data;
    uint16_t romsize = rom->size;
    std::vector<std::string> res;
    std::string str = "";

    hexdump(data, romsize);

    for (uint16_t i = 0; i < romsize; i++) {
        str += instruct_to_string(data[i], data, &i);
        res.push_back(str);
    }

    for (auto string : res) {
        std::cout << string << '\n';
    }
}
