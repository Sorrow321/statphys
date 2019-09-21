
#ifndef STATPHYS_MENU_BUTTON_H
#define STATPHYS_MENU_BUTTON_H


#include <SFML/Graphics/RectangleShape.hpp>
#include "SFML/Graphics.hpp"

class Menu_button {
private:
    sf::RectangleShape shape;
    sf::Font* font;
    sf::Text text;

    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;

public:
    Menu_button(float x, float y, float width, float height,
            sf::Font* font, std::string text,
            sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor);
    ~Menu_button();


    void update(const sf::Vector2f mousePos);
    void render(sf::RenderTarget* target);
};


#endif //STATPHYS_MENU_BUTTON_H
