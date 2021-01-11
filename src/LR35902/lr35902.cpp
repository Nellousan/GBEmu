#include "lr35902.h"
#include "bus.h"
#include <iostream>

lr35902::~lr35902() {
    
}

void lr35902::registerBus(Bus *newbus) {
    bus = newbus;
}

void lr35902::registerMutexes(std::mutex *mem, std::mutex *reg) {
    memMutex = mem;
    regMutex = reg;
}

bool lr35902::getFlag(FLAGSLR35902 flag) {
    return (bool)((af.f & flag) > 0);
}

void lr35902::setFlag(FLAGSLR35902 flag, bool value) {
    regMutex->lock();
    if (value) {
        af.f |= flag;
    } else {
        af.f &= ~(flag);
    }
    regMutex->unlock();
}

void lr35902::step() {
    uint8_t next = bus->read(pc++);
    prepareOperands(next);
    (this->*instlist[next].operate)();
}

/***************************/
template <typename T>
void lr35902::setVal(T *v1, T *v2) {
    memMutex->lock();
    regMutex->lock();
    *v1 = *v2;
    memMutex->unlock();
    regMutex->unlock();
}

void lr35902::mutLock(void) {
    memMutex->lock();
    regMutex->lock();
}

void lr35902::mutUnlock(void) {
    memMutex->unlock();
    regMutex->unlock();
}

uint8_t lr35902::LDH() {
    mutLock();
    *op8_1 = *op8_2;
    mutUnlock();
    return 0;
}

uint8_t lr35902::LD() {
    mutLock();
    *op8_1 = *op8_2;
    mutUnlock();
    return 0;
}

uint8_t lr35902::LDw() {
    mutLock();
    *op16_1 = *op16_2;
    mutUnlock();
    return 0;
}

uint8_t lr35902::LDsp() {
    return 0;
}

uint8_t lr35902::POP() {
    uint16_t temp = 0x0000;
    temp |= bus->read(sp++);
    temp |= (bus->read(sp++) << 8);
    mutLock();
    *op16_1 = temp;
    mutUnlock();
    return 0;
}

uint8_t lr35902::PUSH() {
    bus->write(--sp, (uint8_t) ((*op16_1) >> 8));
    bus->write(--sp, (uint8_t) ((*op16_1) & 0xFF));
    return 0;
}

uint8_t lr35902::NOP() {
    return 0;
}

uint8_t lr35902::STOP() {
    return 0;
}

uint8_t lr35902::HALT() {
    std::cout << "HALT\n";
    return 0;
}

uint8_t lr35902::DI() {
    std::cout << "DI\n";
    return 0;
}

uint8_t lr35902::EI() {
    std::cout << "EI\n";
    return 0;
}

uint8_t lr35902::PREFIX() {
    (this->*preflist[bus->read(pc)].operate)();
    return 0;
}

uint8_t lr35902::INC() {
    uint8_t temp;
    mutLock();
    temp = *op8_1 + 1;
    *op8_1 = temp;
    mutUnlock();
    setFlag(H, ((temp & 0xF) + (0x01)) & 0x10);
    setFlag(N, true);
    setFlag(Z, (temp) == 0);
    return 0;
}

uint8_t lr35902::INCw() {
    uint16_t temp;
    mutLock();
    temp = *op16_1 + 1;
    *op16_1 = temp;
    mutUnlock();
    setFlag(H, ((temp & 0xF) + (0x0001)) & 0x1000);
    setFlag(N, true);
    setFlag(Z, (temp) == 0);
    return 0;
}

uint8_t lr35902::DEC() {
    uint8_t temp;
    mutLock();
    temp = *op8_1 - 1;
    *op8_1 = temp;
    mutUnlock();
    setFlag(H, ((temp & 0xF) - (0x01)) & 0x10);
    setFlag(N, true);
    setFlag(Z, (temp) == 0);
    return 0;
}

uint8_t lr35902::DECw() {
    uint16_t temp;
    mutLock();
    temp = *op16_1 - 1;
    *op16_1 = temp;
    mutUnlock();
    setFlag(H, ((temp & 0xF) - (0x0001)) & 0x1000);
    setFlag(N, true);
    setFlag(Z, (temp) == 0);
    return 0;
}

