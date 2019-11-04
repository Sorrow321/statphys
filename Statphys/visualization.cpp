#include "SFML/Graphics.hpp"
#include <iostream>
#include "box.cpp"
#include <cmath>

int mode_curr = mode_curr;

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
        this->bText.setFillColor(sf::Color::White);
        this->bText.setPosition(bPosition);
        this->sprite.setTexture(texture);
        this->sprite.setTextureRect(bSprite);
        this->sprite.setPosition(bPosition);
        this->sprite.setScale(1, 1);
    }
};

int main() {
    int main_window_state = 0;
    int amount_molecule = def_molnum;
    float radius_molecule = def_radius;
    bool is_molecules_active = true;

    std::tuple<double, double, double, double> bounds = {5.0, sf::VideoMode::getDesktopMode().width, 5.0, sf::VideoMode::getDesktopMode().height/2};
    //fprintf(stderr, "%d, %d", sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height/2);
    sf::RenderWindow main_window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height),
            "StatPhys");


    main_window.clear(sf::Color(255, 255, 255));
    Box *molecule_box = new Box(radius_molecule, bounds, amount_molecule);
    std::vector<sf::CircleShape> molecules(amount_molecule, sf::CircleShape(radius_molecule));
    //fprintf(stderr, "1");

    // VISUAL BORDERS for molecules box
    sf::RectangleShape border1;
    border1.setSize(sf::Vector2f(3, sf::VideoMode::getDesktopMode().height/2));
    border1.setFillColor(sf::Color(100, 100, 100));
    border1.setPosition(sf::VideoMode::getDesktopMode().width, 0.0);

    sf::RectangleShape border2;
    border2.setSize(sf::Vector2f(3, sf::VideoMode::getDesktopMode().height/2));
    border2.setFillColor(sf::Color(100, 100, 100));
    border2.setPosition(0.0, 0.0);

    sf::RectangleShape border3;
    border3.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 3));
    border3.setFillColor(sf::Color(100, 100, 100));
    border3.setPosition(0.0, 0.0);

    sf::RectangleShape border4;
    border4.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 3));
    border4.setFillColor(sf::Color(100, 100, 100));
    border4.setPosition(0.0, sf::VideoMode::getDesktopMode().height/2);



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
    Button_menu main_menu_authors("Authors", demonstration_button_texture, sf::Vector2f(
            sf::VideoMode::getDesktopMode().width * 0.5 - 350 * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 + 250 * 0.5), sf::IntRect(0, 0, 350, 50));
    Button_menu main_menu_exit("Exit", demonstration_button_texture, sf::Vector2f(
            sf::VideoMode::getDesktopMode().width * 0.5 - 350 * 0.5,
            sf::VideoMode::getDesktopMode().height * 0.5 + 400 * 0.5), sf::IntRect(0, 0, 350, 50));

    /* DEMONSTATION(1) */
    // BUTTONS
    int button_menu_demo_width = 350;
    int button_menu_demo_height = 50;
    Button_menu demo_start_stop("Start", demonstration_button_texture, sf::Vector2f(sf::VideoMode::getDesktopMode().width - button_menu_demo_width, sf::VideoMode::getDesktopMode().height - 10*button_menu_demo_height),
            sf::IntRect(0, 0, button_menu_demo_width, button_menu_demo_height));
    Button_menu demo_back("Back to menu", demonstration_button_texture, sf::Vector2f(sf::VideoMode::getDesktopMode().width - button_menu_demo_width, sf::VideoMode::getDesktopMode().height - 10*button_menu_demo_height + 100),
            sf::IntRect(0, 0, button_menu_demo_width, button_menu_demo_height));
    Button_menu demo_input("Input", demonstration_button_texture, sf::Vector2f(sf::VideoMode::getDesktopMode().width - button_menu_demo_width, sf::VideoMode::getDesktopMode().height - 10*button_menu_demo_height + 200),
            sf::IntRect(0, 0, button_menu_demo_width, button_menu_demo_height));
    Button_menu demo_clear("Check trajectories", demonstration_button_texture, sf::Vector2f(sf::VideoMode::getDesktopMode().width - button_menu_demo_width, sf::VideoMode::getDesktopMode().height - 10*button_menu_demo_height + 300),
                           sf::IntRect(0, 0, button_menu_demo_width, button_menu_demo_height));
    bool is_clear_box = false;

    // INPUT VALUES
    sf::Font global_font;
    global_font.loadFromFile("text/Alice-Regular.ttf");

    sf::Text demo_regime_type_text;
    demo_regime_type_text.setFont(global_font);
    demo_regime_type_text.setString("Regime type: " + std::to_string(mode_curr));
    demo_regime_type_text.setFillColor(sf::Color::Black);
    demo_regime_type_text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - 2*button_menu_demo_width, demo_start_stop.bPosition.y));

    //    int mode_curr = 1; //choose length - 1; choose amount of collisions - 2
    sf::Text demo_statistics_type_text;
    demo_statistics_type_text.setFont(global_font);
    demo_statistics_type_text.setString("Statistics type: " + std::to_string(mode_curr));
    demo_statistics_type_text.setFillColor(sf::Color::Black);
    demo_statistics_type_text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - 2*button_menu_demo_width, demo_start_stop.bPosition.y + 50));

    float demo_length_or_collisions = int(def_interactions_num);
    sf::Text demo_length_or_collisions_text;
    demo_length_or_collisions_text.setFont(global_font);
    std::string demo_length_or_collisions_string = "Length of trajectory: " + std::to_string(demo_length_or_collisions);
    if (mode_curr == 2) {
        demo_length_or_collisions_string = "Interactions amount: " + std::to_string(demo_length_or_collisions);
    }
    demo_length_or_collisions_string = demo_length_or_collisions_string.erase(demo_length_or_collisions_string.find_last_not_of('0') + 1, std::string::npos);
    demo_length_or_collisions_text.setString(demo_length_or_collisions_string);
    demo_length_or_collisions_text.setFillColor(sf::Color::Black);
    demo_length_or_collisions_text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - 2*button_menu_demo_width, demo_start_stop.bPosition.y + 100));

    sf::Text radius_size_text;
    radius_size_text.setFont(global_font);
    radius_molecule = def_radius;
    std::string radius_size_string = "Radius size: " + std::to_string(radius_molecule);
    radius_size_string = radius_size_string.erase(radius_size_string.find_last_not_of('0') + 1, std::string::npos);
    radius_size_text.setString(radius_size_string);
    radius_size_text.setFillColor(sf::Color::Black);
    radius_size_text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - 2*button_menu_demo_width, demo_start_stop.bPosition.y + 150));

    sf::Text amount_molecules_text;
    amount_molecules_text.setFont(global_font);
    amount_molecule = def_molnum;
    std::string amount_molecules_string = std::to_string(amount_molecule);
    amount_molecules_text.setString("Molecules amount: " + std::to_string(amount_molecule));
    amount_molecules_text.setFillColor(sf::Color::Black);
    amount_molecules_text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - 2*button_menu_demo_width, demo_start_stop.bPosition.y + 200));
    // INPUT VALUES END

    // VISUAL BORDERS for graphs
    sf::RectangleShape border5;
    border5.setSize(sf::Vector2f(3, sf::VideoMode::getDesktopMode().height / 2.0));
    border5.setFillColor(sf::Color(100, 100, 100));
    border5.setPosition(amount_molecules_text.getPosition().x, sf::VideoMode::getDesktopMode().height / 2.0);

    //TRAJECTORY LINES:
    std::vector<std::vector<sf::Vertex>> trajectories(def_obs);
    std::vector<std::vector<sf::Color>> colors(def_obs);
    // ТОЧКИ ИЗЛОМА
    std::vector<std::vector<sf::CircleShape>> break_points(def_obs);

    for (int i = 0; i < def_obs; i++) {
        colors[i].emplace_back(sf::Color::Blue);
        colors[i].emplace_back(sf::Color::Red);
    }


    // INTERACTIONS AMOUNT BLOCK(HISTOGRAM)
    sf::Text molecules_interactions_text;
    molecules_interactions_text.setFont(global_font);
    molecules_interactions_text.setString("Molecules interactions amount: ");
    molecules_interactions_text.setFillColor(sf::Color::Black);
    molecules_interactions_text.setPosition(sf::Vector2f(800, 450));


    // HISTOGRAM VISUALIZATION:
    int histogram_bins = 40;
    constexpr int histogram_norm_const = 1000;
    double trajectory_max_len = 0;
    double trajectory_min_len = 100000000;

    double collision_max_amount = 0;
    double collision_min_amount = 1000000000;

    float full_counts_max = 0;
    bool statistics_collected = false;

    std::vector<int> histogram_demo_counts(histogram_bins);
    std::vector<float> trajectory_lens(histogram_bins);
    std::vector<float> collision_amounts(histogram_bins);
    std::vector<sf::RectangleShape> histogram_demo(histogram_bins);
