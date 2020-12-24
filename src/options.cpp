/*
**
**
**
**
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "options.h"

using namespace std;

options::options(int argc, char** argv)
{
    int opt;
    if (argc < 2) {
        optionsError(argv);
    }
    while ((opt = getopt(argc, argv, "r:")) != -1) {
        switch (opt) {
        case 'r':
            rom = string(optarg);
            break;
        case 'i':
            controlMap = string(optarg);
            break;
        default:
            optionsError(argv);
        }
    }
    if (rom.empty()) {
        optionsError(argv);
    }
}

void options::optionsError(char** argv)
{
    fprintf(stderr, "Usage: %s [-r rom_path.gb]\n", argv[0]);
    exit(EXIT_FAILURE);
}

ostream& operator<<(ostream& os, const options& opt)
{
    os << "[Rom=" << opt.rom << "]";
    return os;
}