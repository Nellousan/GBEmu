#include "bus.h"
#include "rom.h"
#include "a_malloc.h"
#include <iostream>

Bus::Bus() {
    mem = (uint8_t *) malloc(sizeof(uint8_t) * 64 * 1024);
    for (int i = 0; i < 64 * 1024; i++)
        mem[i] = 0x00;
}

Bus::~Bus() {
    free(mem);
}

void Bus::registerMutexes(std::mutex *mem) {
    memMutex = mem;
}

void Bus::loadRom(rom_t *rom) {
    memMutex->lock();
    for (size_t i = 0; i < rom->size; i++) {
        mem[i] = rom->data[i];
    }
    memMutex->unlock();
}

void Bus::write(uint16_t addr, uint8_t data) {
    memMutex->lock();
    mem[addr] = data;
    memMutex->unlock();
}

uint8_t Bus::read(uint16_t addr) {
    uint8_t temp;
    memMutex->lock();
    temp = mem[addr];
    memMutex->unlock();
    return temp;
}

uint8_t *Bus::readPtr(uint16_t addr) {
    uint8_t *temp;
    memMutex->lock();
    temp = &mem[addr];
    memMutex->unlock();
    return temp;
}