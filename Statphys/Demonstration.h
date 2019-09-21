//
// Created by paniquex on 9/21/19.
//

#ifndef STATPHYS_DEMONSTRATION_H
#define STATPHYS_DEMONSTRATION_H

#include "State.h"
#include "DemonstrationState.h"


class Demonstration {
private:
    //Variables
    sf::RenderWindow *window;
    sf::Event sfEvent;

    sf::Clock dtClock;
    float dt;

    //For demonstration states: "Main window", "demo", etc.
    std::stack<State*> states;

    //Init
    void initWindow();
    void initStates();
public:
    Demonstration();

    virtual ~Demonstration();

    //Methods:

    //Regular
    void endApplication();
    //update
    void updateDt();
    void updateSFMLEvents();
    void update();

    //Render
    void render();

    //Core
    void run();
};


#endif //STATPHYS_DEMONSTRATION_H
