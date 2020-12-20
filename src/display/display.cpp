#include "dmg.h"
#include <iostream>

Dmg::Dmg() {
    cpu = new lr35902;
    bus = new Bus;
    window = new sf::RenderWindow(sf::VideoMode(WINW, WINH), "DMG emu");
    font = new sf::Font;
    font->loadFromFile(FONTPATH);
    debugRender = new debugWindow(this);
    cpu->registerBus(bus);
    inputs = {
        {"N", {sf::Keyboard::N, false, false}},
        {"R", {sf::Keyboard::R, false, false}},
        {"Q", {sf::Keyboard::Q, false, false}}
    };
}

Dmg::~Dmg() {
    delete(cpu);
    delete(bus);
    window->close();
    delete(window);
    delete(debugRender);
    delete(font);
}

void Dmg::cpuTest() {
    window->setFramerateLimit(60);
    while (window->isOpen()) {
        manageEvent();
        window->draw(*debugRender->renderMain());
        window->display();
        if (getInput("Q")) {
            break;
        }
        if (getInput("N")) {
            cpu->step();
        }
    }
}
