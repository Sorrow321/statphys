//
// Created by paniquex on 9/21/19.
//

#include "Demonstration.h"
#include <iostream>


void Demonstration::initWindow() {
    std::string title = "Demonstration";
    unsigned framerate_limit = 120;
    sf::VideoMode window_bounds(sf::VideoMode::getDesktopMode().width,
                               sf::VideoMode::getDesktopMode().height);
    this->window = new sf::RenderWindow(sf::VideoMode(window_bounds), title);
    this->window->setFramerateLimit(framerate_limit);
}

void Demonstration::initStates() {
    this->states.push(new DemonstrationState(this->window));
}


Demonstration::Demonstration() {
    this->initWindow();

    this->initStates();
}


Demonstration::~Demonstration() {
    delete this->window;

    while (!this->states.empty()) {
        delete this->states.top();
        this->states.pop();
    }
}

//Methods
void Demonstration::updateDt() {
    /*Updates the dt variable with the time it takes to update and render one frame*/
    this->dt = this->dtClock.restart().asSeconds();

}

void Demonstration::updateSFMLEvents() {
    while (window->pollEvent(this->sfEvent)) {
        if (this->sfEvent.type == sf::Event::Closed)
            this->window->close();
    }
}

void Demonstration::update() {
    this->updateSFMLEvents();

    if (!this->states.empty()) {
        this->states.top()->update(this->dt);

        if(this->states.top()->getQuit()) {
            this->states.top()->endState();
            delete this->states.top();
            this->states.pop();
        }
    } else {
        //APPLICATION end
        this->endApplication();
        this->window->close();

    }
}
void Demonstration::render() {
    this->window->clear();

    //RENDER SOMETHING
    if(!this->states.empty())
        this->states.top()->render(this->window);

    this->window->display();
}

void Demonstration::run() {
    while (this->window->isOpen())
    {
        this->updateDt();
        this->update();
        this->render();
    }
}

void Demonstration::endApplication() {
    std::cout << "Ending Application" << '\n';

}