//    border4.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 3));
//    border4.setFillColor(sf::Color(100, 100, 100));
//    border4.setPosition(0.0, sf::VideoMode::getDesktopMode().height/2);
    std::vector<sf::RectangleShape> histogram_axes_ticks(histogram_bins);
    std::vector<sf::Text> histogram_axes_text(histogram_bins);


    //AUTHORS
    sf::Texture cmc_logo_texture;
    sf::Sprite cmc_logo_sprite;
    cmc_logo_texture.loadFromFile("text/cmc_logo.png");
    cmc_logo_texture.setSmooth(true);
    cmc_logo_sprite.setTexture(cmc_logo_texture);
    cmc_logo_sprite.setPosition(sf::Vector2f(0, 50));
    cmc_logo_sprite.setScale(1, 1);
    
    sf::Texture fiz_logo_texture;
    sf::Sprite fiz_logo_sprite;
    fiz_logo_texture.loadFromFile("text/fiz_logo.jpg");
    fiz_logo_texture.setSmooth(true);
    fiz_logo_sprite.setTexture(fiz_logo_texture);
    fiz_logo_sprite.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - fiz_logo_texture.getSize().x,  50));
    fiz_logo_sprite.setScale(1, 1);

    sf::Text cmc_logo_text;
    cmc_logo_text.setFont(global_font);
    std::string cmc_logo_string = "Faculty of Computational mathematics and cybernetics";
    cmc_logo_text.setString(cmc_logo_string);
    cmc_logo_text.setFillColor(sf::Color::Black);
    cmc_logo_text.setPosition(sf::Vector2f(cmc_logo_sprite.getGlobalBounds().left,  cmc_logo_sprite.getGlobalBounds().height + 50));

    sf::Text fiz_logo_text;
    fiz_logo_text.setFont(global_font);
    std::string fiz_logo_string = "Faculty of physics";
    fiz_logo_text.setString(fiz_logo_string);
    fiz_logo_text.setFillColor(sf::Color::Black);
    fiz_logo_text.setPosition(sf::Vector2f(fiz_logo_sprite.getGlobalBounds().left + fiz_logo_sprite.getLocalBounds().width * 0.5 - fiz_logo_text.getLocalBounds().width * 0.5,
            fiz_logo_sprite.getGlobalBounds().height + 50));
    
    sf::Text teacher_text;
    teacher_text.setFont(global_font);
    std::string teacher_string = "Nauchniy ruk. : docent Chichigina Olga Aleksandrovna\n"
                                 "Lector: professor ??????????????";
    teacher_text.setString(teacher_string);
    teacher_text.setFillColor(sf::Color::Black);
    teacher_text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width * 0.5 - teacher_text.getLocalBounds().width * 0.5,  cmc_logo_sprite.getGlobalBounds().height + 125));
    
    sf::Texture nikita_photo_texture;
    sf::Sprite nikita_photo_sprite;
    nikita_photo_texture.loadFromFile("text/Nikita.jpg");
    nikita_photo_texture.setSmooth(true);
    nikita_photo_sprite.setTexture(nikita_photo_texture);
    nikita_photo_sprite.setPosition(sf::Vector2f(cmc_logo_text.getGlobalBounds().left,  cmc_logo_sprite.getGlobalBounds().height + 225));
    nikita_photo_sprite.setScale(1, 1);

    sf::Text nikita_text;
    nikita_text.setFont(global_font);
    std::string nikita_string = "Nikita Kuzmin";
    nikita_text.setString(nikita_string);
    nikita_text.setFillColor(sf::Color::Black);
    nikita_text.setPosition(sf::Vector2f(nikita_photo_sprite.getGlobalBounds().left + nikita_photo_sprite.getGlobalBounds().width * 0.5 - nikita_text.getLocalBounds().width * 0.5,
            nikita_photo_sprite.getPosition().y + nikita_photo_sprite.getLocalBounds().height + 5));

    sf::Texture ilya_photo_texture;
    sf::Sprite ilya_photo_sprite;
    ilya_photo_texture.loadFromFile("text/Ilya.jpg");
    ilya_photo_texture.setSmooth(true);
    ilya_photo_sprite.setTexture(ilya_photo_texture);
    ilya_photo_sprite.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width * 0.5 - ilya_photo_sprite.getLocalBounds().width * 0.5,  cmc_logo_sprite.getGlobalBounds().height + 225));
    ilya_photo_sprite.setScale(1, 1);

    sf::Text ilya_text;
    ilya_text.setFont(global_font);
    std::string ilya_string = "Ilya Fedorov";
    ilya_text.setString(ilya_string);
    ilya_text.setFillColor(sf::Color::Black);
    ilya_text.setPosition(sf::Vector2f(sf::Vector2f(ilya_photo_sprite.getGlobalBounds().left + ilya_photo_sprite.getGlobalBounds().width * 0.5 - ilya_text.getLocalBounds().width * 0.5,
            ilya_photo_sprite.getPosition().y + ilya_photo_sprite.getLocalBounds().height + 5)));


    sf::Texture dima_photo_texture;
    sf::Sprite dima_photo_sprite;
    dima_photo_texture.loadFromFile("text/Dima.jpg");
    dima_photo_texture.setSmooth(true);
    dima_photo_sprite.setTexture(dima_photo_texture);
    dima_photo_sprite.setPosition(sf::Vector2f(fiz_logo_sprite.getGlobalBounds().left + fiz_logo_sprite.getLocalBounds().width * 0.5 - dima_photo_sprite.getLocalBounds().width * 0.5,
            cmc_logo_sprite.getGlobalBounds().height + 225));
    dima_photo_sprite.setScale(1, 1);

    sf::Text dima_text;
    dima_text.setFont(global_font);
    std::string dima_string = "Dmitriy Poymanov";
    dima_text.setString(dima_string);
    dima_text.setFillColor(sf::Color::Black);
    dima_text.setPosition(sf::Vector2f(dima_photo_sprite.getGlobalBounds().left + dima_photo_sprite.getGlobalBounds().width * 0.5 - dima_text.getLocalBounds().width * 0.5,
            dima_photo_sprite.getPosition().y + dima_photo_sprite.getLocalBounds().height + 5));

    sf::Text authors_title_text1;
    authors_title_text1.setFont(global_font);
    std::string authors_title_string1 = "Moscow state university\n ............\n........................\n...........";
    authors_title_text1.setString(authors_title_string1);
    authors_title_text1.setFillColor(sf::Color::Black);
    authors_title_text1.setPosition(sf::VideoMode::getDesktopMode().width * 0.5 - authors_title_text1.getLocalBounds().width * 0.5,
                                    authors_title_text1.getGlobalBounds().height * 0.5);

    sf::Text authors_title_text2;
    authors_title_text2.setFont(global_font);
    std::string authors_title_string2 = "Authors:";
    authors_title_text2.setString(authors_title_string2);
    authors_title_text2.setFillColor(sf::Color::Black);
    authors_title_text2.setPosition(sf::VideoMode::getDesktopMode().width * 0.5 - authors_title_text2.getLocalBounds().width * 0.5,
                                    cmc_logo_sprite.getGlobalBounds().height + 190);
    



    for (int i = 0; i < histogram_bins; i++) {
        histogram_demo_counts[i] = 0;
        trajectory_lens[i] = (i + 1)  * fabs(trajectory_max_len - trajectory_min_len) / histogram_bins;
        collision_amounts[i] = i * fabs((collision_max_amount - collision_min_amount)) / histogram_bins;
        histogram_demo[i].setSize(sf::Vector2f(histogram_norm_const / histogram_bins, 0));
        histogram_demo[i].setFillColor(sf::Color(rand() % 200, rand() % 200, rand() % 200));
        histogram_demo[i].setPosition(
                    i * histogram_demo[i].getSize().x,
                    sf::VideoMode::getDesktopMode().height - 150);
        histogram_demo[i].setOrigin(histogram_demo[i].getLocalBounds().left,
                                     histogram_demo[i].getSize().y);
        if ((i % 10) == 0) {
            histogram_axes_text[i].setFont(global_font);
            if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i])));
                                }
            histogram_axes_text[i].setFillColor(sf::Color::Black);
            histogram_axes_text[i].setPosition(i * histogram_demo[i].getSize().x,
                                               sf::VideoMode::getDesktopMode().height - 155);
            histogram_axes_text[i].setOrigin(histogram_demo[i].getLocalBounds().left,
                                             histogram_demo[i].getSize().y);
        }
    }





    // DEMONSTRATION(1) END

    srand(100500);
    {
        auto m = molecule_box->get_molecules();
        //srand(1723210);
        const std::vector<Molecule> &v = m.get();
        for (int i = 0; i < v.size(); i++) {
            molecules[i].setRadius(radius_molecule);
            molecules[i].setPosition(v[i].position.first, v[i].position.second);
            molecules[i].setOrigin(radius_molecule, radius_molecule);
            molecules[i].setPointCount(100);
            molecules[i].setFillColor(sf::Color(rand() % 220, rand() % 220, rand() % 220));
        }
    }
    molecule_box->pause();
    is_molecules_active = false;

    int enter_press_amount = 0;
    int demo_parameters_amount = 5;
    bool was_first_key_press = false;
    while (main_window.isOpen()) {
        if (main_window_state == 0) {
            main_window.clear(sf::Color(255, 255, 255));
//            main_window.draw(sf::CircleShape(50, 30));
            main_window.draw(main_menu_demo.sprite);
            main_window.draw(main_menu_theory.sprite);
            main_window.draw(main_menu_authors.sprite);
            main_window.draw(main_menu_exit.sprite);
            main_window.draw(main_menu_demo.bText);
            main_window.draw(main_menu_authors.bText);
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
                            sf::FloatRect bounds_authors = main_menu_authors.sprite.getGlobalBounds();
                            sf::FloatRect bounds_exit = main_menu_exit.sprite.getGlobalBounds();
                            if (bounds_start.contains(mouse)) {
//                                std::swap(molecules_activation_button[0], molecules_activation_button[1]);
                                main_window_state = 1;
                            } else if (bounds_theory.contains(mouse)) {
                                main_window_state = 1;
                            } else if (bounds_authors.contains(mouse)) {
                                main_window_state = 3; // Authors
                            }
                            else if (bounds_exit.contains(mouse)) {
                                main_window.close();
                                exit(0);
                            }
                        }
                        break;
                }
            }

        } else if (main_window_state == 1) {  // DEMONSTATION WINDOW with molecules
            if (is_molecules_active) {
                auto m = molecule_box->get_molecules();
                const std::vector<Molecule> &v = m.get();
                for (int i = 0; i < v.size(); i++) {
                    molecules[i].setPosition(v[i].position.first, v[i].position.second);
                }
                for(int i=0; i < def_obs; i++) {
                    if (mode_curr == 1) {
                        if (molecule_box->get_interacted(i)) {
//                            molecule_box->set_interacted(i, false);
                            float break_point_radius = 2.5;
                            sf::CircleShape tmp_point;
                            tmp_point.setRadius(break_point_radius);
                            tmp_point.setPosition(molecules[i].getPosition());
                            tmp_point.setOrigin(break_point_radius, break_point_radius);
                            tmp_point.setPointCount(100);
                            tmp_point.setFillColor(sf::Color(0, 0, 0));
                            break_points[i].push_back(tmp_point);
//                            std::swap(colors[i][0], colors[i][1]);
                        }
                    } else {
                        if (molecule_box->get_finished(i)) {
//                            molecule_box->set_finished(i, false);
                            std::swap(colors[i][0], colors[i][1]);
                        }
                    }
                    trajectories[i].emplace_back(molecules[i].getPosition(), colors[i][0]);
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
                            sf::FloatRect bounds_clear = demo_clear.sprite.getGlobalBounds();
                            if (bounds_start_stop.contains(mouse)) {
//                                std::swap(molecules_activation_button[0], molecules_activation_button[1]);
                                if (is_molecules_active) {
                                    molecule_box->pause();
                                    demo_start_stop.bText.setString("Start");
                                    is_molecules_active = false;
                                } else {
                                    molecule_box->unpause();
                                    demo_start_stop.bText.setString("Stop");

                                    is_molecules_active = true;
                                }
                                is_clear_box = false;
                            } else if (bounds_back_to_menu.contains(mouse)) {
                                main_window_state = 0;
                                if (is_molecules_active) {
                                    molecule_box->pause();
                                    is_molecules_active = false;
                                }
                            } else if (bounds_input.contains(mouse)) {
                                if (is_molecules_active) {
                                    demo_start_stop.bText.setString("Start");
                                    molecule_box->pause();
                                    is_molecules_active = false;
                                }
                                main_window_state = 2; // PARAMETERS INPUT
                            } else if (bounds_clear.contains(mouse)) {
                                if (is_molecules_active) {
                                    molecule_box->pause();
                                    is_molecules_active = false;
                                }
                                demo_start_stop.bText.setString("Start");
                                is_clear_box = true;
                            }
                        }
                        break;
                }
            }
            main_window.clear(sf::Color(255, 255, 255));
            if (!is_clear_box) {
                for (int i = 0; i < molecules.size(); i++) {
                    main_window.draw(molecules[i]);
                }
            }
