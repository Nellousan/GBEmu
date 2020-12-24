/*
** Personal Project, 2020
**
** NesEmulator
**
*/

#include "dmg.h"
#include <iostream>
#include <string>

using namespace std;

struct XV {
    uint8_t x;
    uint8_t v;
};

int main(int argc, char** argv)
{
    options *option = new options(argc, argv);
    rom_t *rom = load_file(option->rom.c_str());

    Dmg *dmg = new Dmg;

    dmg->bus->loadRom(rom);
    dmg->cpuTest();

    a_malloc(A_MALLOC_FREE);
    delete(option);
    delete(dmg);
    return 0;
}
