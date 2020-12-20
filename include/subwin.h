#ifndef __SUBWIN_H__
#define __SUBWIN_H__

#include <SFML/Graphics.hpp>

class SubWin {
    public:
    sf::RenderTexture *tex;
    sf::Sprite *spr;

    SubWin(const sf::Vector2f size, const sf::Vector2f pos = sf::Vector2f(0, 0));
    ~SubWin();

    void draw(sf::Drawable *drawable);
    void clear(sf::Color color);
    sf::Vector2u getSize(void);
    sf::Sprite *display(void);
};

#endif // __SUBWIN_H__