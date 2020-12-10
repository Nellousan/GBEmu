#include "dmg.h"
#include <iostream>

bool Dmg::getInput(std::string str) {
    auto ipt = inputs.find(str);
    if (ipt != inputs.end()) {
        return ipt->second.justPressed;
    }
    return false;
}

void Dmg::checkInput(Dmg::input ipt)
{
    if (sf::Keyboard::isKeyPressed(ipt.key)) {
        if (ipt.justPressed || ipt.alreadyPressed) {
            ipt.justPressed = false;
            ipt.alreadyPressed = true;
        } else if (!ipt.alreadyPressed) {
            cout << "ahah";
            ipt.justPressed = true;
        }
    } else {
        std::cout << "lolz";
        ipt.justPressed = false;
        ipt.alreadyPressed = false;
    }
}

void Dmg::manageEvent()
{
    std::string key;
    input value;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }
    for (auto x: inputs) {
        checkInput(x.second);
    }
}