uint8_t lr35902::ADD() {
    uint16_t res;
    uint8_t temp1, temp2;
    mutLock();
    temp1 = *op8_1;
    temp2 = *op8_2;
    res = temp1 + temp2;
    *op8_1 = res & 0xFF;
    mutUnlock();
    setFlag(C, res & 0x100);
    setFlag(H, ((temp1 & 0xF) + (temp2 & 0xF)) & 0x10);
    setFlag(N, false);
    setFlag(Z, (res & 0xFF) == 0);
    return 0;
}

uint8_t lr35902::ADDw() {
    uint temp;
    uint16_t temp1, temp2;
    mutLock();
    temp1 = *op16_1;
    temp2 = *op16_2;
    temp = temp1 + temp2;
    *op16_1 = temp;
    mutUnlock();
    setFlag(C, temp & 0x10000);
    setFlag(H, (((temp1 & 0xFFF) + (temp2 & 0xFFF)) & 0x1000));
    setFlag(N, false);
    return 0;
}

uint8_t lr35902::ADC() {
    uint16_t temp;
    uint8_t temp1, temp2;
    mutLock();
    temp1 = *op8_1;
    temp2 = *op8_2;
    temp = temp1 + temp2 + (uint16_t) getFlag(C);
    *op8_1 = temp & 0xFF;
    mutUnlock();
    setFlag(C, temp & 0x100);
    setFlag(H, ((temp1 & 0xF) + (temp2 & 0xF)) & 0x10);
    setFlag(N, false);
    setFlag(Z, (temp & 0xFF) == 0);
    return 0;
}

