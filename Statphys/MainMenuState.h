//
// Created by paniquex on 9/21/19.
//

#ifndef STATPHYS_MAINMENUSTATE_H
#define STATPHYS_MAINMENUSTATE_H
#include "State.h"

class MainMenuState {
private:

public:
    MainMenuState(sf::RenderWindow *window);
    virtual ~MainMenuState();

    //functions
    void endState();

    void updateKeybinds(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget* target);

};


#endif //STATPHYS_MAINMENUSTATE_H
