#ifndef __BUS_H__
#define __BUS_H__

#include <array>
#include <cstdint>

class Bus {
public:

    Bus();
    ~Bus();

    uint8_t *ram;

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);
    uint8_t *readPtr(uint16_t addr);
};
#endif // __BUS_H__