#include "dmg.h"
#include <iostream>

Dmg::Dmg() {
    cpu = new lr35902;
    bus = new Bus;
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "DMG emu");
    font = new sf::Font;
    font->loadFromFile("font/consola.ttf");
    inputs = {
        {"N", {sf::Keyboard::N, false, false}}/*,
        {"R", {sf::Keyboard::R, false, false}}*/
    };
}

Dmg::~Dmg() {
    delete(cpu);
    delete(bus);
    delete(window);
    delete(font);
}

void Dmg::cpuTest() {
    while (window->isOpen()) {
        manageEvent();
    }
    if (getInput("N")) {
        std::cout << "lol";
    }
}
