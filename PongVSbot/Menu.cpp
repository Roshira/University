#include "Menu.h"

Menu::Menu() {
    if (!loadTextures()) {
        // Handle error
    }
    setupSprites();
}

bool Menu::loadTextures() {
    if (!menuTexture1.loadFromFile("111.png") ||
        !menuTexture2.loadFromFile("222.png") ||
        !menuTexture3.loadFromFile("333.png") ||
        !aboutTexture.loadFromFile("about.png") ||
        !menuBackground.loadFromFile("Blackground1.jpg")) {
        return false;
    }
    return true;
}

void Menu::setupSprites() {
    menu1.setTexture(menuTexture1);
    menu2.setTexture(menuTexture2);
    menu3.setTexture(menuTexture3);
    about.setTexture(aboutTexture);
    menuBg.setTexture(menuBackground);

    menu1.setPosition(100, 30);
    menu2.setPosition(100, 90);
    menu3.setPosition(100, 150);
    menuBg.setPosition(345, 0);
}

void Menu::show(sf::RenderWindow& window) {
    bool isMenu = true;
    int menuNum = 0;

    while (isMenu) {
        menu1.setColor(sf::Color::White);
        menu2.setColor(sf::Color::White);
        menu3.setColor(sf::Color::White);
        menuNum = 0;
        window.clear(sf::Color(129, 181, 221));

        if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(window))) {
            menu1.setColor(sf::Color::Blue);
            menuNum = 1;
        }
        if (sf::IntRect(100, 90, 300, 50).contains(sf::Mouse::getPosition(window))) {
            menu2.setColor(sf::Color::Blue);
            menuNum = 2;
        }
        if (sf::IntRect(100, 150, 300, 50).contains(sf::Mouse::getPosition(window))) {
            menu3.setColor(sf::Color::Blue);
            menuNum = 3;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (menuNum == 1) isMenu = false; // Exit menu
            if (menuNum == 2) {
                window.draw(about);
                window.display();
                while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));
            }
            if (menuNum == 3) {
                window.close();
                isMenu = false;
            }
        }

        window.draw(menuBg);
        window.draw(menu1);
        window.draw(menu2);
        window.draw(menu3);

        window.display();
    }
}