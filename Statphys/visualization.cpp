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


    sf::RenderWindow window(sf::VideoMode(1200, 600), "StatPhys");
    window.clear(sf::Color(255, 255, 255));
    Box b(5.0, { 5.0, 595, 5.0, 590.0 }, n_mol);
    std::vector<sf::CircleShape> myvec(n_mol, sf::CircleShape(10));

    sf::RectangleShape line1;
    line1.setSize(sf::Vector2f(3, 600));
    line1.setFillColor(sf::Color(100, 100, 100));
    line1.setPosition(600.0, 0.0);

    sf::RectangleShape line2;
    line2.setSize(sf::Vector2f(3, 600));
    line2.setFillColor(sf::Color(100, 100, 100));
    line2.setPosition(0.0, 0.0);

    sf::RectangleShape line3;
    line3.setSize(sf::Vector2f(600, 3));
    line3.setFillColor(sf::Color(100, 100, 100));
    line3.setPosition(0.0, 0.0);

    sf::RectangleShape line4;
    line4.setSize(sf::Vector2f(600, 3));
    line4.setFillColor(sf::Color(100, 100, 100));
    line4.setPosition(0.0, 597.0);

    std::vector<Button> gui;
    sf::Texture guiTexture;

    guiTexture.loadFromFile("text/iphone.jpg");
    gui.emplace_back(guiTexture, sf::Vector2f(800, 200), sf::IntRect(0, 0, 707, 370));

    {
        auto m = b.get_molecules();
        const std::vector<Molecule>& v = m.get();
        for (int i = 0; i < v.size(); i++) {
            myvec[i].setRadius(5);
            myvec[i].setPosition(v[i].position.first, v[i].position.second);
            myvec[i].setPointCount(500);
            myvec[i].setFillColor(sf::Color(100, 100, 100));
        }
    }

    while (window.isOpen())
    {
        {
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
            }
        }

        window.clear(sf::Color(255, 255, 255));
        for (int i = 0; i < myvec.size(); i++) {
            window.draw(myvec[i]);
        }
        window.draw(line1);
        window.draw(line2);
        window.draw(line3);
        window.draw(line4);
        for (int i = 0; i < gui.size(); i++) {
            window.draw(gui[i].sprite);
        }
        window.display();
    }
}