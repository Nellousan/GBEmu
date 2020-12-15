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

void Bus::loadRom(rom_t *rom) {
    for (int i = 0; i < rom->size; i++) {
        mem[i] = rom->data[i];
    }
}

void Bus::write(uint16_t addr, uint8_t data) {
    mem[addr] = data;
}

uint8_t Bus::read(uint16_t addr) {
    return mem[addr];
}

uint8_t *Bus::readPtr(uint16_t addr) {
    return &mem[addr];
}