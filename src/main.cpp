/*
** Personal Project, 2020
**
** NesEmulator
**
*/

#include "dmg.h"
#include "dmg_display.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    //options* option = new options(argc, argv);

    /*rom_t *rom = load_file(argv[1]);
    lr35902 *cpu = new lr35902;
    cpu->disassemble(rom);
    a_malloc(A_MALLOC_FREE);
    delete cpu;*/

    Dmg *dmg = new Dmg;

    dmg->cpuTest();

    return 0;
}
