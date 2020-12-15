#include "lr35902.h"
#include "bus.h"
#include <iostream>

lr35902::~lr35902() {
    
}

void lr35902::registerBus(Bus *newbus) {
    bus = newbus;
}

uint8_t lr35902::getFlag(FLAGSLR35902 flag) {
    return ((af.f & flag) > 0);
}

void lr35902::setFlag(FLAGSLR35902 flag, bool value) {
    if (value) {
        af.f |= flag;
    } else {
        af.f &= ~(flag);
    }
}

void lr35902::step() {
    uint8_t next = bus->read(pc++);
    prepareOperands(next);
    std::cout << "INST: " << (int) next << "\nSP: " << (int) sp << "\nPC:" << (int) pc << "\n";
    (this->*instlist[next].operate)();
}

/***************************/

uint8_t lr35902::LDH() {
    *op8_1 = *op8_2;
    return 0;
}

uint8_t lr35902::LD() {
    *op8_1 = *op8_2;
    return 0;
}

uint8_t lr35902::LDw() {
    *op16_1 = *op16_2;
    return 0;
}

uint8_t lr35902::LDsp() {
    return 0;
}

uint8_t lr35902::POP() {
    uint16_t temp = 0x0000;
    temp |= bus->read(sp);
    sp++;
    temp |= (bus->read(sp) << 8);
    sp++;
    *op16_1 = temp;
    return 0;
}

uint8_t lr35902::PUSH() {
    sp--;
    bus->write(sp, (uint8_t) *op16_1 >> 8);
    sp--;
    bus->write(sp, (uint8_t) *op16_1 & 0xFF);
    return 0;
}

uint8_t lr35902::NOP() {
    return 0;
}

uint8_t lr35902::STOP() {
    return 0;
}

uint8_t lr35902::HALT() {
    return 0;
}

uint8_t lr35902::DI() {
    return 0;
}

uint8_t lr35902::EI() {
    return 0;
}

uint8_t lr35902::PREFIX() {
    return 0;
}

uint8_t lr35902::INC() {
    *op8_1 = *op8_1 + 1;
    return 0;
}

uint8_t lr35902::INCw() {
    *op16_1 = *op16_1 + 1;
    return 0;
}

uint8_t lr35902::DEC() {
    *op8_1 = *op8_1 - 1;
    return 0;
}

uint8_t lr35902::DECw() {
    *op16_1 = *op16_1 - 1;
    return 0;
}

uint8_t lr35902::ADD() {
    uint16_t temp = *op8_1 + *op8_2;
    setFlag(C, temp & 0x100);
    setFlag(H, ((*op8_1 & 0xF) + (*op8_2 & 0xF)) & 0x10);
    setFlag(N, false);
    setFlag(Z, (temp & 0xFF) == 0);
    *op8_1 = temp & 0xFF;
    return 0;
}

uint8_t lr35902::ADDw() {
    uint temp = *op16_1 + *op16_2;
    setFlag(C, temp & 0x10000);
    setFlag(H, (((*op16_1 & 0xFFF) + (*op16_2 & 0xFFF)) & 0x1000));
    setFlag(N, false);
    return 0;
}

uint8_t lr35902::ADC() {
    uint16_t temp = *op8_1 + *op8_2 + (uint16_t) getFlag(C);
    setFlag(C, temp & 0x100);
    setFlag(H, ((*op8_1 & 0xF) + (*op8_2 & 0xF)) & 0x10);
    setFlag(N, false);
    setFlag(Z, (temp & 0xFF) == 0);
    *op8_1 = temp & 0xFF;
    return 0;
}

uint8_t lr35902::SUB() {
    uint8_t temp = af.a - *op8_1;
    setFlag(C, (*op8_1 > af.a));
    setFlag(H, (((af.a & 0xF) - (*op8_1 & 0xF)) < 0));
    setFlag(N, true);
    setFlag(Z, temp == 0);
    af.a = temp;
    return 0;
}

uint8_t lr35902::SBC() {
    uint8_t temp = *op8_1 - *op8_2 - getFlag(C);
    setFlag(C, (*op8_2 > *op8_1));
    setFlag(H, ((((*op8_1 & 0xF0) - (*op8_2 & 0xF0)) >> 4) < 0));
    setFlag(N, true);
    setFlag(Z, temp == 0);
    *op8_1 = temp;
    return 0;
}

