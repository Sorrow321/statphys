//
// Created by paniquex on 9/21/19.
//

#ifndef STATPHYS_DEMONSTRATIONSTATE_H
#define STATPHYS_DEMONSTRATIONSTATE_H

#include "State.h"


class DemonstrationState: public State {
private:

public:
    DemonstrationState(sf::RenderWindow *window);
    virtual ~DemonstrationState();

    //functions
    void endState();

    void updateInput(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget* target);
};


#endif //STATPHYS_DEMONSTRATIONSTATE_H
