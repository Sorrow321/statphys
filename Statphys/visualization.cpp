#include "SFML/Graphics.hpp"
#include <iostream>
#include "box.cpp"


struct Button_menu
{
    sf::Vector2f bPosition;
    sf::IntRect bSprite;
    sf::Sprite sprite;
    sf::Font bFont;
    std::string bString;
    sf::Text bText;

    Button_menu(std::string bString, sf::Texture &texture, sf::Vector2f bPosition, sf::IntRect bSprite)
    : bString{ bString }, bPosition{ bPosition }, bSprite{ bSprite }
    {
        this->bFont.loadFromFile("text/Alice-Regular.ttf");
        this->bText.setFont(bFont);
        this->bText.setString(bString);
        this->bText.setColor(sf::Color::White);
        this->bText.setPosition(bPosition);
        this->sprite.setTexture(texture);
        this->sprite.setTextureRect(bSprite);
        this->sprite.setPosition(bPosition);
        this->sprite.setScale(1, 1);
    }
};

int main() {
    int main_window_state = 0;
    int amount_molecule = 100;
    float radius_molecule = 5;
    bool is_molecules_active = true;

    std::tuple<double, double, double, double> bounds = {5.0, 590.0, 5.0, 590.0};

    sf::RenderWindow main_window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height),
            "StatPhys");

    main_window.clear(sf::Color(255, 255, 255));
    Box molecule_box(radius_molecule, bounds, amount_molecule);
    std::vector<sf::CircleShape> molecules(amount_molecule, sf::CircleShape(radius_molecule));


    /*
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
    line4.setPosition(0.0, 597.0);*/

    std::vector<Button_menu> molecules_activation_button;

    sf::Texture molecules_activation_texture;
    sf::Texture demonstration_button_texture;


    demonstration_button_texture.loadFromFile("text/button.png");
    /* MAIN MENU BUTTONS(0) */
    Button_menu main_menu_demo("Demonstration", demonstration_button_texture,sf::Vector2f(
            sf::VideoMode::getDesktopMode().width * 0.5 - 350 * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 - 50 * 0.5),sf::IntRect(0, 0, 350, 50));
    Button_menu main_menu_theory("Theory", demonstration_button_texture, sf::Vector2f(
            sf::VideoMode::getDesktopMode().width * 0.5 - 350 * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 + 100 * 0.5), sf::IntRect(0, 0, 350, 50));
    Button_menu main_menu_exit("Exit", demonstration_button_texture, sf::Vector2f(
            sf::VideoMode::getDesktopMode().width * 0.5 - 350 * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 + 250 * 0.5), sf::IntRect(0, 0, 350, 50));

    /* DEMONSTATION(1) */
    // BUTTONS
    Button_menu demo_start_stop("Start", demonstration_button_texture, sf::Vector2f(800, 200), sf::IntRect(0, 0, 350, 50));
    Button_menu demo_back("Back to menu", demonstration_button_texture, sf::Vector2f(800, 300), sf::IntRect(0, 0, 350, 50));
    Button_menu demo_input("Input", demonstration_button_texture, sf::Vector2f(800, 400), sf::IntRect(0, 0, 350, 50));

    // INPUT VALUES
    int demo_regime_type = 1; //choose length - 1; choose amount of collisions - 2
    sf::Font global_font;
    global_font.loadFromFile("text/Alice-Regular.ttf");
    sf::Text demo_regime_text;
    demo_regime_text.setFont(global_font);
    demo_regime_text.setString("Regime type: " + std::to_string(demo_regime_type));
    demo_regime_text.setColor(sf::Color::Black);
    demo_regime_text.setPosition(sf::Vector2f(800, 450));

    float demo_length_or_collisions = 1;
    sf::Text demo_length_or_collisions_text;
    demo_length_or_collisions_text.setFont(global_font);
    std::string demo_length_or_collisions_string = "Length of trajectory: " + std::to_string(demo_length_or_collisions);
    demo_length_or_collisions_string = demo_length_or_collisions_string.erase(demo_length_or_collisions_string.find_last_not_of('0') + 1, std::string::npos);
    demo_length_or_collisions_text.setString(demo_length_or_collisions_string);
    demo_length_or_collisions_text.setColor(sf::Color::Black);
    demo_length_or_collisions_text.setPosition(sf::Vector2f(800, 500));

    sf::Text radius_size_text;
    radius_size_text.setFont(global_font);
    std::string radius_size_string = "Radius size: " + std::to_string(radius_molecule);
    radius_size_string = radius_size_string.erase(radius_size_string.find_last_not_of('0') + 1, std::string::npos);
    radius_size_text.setString(radius_size_string);
    radius_size_text.setColor(sf::Color::Black);
    radius_size_text.setPosition(sf::Vector2f(800, 550));

    sf::Text amount_molecules_text;
    amount_molecules_text.setFont(global_font);
    amount_molecules_text.setString("Molecules amount: " + std::to_string(amount_molecule));
    amount_molecules_text.setColor(sf::Color::Black);
    amount_molecules_text.setPosition(sf::Vector2f(800, 600));


    srand(100500);
    {
        auto m = molecule_box.get_molecules();
        //srand(1723210);
        const std::vector<Molecule> &v = m.get();
        for (int i = 0; i < v.size(); i++) {
            molecules[i].setRadius(radius_molecule);
            molecules[i].setPosition(v[i].position.first, v[i].position.second);
            molecules[i].setPointCount(100);
            molecules[i].setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        }
    }
    molecule_box.pause();
    is_molecules_active = false;

    while (main_window.isOpen()) {
        if (main_window_state == 0) {
            main_window.clear(sf::Color(255, 255, 255));
//            main_window.draw(sf::CircleShape(50, 30));
            main_window.draw(main_menu_demo.sprite);
            main_window.draw(main_menu_theory.sprite);
            main_window.draw(main_menu_exit.sprite);
            main_window.draw(main_menu_demo.bText);
            main_window.draw(main_menu_theory.bText);
            main_window.draw(main_menu_exit.bText);
            sf::Event event;
            while (main_window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        main_window.close();
                        exit(0);
                    case sf::Event::MouseButtonPressed:
                        if (event.key.code == sf::Mouse::Left) {
                            sf::Vector2f mouse = main_window.mapPixelToCoords(sf::Mouse::getPosition(main_window));
                            sf::FloatRect bounds_start = main_menu_demo.sprite.getGlobalBounds();
                            sf::FloatRect bounds_theory = main_menu_theory.sprite.getGlobalBounds();
                            sf::FloatRect bounds_exit = main_menu_exit.sprite.getGlobalBounds();
                            if (bounds_start.contains(mouse)) {
//                                std::swap(molecules_activation_button[0], molecules_activation_button[1]);
                                main_window_state = 1;
                            } else if (bounds_theory.contains(mouse)) {
                                main_window_state = 1;
                            } else if (bounds_exit.contains(mouse)) {
                                main_window.close();
                                exit(0);
                            }
                        }
                        break;
                }
            }

        } else if (main_window_state == 1) {
            if (is_molecules_active) {
                auto m = molecule_box.get_molecules();
                const std::vector<Molecule> &v = m.get();
                for (int i = 0; i < v.size(); i++) {
                    molecules[i].setPosition(v[i].position.first, v[i].position.second);
                }
            }
            sf::Event event;
            while (main_window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        main_window.close();
                        exit(0);
                    case sf::Event::MouseButtonPressed:
                        if (event.key.code == sf::Mouse::Left) {
                            sf::Vector2f mouse = main_window.mapPixelToCoords(sf::Mouse::getPosition(main_window));
                            sf::FloatRect bounds_start_stop = demo_start_stop.sprite.getGlobalBounds();
                            sf::FloatRect bounds_back_to_menu = demo_back.sprite.getGlobalBounds();
                            sf::FloatRect bounds_input = demo_input.sprite.getGlobalBounds();
                            if (bounds_start_stop.contains(mouse)) {
//                                std::swap(molecules_activation_button[0], molecules_activation_button[1]);
                                if (is_molecules_active) {
                                    molecule_box.pause();
                                    demo_start_stop.bText.setString("Start");
                                    is_molecules_active = false;
                                } else {
                                    molecule_box.unpause();
                                    demo_start_stop.bText.setString("Stop");

                                    is_molecules_active = true;
                                }
                            } else if (bounds_back_to_menu.contains(mouse)) {
                                main_window_state = 0;
                                if (is_molecules_active) {
                                    molecule_box.pause();
                                    is_molecules_active = false;
                                }
                            } else if (bounds_input.contains(mouse)) {
                                main_window_state = 1;
                                if (is_molecules_active) {
                                    molecule_box.pause();
                                    is_molecules_active = false;
                                }
                                
                            }
                        }
                        break;
                }
            }
            main_window.clear(sf::Color(255, 255, 255));
            for (int i = 0; i < molecules.size(); i++) {
                main_window.draw(molecules[i]);
            }
            main_window.draw(demo_start_stop.sprite);
            main_window.draw(demo_start_stop.bText);

            main_window.draw(demo_back.sprite);
            main_window.draw(demo_back.bText);

            main_window.draw(demo_input.sprite);
            main_window.draw(demo_input.bText);
            main_window.draw(demo_length_or_collisions_text);
            main_window.draw(demo_regime_text);
            main_window.draw(radius_size_text);
            main_window.draw(amount_molecules_text);
        }
        main_window.display();
    }
}

