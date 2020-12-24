#include <cstdint>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "rom.h"
#include "a_malloc.h"

rom_t* load_file(const char* path)
{
    int fd = open(path, O_RDONLY);
    rom_t *rom = (rom_t*) a_malloc(sizeof(rom_t));
    if (fd == -1)
        return NULL;
    struct stat fstats;
    fstat(fd, &fstats);
    rom->data = (uint8_t*) a_malloc(sizeof(char) * (fstats.st_size));
    rom->size = read(fd, rom->data, fstats.st_size);
    close(fd);
    return rom;
}