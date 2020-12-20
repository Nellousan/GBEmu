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

    mainWin = new SubWin(sf::Vector2f(WINW / 2, WINH), sf::Vector2f(WINW / 2, 0));

    romWin = new SubWin(sf::Vector2f(WINW / 2, WINH / 4), sf::Vector2f(0, (WINH / 4) * 0));
    ramWin = new SubWin(sf::Vector2f(WINW / 2, WINH / 4), sf::Vector2f(0, (WINH / 4) * 1));
    stkWin = new SubWin(sf::Vector2f(WINW / 2, WINH / 4), sf::Vector2f(0, (WINH / 4) * 2));
    asmWin = new SubWin(sf::Vector2f(WINW / 4, WINH / 4), sf::Vector2f(0, (WINH / 4) * 3));
    regWin = new SubWin(sf::Vector2f(WINW / 4, WINH / 4), sf::Vector2f(WINW / 4, (WINH / 4) * 3));

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

sf::Sprite *debugWindow::drawRom() {
    sf::Vector2u size = romWin->getSize();
    sf::RectangleShape shape(sf::Vector2f(size.x - 10, size.y - 10));
    std::string str;
    romWin->clear(sf::Color::Blue);
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
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color(30, 30, 150));
    shape.setOutlineThickness(10);
    shape.setPosition(5, 5);
    romWin->draw(&shape);
    return romWin->display();
}

sf::Sprite *debugWindow::drawRam() {
    sf::Vector2u size = ramWin->getSize();
    sf::RectangleShape shape(sf::Vector2f(size.x - 10, size.y - 5));
    std::string str;
    ramWin->clear(sf::Color::Blue);
    for (uint16_t i = 0; i < 16; i++) {
        str = int_to_hex((uint16_t)(i * 16 + RAMADDR), "$");
        text->setString(str);
        text->setPosition(5, i * CHARSIZE);
        ramWin->draw(text);
        str = "";
        for (uint16_t j = 0; j < 16; j++) {
            str += int_to_hex(access->bus->mem[i * 16 + j + RAMADDR], "") + " ";
        }
        text->setString(str);
        text->setPosition(5 * CHARSIZE, i * CHARSIZE);
        ramWin->draw(text);
    }
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color(30, 30, 150));
    shape.setOutlineThickness(10);
    shape.setPosition(5, 0);
    ramWin->draw(&shape);
    return ramWin->display();
}

sf::Sprite *debugWindow::drawReg() {
    sf::Vector2u size = regWin->getSize();
    sf::RectangleShape shape(sf::Vector2f(size.x - 5, size.y - 5));
    std::string str = "";
    lr35902 *cpu = access->cpu;
    regWin->clear(sf::Color::Blue);
    str += "[A]  " + int_to_hex(cpu->af.a, "") + "\t  [F]  " + int_to_hex(cpu->af.f, "") + "\n";
    str += "[B]  " + int_to_hex(cpu->bc.b, "") + "\t  [C]  " + int_to_hex(cpu->bc.c, "") + "\n";
    str += "[D]  " + int_to_hex(cpu->de.d, "") + "\t  [E]  " + int_to_hex(cpu->de.e, "") + "\n";
    str += "[H]  " + int_to_hex(cpu->hl.h, "") + "\t  [L]  " + int_to_hex(cpu->hl.l, "") + "\n";
    str += "[PC] " + int_to_hex(cpu->pc, "") + "\t[SP] " + int_to_hex(cpu->sp, "") + "\n";
    text->setString(str);
    text->setPosition(10, 10);
    regWin->draw(text);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color(30, 30, 150));
    shape.setOutlineThickness(10);
    shape.setPosition(0, 0);
    regWin->draw(&shape);
    return regWin->display();
}

sf::Sprite *debugWindow::drawAsm() {
    sf::Vector2u size = asmWin->getSize();
    sf::RectangleShape shape(sf::Vector2f(size.x - 10, size.y - 5));
    std::string str = access->cpu->disassemble10(access->cpu->pc);
    asmWin->clear(sf::Color::Blue);
    text->setString(str);
    text->setPosition(15, 5);
    asmWin->draw(text);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color(30, 30, 150));
    shape.setOutlineThickness(10);
    shape.setPosition(5, 0);
    asmWin->draw(&shape);
    return asmWin->display();
}

sf::Sprite *debugWindow::drawStk() {
    sf::Vector2u size = stkWin->getSize();
    sf::RectangleShape shape(sf::Vector2f(size.x - 10, size.y - 5));
    std::string str = "";
    stkWin->clear(sf::Color::Blue);
    for (uint16_t i = 0; i < 16; i++) {
        str = int_to_hex((uint16_t)(STKTOP - (i * 16)), "$");
        text->setString(str);
        text->setPosition(5, i * CHARSIZE);
        stkWin->draw(text);
        str = "";
        for (uint16_t j = 0; j < 16; j++) {
            str += int_to_hex(access->bus->mem[STKTOP - (i * 16 + j)], "") + " ";
        }
        text->setString(str);
        text->setPosition(5 * CHARSIZE, i * CHARSIZE);
        stkWin->draw(text);
    }
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color(30, 30, 150));
    shape.setOutlineThickness(10);
    shape.setPosition(5, 0);
    stkWin->draw(&shape);
    return stkWin->display();
}

sf::Sprite *debugWindow::renderMain() {
    mainWin->clear(sf::Color::Blue);
    mainWin->draw(drawRom());
    mainWin->draw(drawRam());
    mainWin->draw(drawStk());
    mainWin->draw(drawAsm());
    mainWin->draw(drawReg());
    return mainWin->display();
}
