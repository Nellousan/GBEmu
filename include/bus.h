#ifndef __BUS_H__
#define __BUS_H__

#include <array>
#include <cstdint>
#include <mutex>
#include "rom.h"

class Bus {
public:

    Bus();
    ~Bus();

    std::mutex *memMutex;

    uint8_t *mem;

    void registerMutexes(std::mutex *mem);
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);
    uint8_t *readPtr(uint16_t addr);
    void loadRom(rom_t *rom);
};
#endif // __BUS_H__