#include "SFML/Graphics.hpp"
#include <iostream>
#include "box.cpp"

struct Button
{
    sf::Vector2f bPosition;
    sf::IntRect bSprite;
    sf::Sprite sprite;

    Button(sf::Texture &texture, sf::Vector2f bPosition, sf::IntRect bSprite)
        : bPosition{ bPosition }, bSprite{ bSprite }
    {
        sprite.setTexture(texture);
        sprite.setTextureRect(bSprite);
        sprite.setPosition(bPosition);
        sprite.setScale(0.2, 0.2);
    }
};

int main()
{
    int n_mol = 1000; 
    double radius = 2.0;

    //std::tuple<double, double, double, double> bounds = { 5.0, 590.0, 5.0, 590.0 };
    std::tuple<double, double, double, double> bounds = { 5.0, 800.0, 5.0, 590.0 };

    sf::RenderWindow window(sf::VideoMode(1200, 600), "Demonstration");
    window.clear(sf::Color(255, 255, 255));
    Box b(radius, bounds, n_mol, 5, 5, 10);
    std::vector<sf::CircleShape> myvec(n_mol, sf::CircleShape(10));

    std::vector<Button> gui;
    sf::Texture guiTexture;

    guiTexture.loadFromFile("text/onoff.png");
    gui.emplace_back(guiTexture, sf::Vector2f(864.4, 264.4), sf::IntRect(0, 0, 356, 356));
    gui.emplace_back(guiTexture, sf::Vector2f(864.4, 264.4), sf::IntRect(438, 0, 356, 356));
    
    bool toggle = true;
    
    srand(100500);
    {
        auto m = b.get_molecules();
        const std::vector<Molecule>& v = m.get();
        for (int i = 0; i < v.size(); i++) {
            myvec[i].setRadius(radius);
            myvec[i].setPosition(v[i].position.first, v[i].position.second);
            myvec[i].setOrigin(radius, radius);
            myvec[i].setPointCount(500);
            if (i) {
                myvec[i].setFillColor(sf::Color(100, 100, 100));
            } else {
                myvec[i].setFillColor(sf::Color(255, 0, 0));
            }
            //myvec[i].setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        }
    }

    std::vector<std::vector<sf::Vertex>> t;

    std::vector<std::vector<std::pair<double, double>>> trajec(def_obs);

    while (window.isOpen())
    {
        if(toggle) {
            auto m = b.get_molecules();
            const std::vector<Molecule>& v = m.get();
            for (int i = 0; i < v.size(); i++) {
                // moving the molecule
                myvec[i].setPosition(v[i].position.first, v[i].position.second);
            }
        }
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                exit(0);
            case sf::Event::MouseButtonPressed:
                if(event.key.code == sf::Mouse::Left) {
                    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    sf::FloatRect bounds = gui[0].sprite.getGlobalBounds();
                    if (bounds.contains(mouse))
                    {
                        std::swap(gui[0], gui[1]);
                        
                        if(toggle) {
                            b.pause();
                            toggle = false;
                        }else{
                            b.unpause();
                            toggle = true;
                        }
                    }
                }
                break;
            }
        }
        
        window.clear(sf::Color(255, 255, 255));
        for (int i = 0; i < myvec.size(); i++) {
            window.draw(myvec[i]);
        }
        
        window.draw(gui[0].sprite);

        // begin of the example
        // this example shows case when segments draws not in real time but only after collisions
        {
            // yes, you have to wrap you code in scope because of work with shared resource
            auto m = b.get_trajectory();
            const auto& t = m.get(); // const reference, otherwise it will be copied
            for (size_t i = 0; i < def_obs; i++) {
                if (!trajec[i].size() || trajec[i].back() != t[i]) {
                    trajec[i].push_back(t[i]);
                }
            }
        }

        for (size_t i = 0; i < def_obs; i++) {
            for (size_t j = 1; j < trajec[i].size(); j++) {
                sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(trajec[i][j - 1].first, trajec[i][j - 1].second), {100, 100, 100}),
                    sf::Vertex(sf::Vector2f(trajec[i][j].first, trajec[i][j].second), {100, 100, 100})
                };

                window.draw(line, 2, sf::Lines);
            }
        }
        // end of the example

        /*
        if (toggle) {
            auto m = b.get_molecules();
            const std::vector<Molecule>& v = m.get();
            for (int i = 0; i < v.size(); i++) {

                // show velocity vector
                sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(v[i].position.first, v[i].position.second), {100, 100, 100}),
                    sf::Vertex(sf::Vector2f(v[i].position.first + 0.1 * v[i].velocity.first,
                                            v[i].position.second + 0.1 * v[i].velocity.second), {100, 100, 100})
                };

                window.draw(line, 2, sf::Lines);
            }
        }*/
        window.display();
    }
}