//            molecule_box

            //INTERACTIONS AMOUNT
//            molecules_interactions_text.setString();


            main_window.draw(demo_start_stop.sprite);
            main_window.draw(demo_start_stop.bText);

            main_window.draw(demo_back.sprite);
            main_window.draw(demo_back.bText);

            main_window.draw(demo_input.sprite);
            main_window.draw(demo_input.bText);

            main_window.draw(demo_clear.sprite);
            main_window.draw(demo_clear.bText);

            main_window.draw(demo_regime_type_text);
            main_window.draw(demo_length_or_collisions_text);
            main_window.draw(demo_statistics_type_text);
            main_window.draw(radius_size_text);
            main_window.draw(amount_molecules_text);

            //BORDERS
            main_window.draw(border1);
            main_window.draw(border2);
            main_window.draw(border3);
            main_window.draw(border4);
            main_window.draw(border5);

            //точки излома:
            if (mode_curr == 1) {
                for (int i = 0; i < def_obs; i++) {
                    for (int j = 0; j < break_points[i].size(); j++) {
                        main_window.draw(break_points[i][j]);
                    }
                }
            }
            for (int i = 0; i < def_obs; i++) {
                main_window.draw(trajectories[i].data(), trajectories[0].size(), sf::PrimitiveType::LineStrip);
            }

            //HISTOGRAM
