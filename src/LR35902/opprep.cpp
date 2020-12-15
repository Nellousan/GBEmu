#include "lr35902.h"
#include <iostream>

uint8_t *lr35902::getRegister8(std::string name) {
    int vecsize = r8list.size();
    for (int i = 0; i < vecsize; i++)
        if (!name.compare(r8list[i].name))
            return r8list[i].r;
    return nullptr;
}

uint16_t *lr35902::getRegister16(std::string name) {
    int vecsize = r16list.size();
    for (int i = 0; i < vecsize; i++) {
        if (!name.compare(r16list[i].name)) {
            return r16list[i].r;
        }
    }
    return nullptr;
}

bool lr35902::prepareOperand8(std::string opn, uint8_t** op) {
    uint16_t *temp;
    *op = getRegister8(opn);
    if (*op != nullptr)
        return true;
    if (opn[0] == '(') {
        opn.erase(opn.begin());
        opn.erase(opn.end());
        if (opn.find("HL") != std::string::npos) {
            if (opn[opn.size() - 1] == '+') {
                opn.erase(opn.end());
                temp = getRegister16(opn);
                *op = bus->readPtr(*temp);
                *temp = *temp + 1;
            } else if (opn[opn.size() - 1] == '-') {
                opn.erase(opn.end());
                temp = getRegister16(opn);
                *op = bus->readPtr(*temp);
                *temp = *temp - 1;
            } else {
                temp = getRegister16(opn);
                *op = bus->readPtr(*temp);
            }
        } else if (!opn.compare("a16")) {
            temp16_1 = (bus->read(pc + 1) << 8 | bus->read(pc));
            pc += 2;
            *op = bus->readPtr(temp16_1);
        } else if (!opn.compare("a8")) {
            temp8_1 = bus->read(pc + 1);
            pc++;
            *op = bus->readPtr(temp8_1 + 0xFF00);
        }
    } else if (!opn.compare("d8")) {
        temp8_1 = bus->read(pc + 1);
        *op = &temp8_1;
        pc++;
    } else if (!opn.compare("Z")) {
        temp8_1 = getFlag(Z);
        *op = &temp8_1;
    } else if (!opn.compare("NZ")) {
        temp8_1 = ~(getFlag(Z));
        *op = &temp8_1;
    } else if (!opn.compare("C")) {
        temp8_1 = getFlag(C);
        *op = &temp8_1;
    } else if (!opn.compare("NC")) {
        temp8_1 = ~(getFlag(C));
        *op = &temp8_1;
    }
    if (*op == nullptr) {
        return false;
    }
    return true;
}

bool lr35902::prepareOperand16(std::string opn, uint16_t** op) {
    *op = getRegister16(opn);
    if (*op != nullptr)
        return true;
    if (!opn.compare("a16") || !opn.compare("d16")) {
        temp16_1 = (bus->read(pc + 1) << 8 | bus->read(pc));
        pc += 2;
        *op = &temp16_1;
    }
    if (*op == nullptr) {
        return false;
    }
    return true;
}

bool lr35902::isOperand8(std::string name) {
    int vecsize = op8list.size();
    for (int i = 0; i < vecsize; i++) {
        if (!name.compare(op8list[i])) {
            return true;
        }
    }
    return false;
}

bool lr35902::isOperand16(std::string name) {
    int vecsize = op16list.size();
    for (int i = 0; i < vecsize; i++) {
        if (!name.compare(op16list[i])) {
            return true;
        }
    }
    return false;
}

void lr35902::prepareOperands(uint8_t instcode) {
    instruction inst = instlist[instcode];

    std::cout << inst.op1 + "\n" + inst.op2 + "\n";

    if (!inst.op1.compare("")) {
        op8_1 = 0;
        op16_1 = 0;
    } else if (isOperand8(inst.op1))
        prepareOperand8(inst.op1, &op8_1);
    else if (isOperand16(inst.op1))
        prepareOperand16(inst.op1, &op16_1);
    else
        std::cout << "Error in prepare operands: couldn't handle operand: " << inst.op1 << "\n";
    
    if (!inst.op2.compare("")) {
        op8_2 = 0;
        op16_2 = 0;
    } else if (isOperand8(inst.op2))
        prepareOperand8(inst.op2, &op8_2);
    else if (isOperand16(inst.op2))
        prepareOperand16(inst.op2, &op16_2);
    else
        std::cout << "Error in prepare operands: couldn't handle operand: " << inst.op2 << "\n";
}