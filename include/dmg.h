#ifndef __DMG_H__
#define __DMG_H__

#include "rom.h"
#include "options.h"
#include "bus.h"
#include "a_malloc.h"
#include "lr35902.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

class Dmg {
public:
    sf::RenderWindow *window;
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

    void manageEvent();
    void checkInput(input ipt);
    bool getInput(std::string str);
};

#endif // __DMG_H__