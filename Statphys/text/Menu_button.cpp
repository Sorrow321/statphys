//
// Created by paniquex on 9/21/19.
//

#include "Menu_button.h"

Menu_button::Menu_button(float x, float y, float width, float height, sf::Font *font, std::string text,
                         sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor) {
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));

    this->font = font;
    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setFillColor(activeColor);
    this->text.setCharacterSize(12);
    this->text.setPosition(
            this->shape.getPosition().x / 2.f - this->text.getGlobalBounds().width / 2.f,
            this->shape.getPosition().y / 2.f - this->text.getGlobalBounds().width / 2.f);

}


void Menu_button::update(const sf::Vector2f mousePos) {
    if(this->shape.getGlobalBounds().contains(mousePos)) {

    }
}


void Menu_button::render(sf::RenderTarget *target) {
    target->draw(this->shape);
}