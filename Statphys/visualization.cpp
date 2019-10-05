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
    int n_mol = 100;
    double radius = 5.0;

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
            myvec[i].setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        }
    }

    std::vector<std::vector<sf::Vertex>> t;

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
