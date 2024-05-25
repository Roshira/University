#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu();
    void show(sf::RenderWindow& window);

private:
    sf::Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
    sf::Sprite menu1, menu2, menu3, about, menuBg;
    bool loadTextures();
    void setupSprites();
};

#endif // MENU_H