//            get_la
            if (mode_curr == 1) {
                double last_amount = molecule_box->get_last_interactions_num();
                if (last_amount != -1) {
//                    std::cout << last_amount << std::endl;
                    //fprintf(stderr, "Last amount max: %f\n", collision_max_amount);
                    if (collision_max_amount < last_amount) {
                        collision_max_amount = last_amount;
//                        histogram_bins = int(collision_max_amount);
                        for (int i = 0; i < histogram_bins; i++) {
                            histogram_demo_counts[i] = 0;
                            full_counts_max = 0;
                            collision_amounts[i] = i * (collision_max_amount - collision_min_amount) / histogram_bins;
                            if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i])));
                                }
                            //fprintf(stderr, "%d\n", collision_amounts[i]);
                        }
                    }
                    if (collision_min_amount > last_amount) {
                        collision_min_amount = last_amount;
                        for (int i = 0; i < histogram_bins; i++) {
                            histogram_demo_counts[i] = 0;
                            full_counts_max = 0;
                            collision_amounts[i] = i * (collision_max_amount - collision_min_amount) / histogram_bins;
                            //                    //fprintf(stderr, "%d\n", collision_amounts[i]);
                            if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i])));
                                }
                        }
                    }
                    //fprintf(stderr, "Last_amount: %d\n", last_amount);
                    for (int i = 0; i < (histogram_bins - 1); i++) {
                        //fprintf(stderr, "collision_%d - %f \n", i, collision_amounts[i]);
                        if ((last_amount >= collision_amounts[i]) && (last_amount < collision_amounts[i + 1])) {
                            histogram_demo_counts[i] += 1;
                            if (full_counts_max < histogram_demo_counts[i]) {
                                full_counts_max = histogram_demo_counts[i];
                            }
                        }
                        histogram_demo[i].setSize(
                                sf::Vector2f(histogram_norm_const / histogram_bins,
                                             histogram_demo_counts[i] * 350.0 / full_counts_max)
                        );
                        histogram_demo[i].setOrigin(histogram_demo[i].getLocalBounds().left,
                                                    histogram_demo[i].getSize().y);
                        if (statistics_collected) {
                            main_window.draw(histogram_demo[i]);
                            main_window.draw(histogram_axes_text[i]);
                        }

                        if ((full_counts_max >= 1) &&
                            (!statistics_collected)) { // после набора статистики (чтобы узнать макс длину траектории)
                            full_counts_max = 0;
                            statistics_collected = true;
                            for (int j = 0; j < histogram_bins; j++) {
                                histogram_demo_counts[j] = 0;
                                collision_amounts[j] = j * (collision_max_amount - collision_min_amount) / histogram_bins;
                                if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i])));
                                }
                            }
                        }
                    }
                } else {
                    for (int i=0; i < histogram_bins; i++) {
                        if (statistics_collected) {
                            main_window.draw(histogram_demo[i]);
                            main_window.draw(histogram_axes_text[i]);
                        }
                    }
                }
            }
            else {
                double last_len = molecule_box->get_last_len();
                if (last_len != -1) {
                    //fprintf(stderr, "Trajectory max len: %f\n", trajectory_max_len);
                    if (trajectory_max_len < last_len) {
                        trajectory_max_len = last_len;
                        for (int i = 0; i < histogram_bins; i++) {
                            histogram_demo_counts[i] = 0;
                            full_counts_max = 0;
                            trajectory_lens[i] = (i + 1) * (trajectory_max_len - trajectory_min_len) / histogram_bins;
                            if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i])));
                                }
                            //fprintf(stderr, "%d\n", trajectory_lens[i]);
                        }
                    }
                    if (trajectory_min_len > last_len) {
                        trajectory_min_len = last_len;
                        for (int i = 0; i < histogram_bins; i++) {
                            histogram_demo_counts[i] = 0;
                            full_counts_max = 0;
                            trajectory_lens[i] = (i + 1) * (trajectory_max_len - trajectory_min_len) / histogram_bins;
                            if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i])));
                                }
                            //                    //fprintf(stderr, "%d\n", trajectory_lens[i]);
                        }
                    }
                    //fprintf(stderr, "Last_len: %f\n", last_len);
                    for (int i = 0; i < (histogram_bins - 1); i++) {
                        //fprintf(stderr, "trajectory_%d - %f \n", i, trajectory_lens[i]);
                        if ((last_len >= trajectory_lens[i]) && (last_len < trajectory_lens[i + 1])) {
                            histogram_demo_counts[i] += 1;
                            if (full_counts_max < histogram_demo_counts[i]) {
                                full_counts_max = histogram_demo_counts[i];
                            }
                        }
                        histogram_demo[i].setSize(
                                sf::Vector2f(histogram_norm_const / histogram_bins,
                                             histogram_demo_counts[i] *  350.0 / full_counts_max)
                        );
                        histogram_demo[i].setOrigin(histogram_demo[i].getLocalBounds().left,
                                                    histogram_demo[i].getSize().y);
                        if (statistics_collected) {
                            main_window.draw(histogram_demo[i]);
                            main_window.draw(histogram_axes_text[i]);
                        }

                        if ((full_counts_max >= 1) &&
                            (!statistics_collected)) { // после набора статистики (чтобы узнать макс длину траектории)
                            full_counts_max = 0;
                            statistics_collected = true;
                            for (int j = 0; j < histogram_bins; j++) {
                                histogram_demo_counts[j] = 0;
                                trajectory_lens[j] = j * (trajectory_max_len - trajectory_min_len) / histogram_bins;
                                if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i])));
                                }
                            }
                        }
                    }
                } else {
                    for (int i=0; i < histogram_bins; i++) {
                        if (statistics_collected) {
                            main_window.draw(histogram_demo[i]);
                            main_window.draw(histogram_axes_text[i]);
                        }
                    }
                }
            }
        } else if (main_window_state == 2) { // INPUT
            std::vector<std::string>  input_button_strings = {"Enter regime type",
                                                              "Enter statistics type: ",
                                                              "Enter length of trajectory: ",
                                                              "Enter radius size: ",
                                                              "Enter molecules amount: "};

            demo_input.bText.setString(input_button_strings[enter_press_amount]);
            bool is_all_params_were_passed = false;
                switch (enter_press_amount) {
                    case 0:
                        demo_regime_type_text.setFillColor(sf::Color::Red);
                        break;
                    case 1:
                        demo_statistics_type_text.setFillColor(sf::Color::Red);
                        break;
                    case 2:
                        demo_length_or_collisions_text.setFillColor(sf::Color::Red);
                        break;
                    case 3:
                        radius_size_text.setFillColor(sf::Color::Red);
                        break;
                    case 4:
                        amount_molecules_text.setFillColor(sf::Color::Red);
                        break;
                }
//            //fprintf(stderr, "%d", was_first_key_press);
            if (!was_first_key_press) {
                if (enter_press_amount < (demo_parameters_amount - 3)) {
                    demo_length_or_collisions_string = std::to_string(demo_length_or_collisions);
                }
                if (enter_press_amount < (demo_parameters_amount - 2)) {
                    radius_size_string = std::to_string(radius_molecule);
                }
                if (enter_press_amount < (demo_parameters_amount - 1)) {
                    amount_molecules_string = std::to_string(amount_molecule);
                }
            }
            sf::Event event;
            while ((main_window.pollEvent(event)) && (not is_all_params_were_passed)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        main_window.close();
                        exit(0);
                    case sf::Event::TextEntered:
                        if (enter_press_amount == 0) {
                            if (event.key.code != 13) {
                                if (!was_first_key_press) {
                                    was_first_key_press = true;
//                                    mode_curr = 0;
                                }
                            }
                            if (event.key.code == 49) {
                                demo_regime_type_text.setString(
                                        "Regime type: " + std::to_string(event.text.unicode - 48));
                                mode_curr = 1;
                            } else if (event.key.code == 50) {
                                demo_regime_type_text.setString(
                                        "Regime type: " + std::to_string(event.text.unicode - 48));
                                mode_curr = 2;
                            } else if (event.key.code == 13) {
                                was_first_key_press = false;
                                demo_regime_type_text.setFillColor(sf::Color::Black);
//                                if (mode_curr == 1) {
//                                    demo_length_or_collisions_text.setString("Length of trajectory: 1");
//                                } else if (mode_curr == 2) {
//                                    demo_length_or_collisions_text.setString("Interactions amount: 1");
//                                }
//                                main_window.draw(demo_length_or_collisions_text);
                                enter_press_amount += 1;
                            }
                        } else if (enter_press_amount == 1) {
                            if (event.key.code != 13) {
                                if (!was_first_key_press) {
                                    was_first_key_press = true;
//                                    mode_curr = 0;
                                }
                            }
                            if (event.key.code == 49) {
                                demo_statistics_type_text.setString("Statistics type: " + std::to_string(event.text.unicode - 48));
                                mode_curr = 1;
                            } else if (event.key.code == 50) {
                                demo_statistics_type_text.setString("Statistics type: " + std::to_string(event.text.unicode - 48));
                                mode_curr = 2;
                            } else if (event.key.code == 13) {
                                was_first_key_press = false;
                                demo_statistics_type_text.setFillColor(sf::Color::Black);
                                if (mode_curr == 1) {
                                    demo_length_or_collisions_text.setString("Length of trajectory: 1");
                                } else if (mode_curr == 2) {
                                    demo_length_or_collisions_text.setString("Interactions amount: 1");
                                }
                                main_window.draw(demo_length_or_collisions_text);
                                enter_press_amount += 1;
                            }
                        } else if (enter_press_amount == 2) {
                            if (event.key.code != 13) {
                                if (!was_first_key_press) {
                                    was_first_key_press = true;
                                    demo_length_or_collisions_string = "";
                                }
                            }
                            demo_length_or_collisions_text.setFillColor(sf::Color::Red);
                            if (mode_curr == 1) {
                                if ((event.key.code >= 48) && (event.key.code < 58)) {
                                    if (demo_length_or_collisions_string.size() <= 3) {
                                        demo_length_or_collisions_string += std::to_string(event.text.unicode - 48);
                                    }
                                    demo_length_or_collisions_text.setString(
                                            "Length of trajectory: " + demo_length_or_collisions_string);
                                } else if (event.key.code == 8) {
                                    if (demo_length_or_collisions_string.length() != 0) {
                                        demo_length_or_collisions_string.pop_back();
                                    }
                                    demo_length_or_collisions_text.setString(
                                            "Length of trajectory: " + demo_length_or_collisions_string);
                                } else if (event.key.code == 13) {
                                    demo_length_or_collisions = std::stof(demo_length_or_collisions_string);
                                    was_first_key_press = false;
                                    demo_length_or_collisions_text.setFillColor(sf::Color::Black);
                                    enter_press_amount += 1;
                                }
                            } else if (mode_curr == 2) {
                                if (event.key.code != 13) {
                                    if (!was_first_key_press) {
                                        was_first_key_press = true;
                                        demo_length_or_collisions_string = "";
                                    }
                                }
                                demo_length_or_collisions_text.setFillColor(sf::Color::Red);
                                if (((event.key.code >= 48) && (event.key.code < 58)) || (event.key.code == 46)) {
                                    demo_length_or_collisions_string += static_cast<char>(event.text.unicode);
                                    demo_length_or_collisions_text.setString("Interactions amount: " + demo_length_or_collisions_string);
                                } else if (event.key.code == 8) {
                                    if (demo_length_or_collisions_string.length() != 0) {
                                        demo_length_or_collisions_string.pop_back();
                                    }
                                    demo_length_or_collisions_text.setString(
                                            "Interactions amount: " + demo_length_or_collisions_string);
                                } else if (event.key.code == 13) {
                                    demo_length_or_collisions = std::stof(demo_length_or_collisions_string);
                                    was_first_key_press = false;
                                    demo_length_or_collisions_text.setFillColor(sf::Color::Black);
                                    enter_press_amount += 1;
                                }
                            }
                        } else if (enter_press_amount == 3) {
                            if (event.key.code != 13) {
                                if (!was_first_key_press) {
                                    was_first_key_press = true;
                                    radius_size_string = "";
                                }
                            }
                            radius_size_text.setFillColor(sf::Color::Red);
                            if (((event.key.code >= 48) && (event.key.code < 58)) || (event.key.code == 46)) {
                                radius_size_string += static_cast<char>(event.text.unicode);
                                if (std::stod(radius_size_string) > 20) {
                                    radius_size_string = "20.0";
                                } else if (std::stod(radius_size_string) < 1) {
                                    radius_size_string =  "1";
                                }
                                if (radius_size_string.size() > 4) {
                                    radius_size_string.pop_back();
                                }
                                radius_size_text.setString("Radius size: " + radius_size_string);
                            } else if (event.key.code == 8) {
                                if (radius_size_string.length() != 0) {
                                    radius_size_string.pop_back();
                                }
                                radius_size_text.setString(
                                        "Radius size: " + radius_size_string);
                            } else if (event.key.code == 13) {
                                radius_molecule = std::stod(radius_size_string);
                                was_first_key_press = false;
                                radius_size_text.setFillColor(sf::Color::Black);
                                enter_press_amount += 1;
                            }
                        } else if (enter_press_amount == 4) {
                            if (event.key.code != 13) {
                                if (!was_first_key_press) {
                                    was_first_key_press = true;
                                    amount_molecules_string = "";
                                }
                            }
                            amount_molecules_text.setFillColor(sf::Color::Red);
                            if ((event.key.code >= 48) && (event.key.code < 58)) {
                                if (amount_molecules_string.size() < 3) {
                                    amount_molecules_string += std::to_string(event.text.unicode - 48);
                                }
                                if (amount_molecules_string == "0") {
                                    amount_molecules_string = "1";
                                }
                                amount_molecules_text.setString(
                                        "Molecules amount: " + amount_molecules_string);
                            } else if (event.key.code == 8) {
                                if (amount_molecules_string.length() != 0) {
                                    amount_molecules_string.pop_back();
                                }
                                amount_molecules_text.setString(
                                        "Molecules amount: " + amount_molecules_string);
                            } else if (event.key.code == 13) {
                                amount_molecule = std::stoi(amount_molecules_string);
                                was_first_key_press = false;
                                amount_molecules_text.setFillColor(sf::Color::Black);
                                enter_press_amount += 1;
                            }
                        }
                        if (enter_press_amount == demo_parameters_amount) {
//                            delete molecule_box;  // BUG! WHY DELETE DOES NOT WORK?????????

                            //HISTOGRAM
                            for (int i = 0; i < histogram_bins; i++) {
                                statistics_collected = false;
                                full_counts_max = 0;
                                histogram_demo_counts[i] = 0;
                                trajectory_max_len = 0;
                                trajectory_min_len = 10000000;
                                collision_max_amount  = 0;
                                collision_min_amount = 10000000;
                                trajectory_lens[i] = (i + 1) * (trajectory_max_len - trajectory_min_len) / histogram_bins;
                                collision_amounts[i] = i * (collision_max_amount - collision_min_amount) / histogram_bins;
                                main_window.draw(histogram_demo[i]);
                                main_window.draw(histogram_axes_text[i]);
                            }

                            molecule_box = NULL;
                            std::cout << demo_length_or_collisions << std::endl;
                            molecule_box = new Box(radius_molecule, bounds, amount_molecule, calc_ms, mode_curr, int(demo_length_or_collisions), demo_length_or_collisions);
                            molecule_box->unpause();
                            is_molecules_active = true;
                            molecules.clear();
                            molecules = std::vector<sf::CircleShape>(amount_molecule, sf::CircleShape(radius_molecule));
                            {
                                auto m = molecule_box->get_molecules();
                                //srand(1723210);
                                const std::vector<Molecule> &v = m.get();
                                for (int i = 0; i < v.size(); i++) {
                                    molecules[i].setRadius(radius_molecule);
                                    molecules[i].setPosition(v[i].position.first, v[i].position.second);
                                    molecules[i].setOrigin(radius_molecule, radius_molecule);
                                    molecules[i].setPointCount(100);
                                    molecules[i].setFillColor(sf::Color(rand() % 220, rand() % 220, rand() % 220));
                                }
                            }
                            for (int i = 0; i < def_obs; i++) {
                                trajectories[i].clear();
                                break_points[i].clear();
                            }
                            molecule_box->pause();
                            is_molecules_active = false;
                            enter_press_amount = 0;
                            is_all_params_were_passed = true;
                            demo_input.bText.setString("Input");
                            main_window_state = 1;
                        }
                        break;
                }
            }
            main_window.clear(sf::Color(255, 255, 255));
            if (!is_clear_box) {
                for (int i = 0; i < molecules.size(); i++) {
                    main_window.draw(molecules[i]);
                }
            }
            for (int i = 0; i < def_obs; i++) {
                main_window.draw(trajectories[i].data(), trajectories[i].size(), sf::PrimitiveType::LineStrip);
            }

            main_window.draw(demo_start_stop.sprite);
            main_window.draw(demo_start_stop.bText);

            main_window.draw(demo_back.sprite);
            main_window.draw(demo_back.bText);

            main_window.draw(demo_input.sprite);
            main_window.draw(demo_input.bText);

            main_window.draw(demo_clear.sprite);
            main_window.draw(demo_clear.bText);

            main_window.draw(demo_regime_type_text);
            main_window.draw(demo_length_or_collisions_text);
            main_window.draw(demo_statistics_type_text);
            main_window.draw(radius_size_text);
            main_window.draw(amount_molecules_text);

            //BORDERS
            main_window.draw(border1);
            main_window.draw(border2);
            main_window.draw(border3);
            main_window.draw(border4);
            main_window.draw(border5);

        } else if (main_window_state == 3) { // AUTHORS
            main_window.clear(sf::Color(255, 255, 255));
//            main_window.draw(sf::CircleShape(50, 30));
//            main_window.draw(main_menu_demo.sprite);
//            main_window.draw(main_menu_theory.sprite);
//            main_window.draw(main_menu_authors.sprite);
//            main_window.draw(main_menu_exit.sprite);
//            main_window.draw(main_menu_demo.bText);
//            main_window.draw(main_menu_authors.bText);
//            main_window.draw(main_menu_theory.bText);
//            main_window.draw(main_menu_exit.bText);
            main_window.draw(authors_title_text1);
            main_window.draw(authors_title_text2);
            main_window.draw(cmc_logo_sprite);
            main_window.draw(fiz_logo_sprite);
            main_window.draw(teacher_text);
            main_window.draw(nikita_photo_sprite);
            main_window.draw(ilya_photo_sprite);
            main_window.draw(dima_photo_sprite);
            main_window.draw(nikita_text);
            main_window.draw(ilya_text);
            main_window.draw(dima_text);
            main_window.draw(cmc_logo_text);
            main_window.draw(fiz_logo_text);

            sf::Event event;
            while (main_window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        main_window.close();
                        exit(0);
                    case sf::Event::MouseButtonPressed:
                        if (event.key.code == sf::Mouse::Left) {
//                            sf::Vector2f mouse = main_window.mapPixelToCoords(sf::Mouse::getPosition(main_window));
//                            sf::FloatRect bounds_start = main_menu_demo.sprite.getGlobalBounds();
//                            sf::FloatRect bounds_theory = main_menu_theory.sprite.getGlobalBounds();
//                            sf::FloatRect bounds_authors = main_menu_authors.sprite.getGlobalBounds();
//                            sf::FloatRect bounds_exit = main_menu_exit.sprite.getGlobalBounds();
//                            if (bounds_start.contains(mouse)) {
//                                std::swap(molecules_activation_button[0], molecules_activation_button[1]);
//                                main_window_state = 1;
//                            } else if (bounds_theory.contains(mouse)) {
//                                main_window_state = 1;
//                            } else if (bounds_authors.contains(mouse)) {
//                                main_window_state = 3; // Authors
//                            }
//                            else if (bounds_exit.contains(mouse)) {
//                                main_window.close();
//                                exit(0);
//                        }
                        }
                        break;
                }
            }

        }
        main_window.display();
    }
}

