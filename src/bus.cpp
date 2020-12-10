#include "bus.h"
#include "a_malloc.h"

Bus::Bus() {
    ram = (uint8_t *) malloc(sizeof(uint8_t) * 64 * 1024);
    for (int i = 0; i < 64 * 1024; i++)
        ram[i] = 0x00;
}

Bus::~Bus() {
    free(ram);
}

void Bus::write(uint16_t addr, uint8_t data) {
    ram[addr] = data;
}

uint8_t Bus::read(uint16_t addr) {
    return ram[addr];
}

uint8_t *Bus::readPtr(uint16_t addr) {
    return &ram[addr];
}