uint8_t lr35902::SUB() {
    uint8_t temp, temp1, temp2;
    mutLock();
    temp1 = af.a;
    temp2 = *op8_1;
    temp = temp1 - temp2;
    af.a = temp;
    mutUnlock();
    setFlag(C, (temp2 > af.a));
    setFlag(H, (((temp1 & 0xF) - (temp2 & 0xF)) < 0));
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::SBC() {
    uint8_t temp, temp1, temp2;
    mutLock();
    temp1 = *op8_1;
    temp2 = *op8_2;
    temp = temp1 - temp2 - getFlag(C);
    *op8_1 = temp;
    mutUnlock();
    setFlag(C, (temp2 > temp1));
    setFlag(H, ((((temp1 & 0xF0) - (temp2 & 0xF0)) >> 4) < 0));
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::AND() {
    uint8_t temp;
    mutLock();
    temp = af.a & *op8_1;
    mutUnlock();
    setFlag(C, false);
    setFlag(H, false);
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::OR() {
    uint8_t temp;
    mutLock();
    temp = af.a | *op8_1;
    mutUnlock();
    setFlag(C, false);
    setFlag(H, false);
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::XOR() {
    uint8_t temp;
    mutLock();
    temp = af.a ^ *op8_1;
    mutUnlock();
    setFlag(C, false);
    setFlag(H, false);
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::CP() {
    uint8_t temp, temp1, temp2;
    mutLock();
    temp1 = af.a;
    temp2 = *op8_1;
    temp = temp1 - temp2;
    mutUnlock();
    setFlag(C, (temp2 > temp1));
    setFlag(H, (((temp1 & 0xF) - (temp2 & 0xF)) < 0));
    setFlag(N, true);
    setFlag(Z, temp == 0);
    return 0;
}

uint8_t lr35902::DAA() {
    std::cout << "DAA\n";
    return 0;
}

uint8_t lr35902::SCF() {
    setFlag(C, true);
    return 0;
}

uint8_t lr35902::CPL() {
    mutLock();
    af.a = ~af.a;
    mutUnlock();
    return 0;
}

uint8_t lr35902::CCF() {
    setFlag(C, false);
    return 0;
}

uint8_t lr35902::RLA() {
    uint8_t temp, temp1;
    mutLock();
    temp = getFlag(C);
    temp1 = af.a;
    af.a = af.a << 1;
    af.a |= temp;
    mutUnlock();
    setFlag(C, temp1 & 0x80);
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, false);
    return 0;
}

uint8_t lr35902::RLCA() {
    uint8_t temp, temp1;
    mutLock();
    temp = af.a & 0X80;
    temp1 = af.a;
    af.a = af.a << 1;
    af.a |= temp >> 8;
    mutUnlock();
    setFlag(C, temp1 & 0x80);
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, false);
    return 0;
}

//AAAAAAA

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
    mutLock();
    if (*op8_1) {
        pc += (int8_t)(*op8_2);
    }
    mutUnlock();
    return 0;
}

uint8_t lr35902::JRsp() {
    mutLock();
    pc += (int8_t)(*op8_2);
    mutUnlock();
    return 0;
}

uint8_t lr35902::JP() {
    mutLock();
    if (*op8_1) {
        pc = *op16_2;
    }
    mutUnlock();
    return 0;
}

uint8_t lr35902::JPsp() {
    mutLock();
    pc = *op16_1;
    mutUnlock();
    return 0;
}

uint8_t lr35902::RET() {
    uint16_t temp = 0x0000;
    temp |= bus->read(sp++);
    temp |= (bus->read(sp++) << 8);
    regMutex->lock();
    pc = temp;
    regMutex->unlock();
    return 0;
}

uint8_t lr35902::RETI() {
    uint16_t temp = 0x0000;
    temp |= bus->read(sp++);
    temp |= (bus->read(sp++) << 8);
    pc = temp;
    return 0;
}

uint8_t lr35902::RST() {
    std::cout << "RST\n";
    return 0;
}

uint8_t lr35902::CALL() {
    if (*op8_1) {
        bus->write(--sp, (uint8_t) (pc >> 8));
        bus->write(--sp, (uint8_t) (pc & 0xFF));
        regMutex->lock();
        pc = *op16_1;
        regMutex->unlock();
    }
    return 0;
}

uint8_t lr35902::CALLsp() {
    bus->write(--sp, (uint8_t) (pc >> 8));
    bus->write(--sp, (uint8_t) (pc & 0xFF));
    regMutex->lock();
    pc = *op16_1;
    regMutex->unlock();
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

//BBBBBBB

uint8_t lr35902::SLA() {
    uint8_t temp, temp2;
    mutLock();
    temp = *op8_1;
    temp2 = *op8_1 << 1;
    *op8_1 = temp2;
    mutUnlock();
    setFlag(C, (temp & 0x80));
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, temp2 == 0);
    return 0;
}

uint8_t lr35902::SRA() {
    uint8_t temp, temp2;
    mutLock();
    temp = *op8_1;
    temp2 = (*op8_1 >> 1) | (*op8_1 & 0x80);
    *op8_1 = temp2;
    mutUnlock();
    setFlag(C, temp & 0x01);
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, temp2 == 0);
    return 0;
}

uint8_t lr35902::SRL() {
    uint8_t temp, temp2;
    mutLock();
    temp = *op8_1;
    temp2 = (*op8_1 >> 1);
    *op8_1 = temp2;
    mutUnlock();
    setFlag(C, temp & 0x01);
    setFlag(H, false);
    setFlag(N, false);
    setFlag(Z, temp2 == 0);
    return 0;
}

uint8_t lr35902::SWAP() {
    mutLock();
    *op8_1 = ((*op8_1 & 0xF0) >> 4) + ((*op8_1 & 0x0F) << 4);
    mutUnlock();
    return 0;
}

uint8_t lr35902::BIT() {
    uint8_t temp = 0x00;
    mutLock();
    temp = (1 << *op8_1) & *op8_2;
    mutUnlock();
    setFlag(H, true);
    setFlag(N, false);
    setFlag(Z, temp);
    return 0;
}

uint8_t lr35902::RES() {
    mutLock();
    *op8_2 &= ~(1 << *op8_2);
    mutUnlock();
    return 0;
}

uint8_t lr35902::SET() {
    mutLock();
    *op8_2 |= (1 << *op8_2);
    mutUnlock();
    return 0;
}
