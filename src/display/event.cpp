#include "dmg.h"
#include <iostream>

bool Dmg::getInput(std::string str) {
    auto ipt = inputs.find(str);
    if (ipt != inputs.end()) {
        if (ipt->second.justPressed) {
            ipt->second.alreadyPressed = true;
            ipt->second.justPressed = false;
            return true;
        }
    }
    return false;
}

bool Dmg::isPressed(std::string str) {
    auto ipt = inputs.find(str);
    if (ipt != inputs.end()) {
        return ipt->second.justPressed | ipt->second.alreadyPressed;
    }
    return false;
}

void Dmg::checkInput(Dmg::input *ipt)
{
    if (sf::Keyboard::isKeyPressed(ipt->key)) {
        if (ipt->alreadyPressed == false) {
            ipt->justPressed = true;
        }
    } else {
        ipt->justPressed = false;
        ipt->alreadyPressed = false;
    }
}

void Dmg::manageEvent()
{
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }
    for (auto &x: inputs) {
        checkInput(&x.second);
    }
}