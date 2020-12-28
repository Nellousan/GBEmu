#include "dmg.h"
#include <iostream>
#include <chrono>
#include <cmath>

Dmg::Dmg() {
    cpu = new lr35902;
    bus = new Bus;
    window = new sf::RenderWindow(sf::VideoMode(WINW, WINH), "DMG emu");
    font = new sf::Font;
    font->loadFromFile(FONTPATH);
    debugRender = new debugWindow(this);
    cpu->registerBus(bus);
    framerate = 60;
    tickrate = 4.194304f * pow(10.0f, 6.0f);
    lastFrame = std::chrono::system_clock::now();
    lastTick = std::chrono::system_clock::now();
    inputs = {
        {"N", {sf::Keyboard::N, false, false}},
        {"R", {sf::Keyboard::R, false, false}},
        {"Q", {sf::Keyboard::Q, false, false}},
        {" ", {sf::Keyboard::Space, false, false}}
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

void Dmg::setFramerateLimit(double framerate) {
    this->framerate = framerate;
}

void Dmg::winRender() {
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> work_time = t - lastFrame;
    if (work_time.count() < (1000.0f / (framerate))) {
        return;
    }
    window->draw(*debugRender->renderMain());
    window->display();
    lastFrame = std::chrono::system_clock::now();
}

bool Dmg::isRunning() {
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> work_time = t - lastTick;
    std::cout << "Work Time: " << work_time.count() << "\n";
    if (work_time.count() < (1000.0f / tickrate)) {
        std::cout << "working too fast!\n";
    }
    lastTick = std::chrono::system_clock::now();
    return true & window->isOpen();
}

void Dmg::cpuTest() {
    while (isRunning()) {
        //manageEvent();
        //winRender();
        if (getInput("Q")) {
            break;
        }
        if (!isPressed("N")) {
            cpu->step();
        } else if (getInput(" ")) {
            cpu->step();
        }
    }
}
