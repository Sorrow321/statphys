//
// Created by paniquex on 9/21/19.
//

#ifndef STATPHYS_STATE_H
#define STATPHYS_STATE_H
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <stack>
#include <map>
#include <vector>

class State {
    //Virtual class for different main_menu state
private:

    std::vector<sf::Texture> textures;
    bool quit;

public:
    sf::RenderWindow *window;
    State(sf::RenderWindow *window);
    virtual ~State();

    const bool& getQuit() const;

    virtual void checkForQuit();

    virtual void endState() = 0;
    virtual void updateInput(const float& dt) = 0;
    virtual void update(const float& dt) = 0;
    virtual void render(sf::RenderTarget* target) = 0;
};
#endif //STATPHYS_STATE_H
