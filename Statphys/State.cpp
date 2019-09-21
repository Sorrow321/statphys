//
// Created by paniquex on 9/21/19.
//

#include "State.h"

State::State(sf::RenderWindow *window) {
    this->window = window;
    this->quit = false;
}

State::~State() {

}

const bool& State::getQuit() const {
    return this->quit;
}

void State::checkForQuit() {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        //end by escape
        this->quit = true;
    }
}