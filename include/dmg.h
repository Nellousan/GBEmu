#ifndef __DMG_H__
#define __DMG_H__

#include "rom.h"
#include "options.h"
#include "bus.h"
#include "a_malloc.h"
#include "lr35902.h"
#include "subwin.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

#define WINH 1000
#define WINW 1500
#define FONTPATH "font/consola.ttf"
#define ROMADDR 0x0000
#define ROMSIZE 0X7FFF
#define RAMADDR 0xC000
#define RAMSIZE 0x1FFF
#define CHARSIZE 20

class Dmg;

class debugWindow {
public:

    Dmg *access;
    void registerAccess(Dmg *access);

    SubWin *mainWin;
    SubWin *ramWin;
    SubWin *romWin;
    SubWin *regWin;
    SubWin *asmWin;
    SubWin *stkWin;

    sf::Text *text;

    debugWindow(Dmg *acc);
    ~debugWindow();

    sf::Sprite *drawRam();
    sf::Sprite *drawRom();
    sf::Sprite *drawReg();
    sf::Sprite *drawAsm();
    sf::Sprite *drawStk();
    sf::Sprite *renderMain();
};



class Dmg {
public:
    sf::RenderWindow *window;
    debugWindow *debugRender;
    sf::Font *font;
    sf::Event event;

    lr35902 *cpu;
    Bus *bus;

    Dmg();
    ~Dmg();

    void cpuTest();

    struct input {
        sf::Keyboard::Key key;
        bool justPressed;
        bool alreadyPressed;
    };

    std::multimap<std::string, input> inputs;

    /************************/

    void manageEvent();
    void checkInput(Dmg::input *ipt);
    bool getInput(std::string str);

    /***********************/
};

#endif // __DMG_H__