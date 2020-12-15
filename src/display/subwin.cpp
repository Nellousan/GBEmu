#include "subwin.h"


SubWin::SubWin(const sf::Vector2f size, const sf::Vector2f pos) {
    tex = new sf::RenderTexture;
    spr = new sf::Sprite;
    tex->create(size.x, size.y);
    spr->setTexture(tex->getTexture());
    spr->setPosition(pos);
}

SubWin::~SubWin() {
    delete(tex);
    delete(spr);
}

void SubWin::draw(sf::Drawable *drawable) {
    tex->draw(*drawable);
}

void SubWin::clear(sf::Color color) {
    tex->clear(color);
}

sf::Sprite *SubWin::display() {
    tex->display();
    return spr;
}
