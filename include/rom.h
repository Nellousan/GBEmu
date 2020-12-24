#ifndef __ROM_H__
#define __ROM_H__

#include <cstdint>
#include <stddef.h>

typedef struct romstruct {
    uint8_t *data;
    size_t size;
} rom_t;

rom_t* load_file(const char* path);

#endif // __ROM_H__

