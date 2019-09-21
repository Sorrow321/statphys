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

    std::tuple<double, double, double, double> bounds = { 5.0, 590.0, 5.0, 590.0 };
    
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Demonstration");
    window.clear(sf::Color(255, 255, 255));
    Box b(5.0, bounds, n_mol);
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
        //srand(1723210); 
        const std::vector<Molecule>& v = m.get();
        for (int i = 0; i < v.size(); i++) {
            myvec[i].setRadius(5);
            myvec[i].setPosition(v[i].position.first, v[i].position.second);
            myvec[i].setPointCount(500);
            myvec[i].setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        }
    }

    while (window.isOpen())
    {
        if(toggle) {
            auto m = b.get_molecules();
            const std::vector<Molecule>& v = m.get();
            for (int i = 0; i < v.size(); i++) {
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
                break;
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
        window.display();
    }
}
