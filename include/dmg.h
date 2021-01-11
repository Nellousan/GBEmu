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
#include <chrono>
#include <mutex>

#define WINH 1000
#define WINW 1500
#define FONTPATH "font/consola.ttf"
#define ROMADDR 0x0000
#define ROMSIZE 0X7FFF
#define RAMADDR 0xC000
#define RAMSIZE 0x1FFF
#define STKADDR 0xFF80
#define STKSIZE 0x007E
#define STKTOP 0xFFFF
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
    double framerate;
    std::chrono::system_clock::time_point lastFrame;
    double tickrate;
    std::chrono::system_clock::time_point lastTick;
    bool stopThreads;

    std::mutex memMutex;
    std::mutex regMutex;

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

    bool isRunning();

    void manageEvent();
    void checkInput(Dmg::input *ipt);
    bool getInput(std::string str);
    bool isPressed(std::string str);

    //void renderRunnable(void);

    void setFramerateLimit(double framerate);
    void winRender(void);

    /***********************/
};

#endif // __DMG_H__