uint8_t lr35902::AND() {
    uint8_t temp = af.a & *op8_1;
    setFlag(C, false);
    setFlag(H, false);
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::OR() {
    uint8_t temp = af.a | *op8_1;
    setFlag(C, false);
    setFlag(H, false);
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::XOR() {
    uint8_t temp = af.a ^ *op8_1;
    setFlag(C, false);
    setFlag(H, false);
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::CP() {
    uint8_t temp = af.a - *op8_1;
    setFlag(C, (*op8_1 > af.a));
    setFlag(H, (((af.a & 0xF) - (*op8_1 & 0xF)) < 0));
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::DAA() {
    return 0;
}

uint8_t lr35902::SCF() {
    setFlag(C, true);
    return 0;
}

uint8_t lr35902::CPL() {
    af.a = ~af.a;
    return 0;
}

uint8_t lr35902::CCF() {
    setFlag(C, ~(getFlag(C)));
    return 0;
}

uint8_t lr35902::RLA() {
    uint8_t temp = getFlag(C);
    setFlag(C, af.a & 0x80);
    af.a = af.a << 1;
    af.a |= temp;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, false);
    return 0;
}

uint8_t lr35902::RLCA() {
    uint8_t temp = af.a & 0x80;
    setFlag(C, af.a & 0x80);
    af.a = af.a << 1;
    af.a |= temp >> 8;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, false);
    return 0;
}

uint8_t lr35902::RRA() {
    uint8_t temp = getFlag(C);
    setFlag(C, af.a & 0x01);
    af.a = af.a >> 1;
    af.a |= temp << 8;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, false);
    return 0;
}

uint8_t lr35902::RRCA() {
    uint8_t temp = af.a & 0x01;
    setFlag(C, temp);
    af.a = af.a >> 1;
    af.a |= temp << 8;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, false);
    return 0;
}

uint8_t lr35902::JR() {
    return 0;
}

uint8_t lr35902::JP() {
    return 0;
}

uint8_t lr35902::RET() {
    return 0;
}

uint8_t lr35902::RETI() {
    return 0;
}

uint8_t lr35902::RST() {
    return 0;
}

uint8_t lr35902::CALL() {
    return 0;
}

uint8_t lr35902::RLC() {
    uint8_t temp = *op8_1 & 0x80;
    setFlag(C, temp);
    *op8_1 = *op8_1 << 1;
    *op8_1 |= temp >> 8;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, *op8_1 == 0);
    return 0;
}

uint8_t lr35902::RRC() {
    uint8_t temp = *op8_1 & 0x01;
    setFlag(C, temp);
    *op8_1 = *op8_1 >> 1;
    *op8_1 |= temp << 8;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, *op8_1 == 0);
    return 0;
}

uint8_t lr35902::RL() {
    uint8_t temp = getFlag(C);
    setFlag(C, *op8_1 & 0x80);
    *op8_1 = *op8_1 << 1;
    *op8_1 |= temp;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, *op8_1 == 0);
    return 0;
}

uint8_t lr35902::RR() {
    uint8_t temp = getFlag(C);
    setFlag(C, *op8_1 & 0x01);
    *op8_1 = *op8_1 >> 1;
    *op8_1 |= temp << 8;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, *op8_1 == 0);
    return 0;
}

uint8_t lr35902::SLA() {
    setFlag(C, (*op8_1 & 0x80));
    *op8_1 = *op8_1 << 1;
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, *op8_1 == 0);
    return 0;
}

uint8_t lr35902::SRA() {
    setFlag(C, *op8_1 & 0x01);
    *op8_1 = (*op8_1 >> 1) | (*op8_1 & 0x80);
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, *op8_1 == 0);
    return 0;
}

uint8_t lr35902::SRL() {
    setFlag(C, *op8_1 & 0x01);
    *op8_1 = (*op8_1 >> 1);
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, *op8_1 == 0);
    return 0;
}

uint8_t lr35902::SWAP() {
    *op8_1 = ((*op8_1 & 0xF0) >> 4) + ((*op8_1 & 0x0F) << 4);
    return 0;
}

uint8_t lr35902::BIT() {
    uint8_t temp = 0x00;
    temp = (1 << *op8_1) & *op8_2;
    setFlag(H, true);
    setFlag(N, false);
    setFlag(Z, temp);
    return 0;
}

uint8_t lr35902::RES() {
    *op8_2 &= ~(1 << *op8_2);
    return 0;
}

uint8_t lr35902::SET() {
    *op8_2 |= (1 << *op8_2);
    return 0;
}
