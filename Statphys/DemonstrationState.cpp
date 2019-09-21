//
// Created by paniquex on 9/21/19.
//

#include "DemonstrationState.h"
#include <iostream>

DemonstrationState::DemonstrationState(sf::RenderWindow *window)
    : State(window)
{

}

DemonstrationState::~DemonstrationState() {

}

void DemonstrationState::endState() {
    std::cout << "End Demonstration state!";
}

void DemonstrationState::updateInput(const float &dt) {
    this->checkForQuit();
}

void DemonstrationState::render(sf::RenderTarget* target) {
    if (!target) {
        target = this->window;
    }
}

void DemonstrationState::update(const float &dt) {
    this->updateInput(dt);

    //Update input
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        std::cout << "a";
}

