#include <string>
#include <iomanip>
#include <iostream>
#include "dmg.h"

template <typename T>
static std::string int_to_hex(T i, std::string prefix)
{
    std::stringstream stream;
    stream << prefix
           << std::setfill('0') << std::setw(sizeof(T) * 2)
           << std::hex << (0xFFFF & i);
    return stream.str();
}

debugWindow::debugWindow(Dmg *acc) {

    access = acc;

    mainWin = new SubWin(sf::Vector2f(WINW / 2, WINH));

    romWin = new SubWin(sf::Vector2f(WINW / 2, WINH / 4), sf::Vector2f(0, (WINH / 4) * 0));
    ramWin = new SubWin(sf::Vector2f(WINW / 2, WINH / 4), sf::Vector2f(0, (WINH / 4) * 1));
    stkWin = new SubWin(sf::Vector2f(WINW / 2, WINH / 4), sf::Vector2f(0, (WINH / 4) * 2));
    asmWin = new SubWin(sf::Vector2f(WINW / 4, WINH / 4), sf::Vector2f(0, (WINH / 4) * 3));
    regWin = new SubWin(sf::Vector2f(WINW / 4, WINH / 4), sf::Vector2f(0, (WINH / 4) * 3));

    text = new sf::Text;
    text->setFont(*access->font);
    text->setFillColor(sf::Color::White);
    text->setCharacterSize(CHARSIZE);
}

debugWindow::~debugWindow() {
    delete(mainWin);
    delete(romWin);
    delete(ramWin);
    delete(stkWin);
    delete(asmWin);
    delete(regWin);
    delete(text);
}

void debugWindow::registerAccess(Dmg *newaccess) {
    access = newaccess;
}

sf::Sprite *debugWindow::drawRam() {
    return ramWin->display();
}

sf::Sprite *debugWindow::drawRom() {
    std::string str;
    for (uint16_t i = 0; i < 16; i++) {
        str = int_to_hex((uint16_t)(i * 16), "$");
        text->setString(str);
        text->setPosition(5, i * CHARSIZE);
        romWin->draw(text);
        str = "";
        for (uint16_t j = 0; j < 16; j++) {
            str += int_to_hex(access->bus->mem[i * 16 + j], "") + " ";
        }
        text->setString(str);
        text->setPosition(5 * CHARSIZE, i * CHARSIZE);
        romWin->draw(text);
    }
    return romWin->display();
}

sf::Sprite *debugWindow::drawReg() {
    return regWin->display();
}

sf::Sprite *debugWindow::drawAsm() {
    return asmWin->display();
}

sf::Sprite *debugWindow::drawStk() {
    return stkWin->display();
}

sf::Sprite *debugWindow::renderMain() {
    mainWin->clear(sf::Color::Blue);
    mainWin->draw(drawRom());
    return mainWin->display();
}
