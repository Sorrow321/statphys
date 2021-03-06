#include "SFML/Graphics.hpp"
#include <iostream>
#include "box.cpp"
#include <cmath>
#include <sstream>
#include "distributions.cpp"
#include <stdio.h>
#include "windows.h"

int mode_curr = def_mode;
bool back_to_menu_was_pressed = false;
constexpr int length_coef = 10;


struct Button_menu
{
    sf::Vector2f bPosition;
    sf::IntRect bSprite;
    sf::Sprite sprite;
    sf::Font bFont;
    std::wstring bString;
    sf::Text bText;

    Button_menu(std::wstring bString, sf::Texture &texture, sf::Vector2f bPosition, sf::IntRect bSprite)
            : bString{ bString }, bPosition{ bPosition }, bSprite{ bSprite }
    {
        this->bFont.loadFromFile("text/TNR.ttf");
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

template <typename object_type, typename second_type>
void set_rus_string(object_type &object, const std::wstring &first_string, second_type param_value) {
    std::wostringstream tmp;
    tmp << first_string << param_value;
    object.setString(tmp.str());
}


int main() {
    const float screenWidth = (float)sf::VideoMode::getDesktopMode().width;
    const float screenHeight = (float)sf::VideoMode::getDesktopMode().height;
    const float coef_x_scale = (float)screenWidth / 1920;
    const float coef_y_scale = (float)screenHeight / 1080;


    setlocale(LC_ALL,"Rus");
    int main_window_state = 0;
    int amount_molecule = def_molnum;
    int radius_molecule = int(def_radius);
    bool is_molecules_active = true;

    sf::Texture box_field_texture;
    box_field_texture.loadFromFile("text/box_field.jpg");
    sf::Sprite box_field_sprite;
    box_field_texture.setSmooth(true);
    box_field_sprite.setTexture(box_field_texture);
    box_field_sprite.setPosition(23.0 * coef_x_scale, 23.0 * coef_y_scale);
    box_field_sprite.setScale(coef_x_scale, coef_y_scale);


    //std::tuple<double, double, double, double> bounds = {box_field_sprite.getPosition().x + 5, box_field_sprite.getLocalBounds().width + 16,
    //                                                     box_field_sprite.getPosition().y + 5, box_field_sprite.getLocalBounds().height + 16};

    std::tuple<double, double, double, double> bounds = {28, box_field_sprite.getLocalBounds().width + 16,
                                                         28, box_field_sprite.getLocalBounds().height + 16};


    //fprintf(stderr, "%d, %d", sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height/2);
    sf::RenderWindow main_window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
            L"����������� �� �������������� ������",
            sf::Style::Fullscreen);


    main_window.clear(sf::Color(255, 255, 255));
    Box *molecule_box = new Box(radius_molecule, bounds, amount_molecule);
    std::vector<sf::CircleShape> molecules(amount_molecule, sf::CircleShape(radius_molecule));
    //fprintf(stderr, "1");

    // VISUAL BORDERS for molecules box
    sf::RectangleShape border1;
    border1.setSize(sf::Vector2f(3 * coef_x_scale, sf::VideoMode::getDesktopMode().height/2));
    border1.setFillColor(sf::Color(100, 100, 100));
    border1.setPosition(sf::VideoMode::getDesktopMode().width, 0.0);

    sf::RectangleShape border2;
    border2.setSize(sf::Vector2f(3 * coef_x_scale, sf::VideoMode::getDesktopMode().height/2));
    border2.setFillColor(sf::Color(100, 100, 100));
    border2.setPosition(0.0, 0.0);

    sf::RectangleShape border3;
    border3.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 3 * coef_y_scale));
    border3.setFillColor(sf::Color(100, 100, 100));
    border3.setPosition(0.0, 0.0);

    sf::RectangleShape border4;
    border4.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, 3 * coef_y_scale));
    border4.setFillColor(sf::Color(100, 100, 100));
    border4.setPosition(0.0, sf::VideoMode::getDesktopMode().height/2);





    std::vector<Button_menu> molecules_activation_button;

    sf::Texture molecules_activation_texture;
    sf::Texture demonstration_button_texture;

    demonstration_button_texture.loadFromFile("text/button.png");
    sf::Texture demonstration_button_main_menu_texture;
    demonstration_button_main_menu_texture.loadFromFile("text/demonstation.jpg");

    sf::Texture theory_button_main_menu_texture;
    theory_button_main_menu_texture.loadFromFile("text/theory.jpg");

    sf::Texture authors_button_main_menu_texture;
    authors_button_main_menu_texture.loadFromFile("text/authors.jpg");

    sf::Texture back_to_menu_authors_texture;
    back_to_menu_authors_texture.loadFromFile("text/b_back.jpg");

    sf::Texture main_menu_exit_texture;
    main_menu_exit_texture.loadFromFile("text/exit.png");

//    sf::Texture demonstration_button_main_menu_texture;
//    demonstration_button_main_menu_texture.loadFromFile("text/demonstation.jpg");

    /* MAIN MENU BUTTONS(0) */
    Button_menu main_menu_demo(L"", demonstration_button_main_menu_texture,
                               sf::Vector2f(
                                       sf::VideoMode::getDesktopMode().width * 0.19 - demonstration_button_main_menu_texture.getSize().x * 0.5 * coef_x_scale,
                                       sf::VideoMode::getDesktopMode().height * 0.7 - demonstration_button_main_menu_texture.getSize().y * 0.5 * coef_y_scale),sf::IntRect(0, 0, demonstration_button_main_menu_texture.getSize().x, demonstration_button_main_menu_texture.getSize().y));

    Button_menu main_menu_theory(L"", theory_button_main_menu_texture,sf::Vector2f(
            main_menu_demo.bPosition.x + main_menu_demo.sprite.getLocalBounds().width * 1.6 * coef_x_scale - theory_button_main_menu_texture.getSize().x * 0.5 * coef_x_scale,
            sf::VideoMode::getDesktopMode().height * 0.7 - theory_button_main_menu_texture.getSize().y * 0.5 * coef_y_scale),sf::IntRect(0, 0, theory_button_main_menu_texture.getSize().x,
                                                                                                                          theory_button_main_menu_texture.getSize().y));

    Button_menu main_menu_authors(L"", authors_button_main_menu_texture,sf::Vector2f(
            main_menu_theory.bPosition.x + coef_x_scale * (main_menu_theory.sprite.getLocalBounds().width * 1.6)  - authors_button_main_menu_texture.getSize().x * 0.5 * coef_x_scale,
            sf::VideoMode::getDesktopMode().height * 0.7 - authors_button_main_menu_texture.getSize().y * 0.5* coef_y_scale),sf::IntRect(0, 0, authors_button_main_menu_texture.getSize().x,
                                                                                                                           authors_button_main_menu_texture.getSize().y));
    Button_menu main_menu_exit(L"", main_menu_exit_texture, sf::Vector2f(
            main_menu_authors.bPosition.x + coef_x_scale * main_menu_authors.sprite.getLocalBounds().width * 0.5 - main_menu_exit_texture.getSize().x * 0.5 * coef_x_scale,
            sf::VideoMode::getDesktopMode().height - main_menu_exit_texture.getSize().y * coef_y_scale - 30 * coef_y_scale), sf::IntRect(0, 0, main_menu_exit_texture.getSize().x, main_menu_exit_texture.getSize().y));

    main_menu_demo.sprite.setScale(coef_x_scale, coef_y_scale);
    main_menu_theory.sprite.setScale(coef_x_scale, coef_y_scale);
    main_menu_authors.sprite.setScale(coef_x_scale, coef_y_scale);
    main_menu_exit.sprite.setScale(coef_x_scale, coef_y_scale);



    sf::Texture background_main_menu_mini_texture;
    sf::Sprite background_main_menu_mini_sprite;
    background_main_menu_mini_texture.loadFromFile("text/title_lom.png");
    background_main_menu_mini_texture.setSmooth(true);
    background_main_menu_mini_sprite.setPosition(sf::VideoMode::getDesktopMode().width * 0.5 - background_main_menu_mini_texture.getSize().x * coef_x_scale * 0.5, 15 * coef_y_scale);
    background_main_menu_mini_sprite.setTexture(background_main_menu_mini_texture);
    background_main_menu_mini_sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Texture background_main_menu_title2_texture;
    sf::Sprite background_main_menu_title2_sprite;
    background_main_menu_title2_texture.loadFromFile("text/title2.png");
    background_main_menu_title2_texture.setSmooth(true);
    background_main_menu_title2_sprite.setPosition(sf::VideoMode::getDesktopMode().width * 0.5 - background_main_menu_title2_texture.getSize().x * 0.5 * coef_x_scale, coef_y_scale * (105 + background_main_menu_mini_texture.getSize().y));
    background_main_menu_title2_sprite.setTexture(background_main_menu_title2_texture);
    // added by ilya
    background_main_menu_title2_sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);
//    background_main_menu_title2_sprite.setScale((float) main_window.getSize().x / background_main_menu_title2_texture.getSize().x,
//                                         (float) main_window.getSize().y / background_main_menu_title2_texture.getSize().y);

    sf::Texture background_main_menu_title3_texture;
    sf::Sprite background_main_menu_title3_sprite;
    background_main_menu_title3_texture.loadFromFile("text/title3.png");
    background_main_menu_title3_texture.setSmooth(true);
    background_main_menu_title3_sprite.setPosition(sf::VideoMode::getDesktopMode().width * 0.5 - background_main_menu_title3_texture.getSize().x * 0.5 * coef_x_scale,   coef_y_scale * (195 + background_main_menu_title2_texture.getSize().y + background_main_menu_mini_texture.getSize().y));
    background_main_menu_title3_sprite.setTexture(background_main_menu_title3_texture);
    background_main_menu_title3_sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);
//    background_main_menu_title3_sprite.setScale((float) main_window.getSize().x / background_main_menu_title3_texture.getSize().x,
//                                                (float) main_window.getSize().y / background_main_menu_title3_texture.getSize().y);




    /* DEMONSTATION(1) */
    // BUTTONS

    sf::Texture start_stop_button_texture;
    start_stop_button_texture.loadFromFile("text/b_start.jpg");
    sf::Texture input_button_texture;
    input_button_texture.loadFromFile("text/b_input.jpg");
    sf::Texture clear_button_texture;
    clear_button_texture.loadFromFile("text/b_del_mol.jpg");
    sf::Texture demo_back_button_texture;
    demo_back_button_texture.loadFromFile("text/b_back.jpg");

    int button_menu_demo_width = 350;
    int button_menu_demo_height = 50;
    Button_menu demo_start_stop(L"", start_stop_button_texture, sf::Vector2f(sf::VideoMode::getDesktopMode().width - (start_stop_button_texture.getSize().x * 2 + 240) * coef_x_scale, sf::VideoMode::getDesktopMode().height - 10*button_menu_demo_height * coef_y_scale),
                                sf::IntRect(0, 0, start_stop_button_texture.getSize().x, start_stop_button_texture.getSize().y));
    Button_menu demo_input(L"", input_button_texture, sf::Vector2f(sf::VideoMode::getDesktopMode().width - (input_button_texture.getSize().x * 2 + 240) * coef_x_scale, sf::VideoMode::getDesktopMode().height - (10*button_menu_demo_height - 100) * coef_y_scale),
                           sf::IntRect(0, 0, input_button_texture.getSize().x, input_button_texture.getSize().y));
    Button_menu demo_clear(L"", clear_button_texture, sf::Vector2f(sf::VideoMode::getDesktopMode().width - (clear_button_texture.getSize().x + 20) * coef_x_scale, sf::VideoMode::getDesktopMode().height - (10*button_menu_demo_height) * coef_y_scale),
                           sf::IntRect(0, 0, clear_button_texture.getSize().x, clear_button_texture.getSize().y));
    Button_menu demo_back(L"", demo_back_button_texture, sf::Vector2f(sf::VideoMode::getDesktopMode().width - (demo_back_button_texture.getSize().x + 20) * coef_x_scale, sf::VideoMode::getDesktopMode().height - (10*button_menu_demo_height - 100) * coef_y_scale),
                          sf::IntRect(0, 0, demo_back_button_texture.getSize().x, demo_back_button_texture.getSize().y));
    
    demo_start_stop.sprite.setScale(coef_x_scale, coef_y_scale);
    demo_input.sprite.setScale(coef_x_scale, coef_y_scale);
    demo_clear.sprite.setScale(coef_x_scale, coef_y_scale);
    demo_back.sprite.setScale(coef_x_scale, coef_y_scale);
    

    bool is_clear_box = false;

    // INPUT VALUES
    sf::Font global_font;
    global_font.loadFromFile("text/TNR.ttf");


    sf::Text input_text_show;
    input_text_show.setFont(global_font);
    input_text_show.setFillColor(sf::Color::Black);
    input_text_show.setPosition(sf::Vector2f( (demo_input.bPosition.x + demo_back.bPosition.x + 250 * coef_x_scale) / 2 - input_text_show.getLocalBounds().width * 0.5 ,
                                              demo_input.bPosition.y + demo_input.sprite.getLocalBounds().height + 25 * coef_y_scale));
    input_text_show.setScale(coef_x_scale, coef_y_scale);

    sf::Text demo_regime_type_text;
    int regime_type = 1;
    demo_regime_type_text.setFont(global_font);
    std::wostringstream regime_type_string;
    if (regime_type == 1) {
        regime_type_string << L"�����: " << L"����������";
    } else {
        regime_type_string << L"�����: " << L"������������";
    }
    demo_regime_type_text.setString(regime_type_string.str());
    demo_regime_type_text.setFillColor(sf::Color::Black);
    demo_regime_type_text.setPosition(sf::Vector2f( (demo_input.bPosition.x + demo_back.bPosition.x + 250 * coef_x_scale) / 2 - demo_regime_type_text.getLocalBounds().width * 0.5 ,
                                                    demo_input.bPosition.y + demo_input.sprite.getLocalBounds().height + 50 * coef_y_scale));
    demo_regime_type_text.setScale(coef_x_scale, coef_y_scale);

    //    int mode_curr = 1; //choose length - 1; choose amount of collisions - 2
    sf::Text demo_statistics_type_text;
    demo_statistics_type_text.setFont(global_font);
    if (mode_curr == 1) {
        set_rus_string(demo_statistics_type_text, L"�����������: ", L"������������");
    } else {
        set_rus_string(demo_statistics_type_text, L"�����������: ", L"�����");
    }
    demo_statistics_type_text.setFillColor(sf::Color::Black);
    demo_statistics_type_text.setPosition(sf::Vector2f((demo_input.bPosition.x + demo_back.bPosition.x + 250 * coef_x_scale) / 2 - demo_regime_type_text.getLocalBounds().width * 0.5,
                                                       demo_input.bPosition.y + demo_input.sprite.getLocalBounds().height + 100 * coef_y_scale));
    demo_statistics_type_text.setScale(coef_x_scale, coef_y_scale);


    int demo_length_or_collisions;
    if (mode_curr == 1) {
        demo_length_or_collisions = int(def_trajlength);
    } else {
        demo_length_or_collisions = int(def_interactions_num);
    }
    sf::Text demo_length_or_collisions_text;
    demo_length_or_collisions_text.setFont(global_font);
    std::wstring demo_length_or_collisions_string = L"����� ����������: ";
    set_rus_string(demo_length_or_collisions_text, demo_length_or_collisions_string, def_trajlength);
//    demo_length_or_collisions_string = std::to_wstring(def_trajlength);
//    std::cout << std::stoi(demo_length_or_collisions_string);
    if (mode_curr == 2) {
        demo_length_or_collisions_string = L"���-�� ������������: ";
        set_rus_string(demo_length_or_collisions_text, demo_length_or_collisions_string, def_interactions_num);
//        demo_length_or_collisions_string = std::to_wstring(def_interactions_num);
    }

//    demo_length_or_collisions_string = demo_length_or_collisions_string.erase(demo_length_or_collisions_string.find_last_not_of('0') + 1, std::string::npos);
//    demo_length_or_collisions_text.setString(demo_length_or_collisions_string);
    demo_length_or_collisions_text.setFillColor(sf::Color::Black);
    demo_length_or_collisions_text.setPosition(sf::Vector2f((demo_input.bPosition.x + demo_back.bPosition.x + 250 * coef_x_scale) / 2 - demo_regime_type_text.getLocalBounds().width * 0.5,
                                                            demo_input.bPosition.y + demo_input.sprite.getLocalBounds().height + 150 * coef_y_scale));
    demo_length_or_collisions_text.setScale(coef_x_scale, coef_y_scale);

    sf::Text radius_size_text;
    radius_size_text.setFont(global_font);
    radius_molecule = def_radius;

    std::string radius_size_string = std::to_string(radius_molecule);
    set_rus_string(radius_size_text, L"������: ", radius_molecule);

    radius_size_string = radius_size_string.erase(radius_size_string.find_last_not_of('0') + 1, std::string::npos);
//    radius_size_text.setString(radius_size_string);
    radius_size_text.setFillColor(sf::Color::Black);
    radius_size_text.setPosition(sf::Vector2f((demo_input.bPosition.x + demo_back.bPosition.x + 250 * coef_x_scale) / 2 - demo_regime_type_text.getLocalBounds().width * 0.5,
                                              demo_input.bPosition.y + demo_input.sprite.getLocalBounds().height + 200 * coef_y_scale));;
    radius_size_text.setScale(coef_x_scale, coef_y_scale);

    sf::Text amount_molecules_text;
    amount_molecules_text.setFont(global_font);
    amount_molecule = def_molnum;
    std::string amount_molecules_string = std::to_string(amount_molecule);
    set_rus_string(amount_molecules_text, L"���-�� �������: ", amount_molecule);
//    amount_molecules_text.setString("Molecules amount: " + std::to_string(amount_molecule));
    amount_molecules_text.setFillColor(sf::Color::Black);
    amount_molecules_text.setPosition(sf::Vector2f((demo_input.bPosition.x + demo_back.bPosition.x + 250 * coef_x_scale) / 2 - demo_regime_type_text.getLocalBounds().width * 0.5,
                                                   demo_input.bPosition.y + demo_input.sprite.getLocalBounds().height + 250 * coef_y_scale));
    amount_molecules_text.setScale(coef_x_scale, coef_y_scale);
    // INPUT VALUES END


    // VISUAL BORDERS for graphs
    sf::RectangleShape border5;
    border5.setSize(sf::Vector2f(3, sf::VideoMode::getDesktopMode().height / 2.0));
    border5.setFillColor(sf::Color(0, 0, 0));
    border5.setPosition(amount_molecules_text.getPosition().x - 5, sf::VideoMode::getDesktopMode().height / 2.0 + 3);



    sf::Texture hist_field_texture;
    hist_field_texture.loadFromFile("text/hist_field_new.jpg");
    sf::Sprite hist_field_sprite;
    hist_field_texture.setSmooth(true);
    hist_field_sprite.setTexture(hist_field_texture);
    hist_field_sprite.setPosition(23 * coef_x_scale, (float) sf::VideoMode::getDesktopMode().height / 2 + 40 * coef_y_scale);
    hist_field_sprite.setScale(coef_x_scale, coef_y_scale);
//    hist_field_sprite.setScale(border5.getPosition().x / hist_field_texture.getSize().x,
//                               (float) sf::VideoMode::getDesktopMode().height / 2 / hist_field_texture.getSize().y);

    //TRAJECTORY LINES:
    std::vector<std::vector<sf::Vertex>> trajectories(def_obs);

    std::vector<sf::Vertex> additional_trajectories[3][3];
    std::vector<std::vector<sf::Color>> colors(def_obs);

    // ����� ������
    std::vector<std::vector<sf::CircleShape>> break_points(def_obs);

    for (int i = 0; i < def_obs; i++) {
        colors[i].emplace_back(sf::Color::Red);
        colors[i].emplace_back(sf::Color::Blue);
    }


    // INTERACTIONS AMOUNT BLOCK(HISTOGRAM)uy
    sf::Text molecules_interactions_text;
    molecules_interactions_text.setFont(global_font);
    set_rus_string(molecules_interactions_text, L"���-�� ������������: ",  L"");
//    molecules_interactions_text.setString("Molecules interactions amount: ");
    molecules_interactions_text.setFillColor(sf::Color::Black);
    molecules_interactions_text.setPosition(sf::Vector2f(800, 450));


    // HISTOGRAM VISUALIZATION:
    int histogram_bins = 60;
    constexpr int histogram_norm_const = 1000;
    double trajectory_max_len = 0;
    double trajectory_min_len = 100000000;
    double collision_max_amount = 0;
    double collision_min_amount = 1000000000;

    std::vector<std::vector<double>> theory_distribution = (mode_curr == 1) ? distribution_1(int(demo_length_or_collisions * length_coef), radius_molecule, (float) (box_field_texture.getSize().x * box_field_texture.getSize().y), amount_molecule)  \
                            : distribution_2(int(demo_length_or_collisions), radius_molecule, (float) (box_field_texture.getSize().x * box_field_texture.getSize().y), amount_molecule);
    if (mode_curr == 1) {
        collision_max_amount = theory_distribution[theory_distribution.size() - 1][0];
        if ((collision_max_amount - histogram_bins) > 0) {
            collision_min_amount = collision_max_amount - histogram_bins;
        } else {
            collision_min_amount = 0;
        }
//        fprintf(stderr, "%f\n", collision_max_amount);
    } else {
        trajectory_max_len = theory_distribution[theory_distribution.size() - 1][0];
        trajectory_min_len = 0;//theory_distribution[0][0];
//        fprintf(stderr, "%f\n", trajectory_max_len);
    }


    float full_counts_max = 0;
    bool statistics_collected = false;

    std::vector<int> histogram_demo_counts(histogram_bins);
    std::vector<float> trajectory_lens(histogram_bins);
    std::vector<float> collision_amounts(histogram_bins);
    std::vector<sf::RectangleShape> histogram_demo(histogram_bins);
    std::vector<sf::RectangleShape> histogram_axes_ticks(histogram_bins);
    int histogram_ticks_frequency = 10;
    std::vector<sf::Text> histogram_axes_text(histogram_bins);
    sf::Text histogram_axis_x_text;


    std::wostringstream histogram_axis_x_string;



    if (mode_curr == 1) {
        histogram_axis_x_string << L"���������� ������������";
    } else {
        histogram_axis_x_string << L"    ����� ����������   ";
    }
    histogram_axis_x_text.setString(histogram_axis_x_string.str());
    histogram_axis_x_text.setFillColor(sf::Color::Black);
    histogram_axis_x_text.setPosition(308 * coef_x_scale,
                                      sf::VideoMode::getDesktopMode().height / 2);
    histogram_axis_x_text.setOrigin(histogram_demo[0].getLocalBounds().left,
                                    histogram_demo[0].getSize().y);
    histogram_axis_x_text.setFont(global_font);
    histogram_axis_x_text.setScale(coef_x_scale, coef_y_scale);


    for (int i = 0; i < histogram_bins; i++) {
//        std::cout << "YES2";
        histogram_demo_counts[i] = 0;
        trajectory_lens[i] = (i)  * fabs(trajectory_max_len - trajectory_min_len) / histogram_bins;
        collision_amounts[i] = i * fabs((collision_max_amount - collision_min_amount)) / histogram_bins;
        histogram_demo[i].setSize(sf::Vector2f(histogram_norm_const / histogram_bins, 0));
        histogram_demo[i].setFillColor(sf::Color(rand() % 200, rand() % 200, rand() % 200));
        histogram_demo[i].setPosition(
                28 * coef_x_scale + i * histogram_demo[i].getSize().x * coef_x_scale,
                sf::VideoMode::getDesktopMode().height - 53 * coef_y_scale);
        histogram_demo[i].setOrigin(histogram_demo[i].getLocalBounds().left,
                                    histogram_demo[i].getSize().y);
        histogram_demo[i].setScale(coef_x_scale, coef_y_scale);

        if ((i % histogram_ticks_frequency) == 0) {
            histogram_axes_text[i].setFont(global_font);
            if (mode_curr == 1) {
                histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
            } else {
                histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i] / length_coef)));
            }
            histogram_axes_text[i].setFillColor(sf::Color::Black);
            histogram_axes_text[i].setPosition(28 * coef_x_scale + i * histogram_demo[i].getSize().x * coef_x_scale,
                                               sf::VideoMode::getDesktopMode().height - 58 * coef_y_scale);
            histogram_axes_text[i].setOrigin(histogram_demo[i].getLocalBounds().left,
                                             histogram_demo[i].getSize().y);
            histogram_axes_text[i].setScale(coef_x_scale, coef_y_scale);
        }
    }

    //THEORY
    sf::Texture theory_first_page_texture;
    sf::Sprite theory_first_page_sprite;
    theory_first_page_texture.loadFromFile("text/theory_first_page.png");
    theory_first_page_texture.setSmooth(true);
    theory_first_page_sprite.setTexture(theory_first_page_texture);
    theory_first_page_sprite.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width * 0.5 - theory_first_page_sprite.getLocalBounds().width * 0.5,
                                                      sf::VideoMode::getDesktopMode().height * 0.5 - theory_first_page_sprite.getLocalBounds().height * 0.5));
    theory_first_page_sprite.setScale(1, 1);

    Button_menu authors_back(L"", back_to_menu_authors_texture,
                             sf::Vector2f(
                                     sf::VideoMode::getDesktopMode().width * 0.5 - back_to_menu_authors_texture.getSize().x * 0.5 * coef_x_scale,
                                     sf::VideoMode::getDesktopMode().height - back_to_menu_authors_texture.getSize().y * 1.2 * coef_y_scale),
                             sf::IntRect(0, 0, back_to_menu_authors_texture.getSize().x, back_to_menu_authors_texture.getSize().y));
    authors_back.sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);
    authors_back.bText.setScale(1 * coef_x_scale, 1 * coef_y_scale);
    //AUTHORS
    sf::Texture cmc_logo_texture;
    sf::Sprite cmc_logo_sprite;
    cmc_logo_texture.loadFromFile("text/cmc_logo_small.png");
    cmc_logo_texture.setSmooth(true);
    cmc_logo_sprite.setTexture(cmc_logo_texture);
    cmc_logo_sprite.setPosition(sf::Vector2f(0 * coef_x_scale, 50 * coef_y_scale));
    cmc_logo_sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Texture background_texture;
    sf::Sprite background_sprite;
    background_texture.loadFromFile("text/background.bmp");
    background_texture.setSmooth(true);
    background_sprite.setTexture(background_texture);
    background_sprite.setScale((float) main_window.getSize().x / background_texture.getSize().x,
                               (float) main_window.getSize().y / background_texture.getSize().y);
//    background_sprite.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - background_texture.getSize().x,  50));


    sf::Texture fiz_logo_texture;
    sf::Sprite fiz_logo_sprite;
    fiz_logo_texture.loadFromFile("text/fiz_logo_wout_back.gif");
    fiz_logo_texture.setSmooth(true);
    fiz_logo_sprite.setTexture(fiz_logo_texture);
    fiz_logo_sprite.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width - fiz_logo_texture.getSize().x * coef_x_scale,  50 * coef_y_scale));
    fiz_logo_sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Text cmc_logo_text;
    cmc_logo_text.setFont(global_font);
    std::wostringstream cmc_logo_string;
    cmc_logo_string << L"      ���������\n�������������� ����������\n            �\n     �����������";
    cmc_logo_text.setString(cmc_logo_string.str());
    cmc_logo_text.setFillColor(sf::Color::Black);
    cmc_logo_text.setPosition(sf::Vector2f(cmc_logo_sprite.getGlobalBounds().left,  cmc_logo_sprite.getGlobalBounds().height + 50 * coef_y_scale));
    cmc_logo_text.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Text fiz_logo_text;
    fiz_logo_text.setFont(global_font);
    std::wstring fiz_logo_string = L"����������\n���������";
    fiz_logo_text.setString(fiz_logo_string);
    fiz_logo_text.setFillColor(sf::Color::Black);
    fiz_logo_text.setPosition(sf::Vector2f(fiz_logo_sprite.getGlobalBounds().left + fiz_logo_sprite.getLocalBounds().width * 0.5 * coef_x_scale - fiz_logo_text.getLocalBounds().width * 0.5 * coef_x_scale,
                                           fiz_logo_sprite.getGlobalBounds().height + 50 * coef_y_scale));
    fiz_logo_text.setScale(1 * coef_x_scale, 1 * coef_y_scale);


    sf::Text teacher_text;
    teacher_text.setFont(global_font);
    std::wstring teacher_string = L"������� ������������: ������ �������� ����� �������������\n������: ��������� ������� �������� ����������";
    teacher_text.setString(teacher_string);
    teacher_text.setFillColor(sf::Color::Black);
    teacher_text.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width * 0.5 - teacher_text.getLocalBounds().width * 0.5 * coef_x_scale,   cmc_logo_sprite.getGlobalBounds().height + 125 * coef_y_scale + 80 * coef_y_scale));
    teacher_text.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Texture nikita_photo_texture;
    sf::Sprite nikita_photo_sprite;
    nikita_photo_texture.loadFromFile("text/Nikita.jpg");
    nikita_photo_texture.setSmooth(true);
    nikita_photo_sprite.setTexture(nikita_photo_texture);
    nikita_photo_sprite.setPosition(sf::Vector2f(cmc_logo_text.getGlobalBounds().left + 5 * coef_x_scale,  cmc_logo_sprite.getGlobalBounds().height + 300 * coef_y_scale + 80 * coef_y_scale));
    nikita_photo_sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Text nikita_text;
    nikita_text.setFont(global_font);
    std::wstring nikita_string = L"������ �������";
    nikita_text.setString(nikita_string);
    nikita_text.setFillColor(sf::Color::Black);
    nikita_text.setPosition(sf::Vector2f(nikita_photo_sprite.getGlobalBounds().left + nikita_photo_sprite.getGlobalBounds().width * 0.5 - nikita_text.getLocalBounds().width * 0.5 * coef_x_scale,
                                         nikita_photo_sprite.getPosition().y + nikita_photo_sprite.getLocalBounds().height * coef_y_scale + 5 * coef_y_scale));
    nikita_text.setScale(1 * coef_x_scale, 1 * coef_y_scale);



    sf::Texture ilya_photo_texture;
    sf::Sprite ilya_photo_sprite;
    ilya_photo_texture.loadFromFile("text/Ilya.jpg");
    ilya_photo_texture.setSmooth(true);
    ilya_photo_sprite.setTexture(ilya_photo_texture);
    ilya_photo_sprite.setPosition(sf::Vector2f(sf::VideoMode::getDesktopMode().width * 0.5 - ilya_photo_sprite.getLocalBounds().width * 0.5 * coef_x_scale,  cmc_logo_sprite.getGlobalBounds().height + 300 * coef_y_scale + 80 * coef_y_scale));
    ilya_photo_sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Text ilya_text;
    ilya_text.setFont(global_font);
    std::wstring ilya_string = L"���� �������";
    ilya_text.setString(ilya_string);
    ilya_text.setFillColor(sf::Color::Black);
    ilya_text.setPosition(sf::Vector2f(sf::Vector2f(ilya_photo_sprite.getGlobalBounds().left + ilya_photo_sprite.getGlobalBounds().width * 0.5 - ilya_text.getLocalBounds().width * 0.5 * coef_x_scale,
                                                    ilya_photo_sprite.getPosition().y + ilya_photo_sprite.getLocalBounds().height * coef_y_scale + 5 * coef_y_scale)));
    ilya_text.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Texture dima_photo_texture;
    sf::Sprite dima_photo_sprite;
    dima_photo_texture.loadFromFile("text/Dima.jpg");
    dima_photo_texture.setSmooth(true);
    dima_photo_sprite.setTexture(dima_photo_texture);
    dima_photo_sprite.setPosition(sf::Vector2f(fiz_logo_sprite.getGlobalBounds().left + fiz_logo_sprite.getGlobalBounds().width * 0.5  - dima_photo_sprite.getLocalBounds().width * 0.55 * coef_x_scale,
                                               cmc_logo_sprite.getGlobalBounds().height + 300 * coef_y_scale + 80 * coef_y_scale));
    dima_photo_sprite.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Text dima_text;
    dima_text.setFont(global_font);
    std::wstring dima_string = L"������� ��������";
    dima_text.setString(dima_string);
    dima_text.setFillColor(sf::Color::Black);
    dima_text.setPosition(sf::Vector2f(dima_photo_sprite.getGlobalBounds().left + dima_photo_sprite.getGlobalBounds().width * 0.5 - dima_text.getLocalBounds().width * 0.5 * coef_x_scale,
                                       dima_photo_sprite.getPosition().y + dima_photo_sprite.getLocalBounds().height * coef_y_scale + 5 * coef_y_scale));
    dima_text.setScale(1 * coef_x_scale, 1 * coef_y_scale);

    sf::Text authors_title_text2;
    authors_title_text2.setFont(global_font);
    std::wstring authors_title_string2 = L"������:";
    authors_title_text2.setString(authors_title_string2);
    authors_title_text2.setFillColor(sf::Color::Black);
    authors_title_text2.setPosition(sf::VideoMode::getDesktopMode().width * 0.5 - authors_title_text2.getLocalBounds().width * 0.5 * coef_x_scale,
                                    cmc_logo_sprite.getGlobalBounds().height + 250 * coef_y_scale + 80 * coef_y_scale);
    authors_title_text2.setScale(1 * coef_x_scale, 1 * coef_y_scale);


    // DEMONSTRATION(1) END

    srand(100500);
    {
        auto m = molecule_box->get_molecules();
        //srand(1723210);
        const std::vector<Molecule> &v = m.get();
        for (int i = 0; i < v.size(); i++) {
            molecules[i].setRadius(radius_molecule);
            molecules[i].setPosition(v[i].position.first * coef_x_scale, v[i].position.second * coef_y_scale);
            molecules[i].setScale(coef_x_scale, coef_y_scale);
            molecules[i].setOrigin(radius_molecule, radius_molecule);
            molecules[i].setPointCount(100);
            if (i < def_obs) {
                molecules[i].setFillColor(sf::Color(255, 0, 0));
            } else {
                molecules[i].setFillColor(sf::Color(rand() % 180, rand() % 180, rand() % 180));
            }
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
            main_window.draw(background_sprite);
            main_window.draw(background_main_menu_mini_sprite);
            main_window.draw(background_main_menu_title2_sprite);
            main_window.draw(background_main_menu_title3_sprite);

//            main_window.draw(sf::CircleShape(50, 30));
            main_window.draw(main_menu_demo.sprite);
            main_window.draw(main_menu_theory.sprite);
            main_window.draw(main_menu_authors.sprite);
            main_window.draw(main_menu_exit.sprite);
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
                                ShellExecute(GetDesktopWindow(), "open", "theory.pdf", NULL, NULL, SW_SHOWNORMAL);
                                //system("theory.pdf");
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
            if (back_to_menu_was_pressed) {
                back_to_menu_was_pressed = false;
                //HISTOGRAM
                trajectory_max_len = 0;
                trajectory_min_len = 10000000;
                collision_max_amount = 0;
                collision_min_amount = 10000000;
                // ����� ��������������: ����
                std::vector<std::vector<double>> theory_distribution = (mode_curr == 1) ? distribution_1(int(demo_length_or_collisions * length_coef), radius_molecule, (float) (box_field_texture.getSize().x * box_field_texture.getSize().y), amount_molecule)  \
                            : distribution_2(int(demo_length_or_collisions), radius_molecule, (float) (box_field_texture.getSize().x * box_field_texture.getSize().y), amount_molecule);

//                            for (int i = 0; i < int(theory_distribution.size()); i++) {
//                                theory_distribution_graph[i].clear();
//                                theory_distribution_graph[i].emplace_back(sf::Vector2f(theory_distribution[i][0] - theory_distribution[0][0] + hist_field_sprite.getPosition().x,
//                                        - theory_distribution[i][1] + hist_field_sprite.getPosition().y + hist_field_sprite.getLocalBounds().height),
//                                                                          sf::Color::Red);
//                                fprintf(stderr, "%f", theory_distribution[i][0]);
//                            }
                if (mode_curr == 1) {
                    collision_max_amount = theory_distribution[theory_distribution.size() - 1][0];
                    if ((collision_max_amount - histogram_bins) > 0) {
                        collision_min_amount = collision_max_amount - histogram_bins;
                    } else {
                        collision_min_amount = 0;
                    }
//                                fprintf(stderr, "%f\n", collision_max_amount);
                } else {
                    trajectory_max_len = theory_distribution[theory_distribution.size() - 1][0];
                    trajectory_min_len = 0;//theory_distribution[0][0];
//                                fprintf(stderr, "%f\n", trajectory_max_len);
                }
                for (int i = 0; i < histogram_bins; i++) {
                    statistics_collected = false;
                    full_counts_max = 0;
                    histogram_demo_counts[i] = 0;
                    trajectory_lens[i] =
                            i  * (trajectory_max_len - trajectory_min_len) / histogram_bins;
                    collision_amounts[i] = i + collision_min_amount;
                    if (mode_curr == 1) {
                        histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                    } else {
                        histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i] / length_coef)));
                    }
                    for (int j = 0; j < histogram_bins; j++) {
                        histogram_demo[j].setSize(sf::Vector2f(0, 0));
                    }
                    if (regime_type == 1) {
                        main_window.draw(histogram_demo[i]);
                        main_window.draw(histogram_axes_text[i]);
                    }
                }
                delete molecule_box;
                molecule_box = new Box(radius_molecule, bounds, amount_molecule, calc_ms, mode_curr, int(demo_length_or_collisions), demo_length_or_collisions * length_coef);
                is_molecules_active = true;
                molecules.clear();
                molecules = std::vector<sf::CircleShape>(amount_molecule, sf::CircleShape(radius_molecule));
                {
                    auto m = molecule_box->get_molecules();
                    //srand(1723210);
                    const std::vector<Molecule> &v = m.get();
                    for (int i = 0; i < v.size(); i++) {
                        // ��� ������ ���� ���?
                        molecules[i].setRadius(radius_molecule);
                        molecules[i].setPosition(v[i].position.first * coef_x_scale, v[i].position.second * coef_y_scale);
                        molecules[i].setScale(coef_x_scale, coef_y_scale);
                        molecules[i].setOrigin(radius_molecule, radius_molecule);
                        molecules[i].setPointCount(100);
                        if (i < def_obs) {
                            molecules[i].setFillColor(sf::Color(255, 0, 0));
                        } else {
                            molecules[i].setFillColor(sf::Color(rand() % 180, rand() % 180, rand() % 180));
                        }
                    }
                }
                for (int i = 0; i < def_obs; i++) {
                    trajectories[i].clear();
                    for (int i1 = -1; i1 < 2; i1++) {
                        for (int i2 = -1; i2 < 2; i2++) {
                            if (!i1 && !i2) {
                                continue;
                            }
                            additional_trajectories[i1 + 1][i2 + 1].clear();
                        }
                    }
                    break_points[i].clear();
                }
                molecule_box->pause();
                is_molecules_active = false;
            }


            if (is_molecules_active) {
                auto m = molecule_box->get_molecules();
                const std::vector<Molecule> &v = m.get();
                for (int i = 0; i < v.size(); i++) {
                    molecules[i].setPosition(v[i].position.first * coef_x_scale, v[i].position.second * coef_y_scale);
                    // marked
                    //std::cout << v[i].position.first * coef_x_scale << std::endl;
                }
                for(int i=0; i < def_obs; i++) {
                    if (mode_curr == 1) {
                        if (molecule_box->get_finished_length(i)) {
                            std::swap(colors[i][0], colors[i][1]);
                        }
                        if (molecule_box->get_interacted(i)) {
//                            molecule_box->set_interacted(i, false);
                            float break_point_radius = 4;
                            sf::CircleShape tmp_point;
                            tmp_point.setRadius(break_point_radius);
                            auto tt = molecules[i].getPosition();
//                            tt.x *= coef_x_scale;
//                            tt.y *= coef_y_scale;
                            tmp_point.setPosition(tt);
                            tmp_point.setOrigin(break_point_radius, break_point_radius);
                            tmp_point.setPointCount(100);
                            tmp_point.setScale(coef_x_scale, coef_y_scale);
                            tmp_point.setFillColor(sf::Color(0, 200, 0));
                            break_points[i].push_back(tmp_point);
//                            std::swap(colors[i][0], colors[i][1]);
                        }
                    } else {
                        if (molecule_box->get_finished_interactions(i)) {
//                            molecule_box->set_finished(i, false);
                            std::swap(colors[i][0], colors[i][1]);
                        }
                    }

                    trajectories[i].emplace_back(molecules[i].getPosition(), colors[i][0]);

                    size_t llen = trajectories[i].size();
                    auto src = trajectories[i].back().position;

                    for (int i1 = -1; i1 < 2; i1++) {
                        for (int i2 = -1; i2 < 2; i2++) {
                            if (!i1 && !i2) {
                                continue;
                            }
                            auto new_coord = src;
                            new_coord.x += i1 * coef_x_scale;
                            new_coord.y += i2 * coef_y_scale;
                            additional_trajectories[i1 + 1][i2 + 1].emplace_back(new_coord, colors[i][0]);
                        }
                    }
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
                                    start_stop_button_texture.loadFromFile("text/b_start.jpg");
                                    is_molecules_active = false;
                                } else {
                                    molecule_box->unpause();
                                    start_stop_button_texture.loadFromFile("text/b_stop.jpg");
//                                    set_rus_string(demo_start_stop.bText, L"����",
//                                                   L"");
                                    is_molecules_active = true;
                                }
                                is_clear_box = false;
                            } else if (bounds_back_to_menu.contains(mouse)) {
                                back_to_menu_was_pressed = true;
                                main_window_state = 0;
                                start_stop_button_texture.loadFromFile("text/b_start.jpg");
                                if (is_molecules_active) {
                                    molecule_box->pause();
                                    is_molecules_active = false;
                                }
                            } else if (bounds_input.contains(mouse)) {
                                if (is_molecules_active) {
                                    start_stop_button_texture.loadFromFile("text/b_start.jpg");
                                    molecule_box->pause();
                                    is_molecules_active = false;
                                }
                                main_window_state = 2; // PARAMETERS INPUT
                            } else if (bounds_clear.contains(mouse)) {
                                if (is_molecules_active) {
                                    molecule_box->pause();
                                    is_molecules_active = false;
                                }
                                start_stop_button_texture.loadFromFile("text/b_start.jpg");
                                is_clear_box = true;
                            }
                        }
                        break;
                }
            }
            main_window.clear(sf::Color(255, 255, 255));
            main_window.draw(background_sprite);
            main_window.draw(box_field_sprite);
            main_window.draw(hist_field_sprite);

            if (!is_clear_box) {
                for (int i = 0; i < molecules.size(); i++) {
                    main_window.draw(molecules[i]);
                }
            } else {
                for (int i = 0; i < histogram_bins; i++) {
                    main_window.draw(histogram_demo[i]);
                    if ((i % histogram_ticks_frequency) == 0) {
                        main_window.draw(histogram_axes_text[i]);
                    }
                }
            }

//            molecule_box

            //INTERACTIONS AMOUNT
//            molecules_interactions_text.setString();


            main_window.draw(demo_start_stop.sprite);
//            main_window.draw(demo_start_stop.bText);

            main_window.draw(demo_back.sprite);
//            main_window.draw(demo_back.bText);

            main_window.draw(demo_input.sprite);
//            main_window.draw(input_text_show);

            main_window.draw(demo_clear.sprite);
//            main_window.draw(demo_clear.bText);

            main_window.draw(demo_regime_type_text);
            main_window.draw(demo_length_or_collisions_text);
            main_window.draw(demo_statistics_type_text);
            main_window.draw(radius_size_text);
            main_window.draw(amount_molecules_text);

            main_window.draw(histogram_axis_x_text);
            //����� ������:
            if (mode_curr == 1) {
                if (regime_type == 2) {
                    for (int i = 0; i < def_obs; i++) {
                        for (int j = 0; j < break_points[i].size(); j++) {
                            main_window.draw(break_points[i][j]);
                        }
                    }
                }
            }
            if (regime_type == 2) {
                for (int i = 0; i < def_obs; i++) {
                    main_window.draw(trajectories[i].data(), trajectories[0].size(), sf::LinesStrip);

                    for (int i1 = -1; i1 < 2; i1++) {
                        for (int i2 = -1; i2 < 2; i2++) {
                            if (!i1 && !i2) {
                                continue;
                            }
                            main_window.draw(additional_trajectories[i1 + 1][i2 + 1].data(), additional_trajectories[i1 + 1][i2 + 1].size(), sf::LinesStrip);
                        }
                    }
                }
            }

            //HISTOGRAM
//            get_la
            if (mode_curr == 1) {
                double last_amount = molecule_box->get_last_interactions_num();
                if (last_amount != -1) {
                    for (int i = 0; i < (histogram_bins - 1); i++) {
//                        std::cout << histogram_demo_counts[i] << "\n";

                        //fprintf(stderr, "collision_%d - %f \n", i, collision_amounts[i]);
                        if ((last_amount >= collision_amounts[i]) && (last_amount < collision_amounts[i + 1])) {
                            histogram_demo_counts[i] += 1;
                            if (full_counts_max < histogram_demo_counts[i]) {
                                full_counts_max = histogram_demo_counts[i];
                            }
                        }
                        histogram_demo[i].setSize(
                                sf::Vector2f(histogram_norm_const / histogram_bins,
                                             histogram_demo_counts[i] * 425.0 / full_counts_max)
                        );
                        histogram_demo[i].setOrigin(histogram_demo[i].getLocalBounds().left,
                                                    histogram_demo[i].getSize().y);
                        if (statistics_collected) {
                            if (regime_type == 1) {
                                main_window.draw(histogram_demo[i]);
                                main_window.draw(histogram_axes_text[i]);
                            }
                        }
//                        std::cout << collision_max_amount << " " << collision_min_amount << " " << collision_max_amount << "\n";

                        if ((full_counts_max >= 1) && (!statistics_collected)) { // ����� ������ ���������� (����� ������ ���� ����� ����������)
                            full_counts_max = 0;
                            statistics_collected = true;
                            for (int j = 0; j < histogram_bins; j++) {
                                histogram_demo_counts[j] = 0;
                                collision_amounts[j] = (j+1) + collision_min_amount;
                                if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i] / length_coef)));
                                }
                            }
                        }
                    }
                } else {
                    for (int i=0; i < histogram_bins; i++) {
                        if (statistics_collected) {
                            if (regime_type == 1) {
                                main_window.draw(histogram_demo[i]);
                                main_window.draw(histogram_axes_text[i]);
                            }
                        }
                    }
                }
            } else {
                double last_len = molecule_box->get_last_len();
                if (last_len != -1) {
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
                                             histogram_demo_counts[i] *  425.0 / full_counts_max)
                        );
                        histogram_demo[i].setOrigin(histogram_demo[i].getLocalBounds().left,
                                                    histogram_demo[i].getSize().y);
                        if (statistics_collected) {
                            if (regime_type == 1) {
                                main_window.draw(histogram_demo[i]);
                                main_window.draw(histogram_axes_text[i]);
                            }
                        }

                        if ((full_counts_max >= 1) &&
                            (!statistics_collected)) { // ����� ������ ���������� (����� ������ ���� ����� ����������)
                            full_counts_max = 0;
                            statistics_collected = true;
                            for (int j = 0; j < histogram_bins; j++) {
                                histogram_demo_counts[j] = 0;
                                trajectory_lens[j] = j * (trajectory_max_len - trajectory_min_len) / histogram_bins;
                                if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i] / length_coef)));
                                }
                            }
                        }
                    }
                } else {
                    for (int i=0; i < histogram_bins; i++) {
                        if (statistics_collected) {
                            if (regime_type == 1) {
                                main_window.draw(histogram_demo[i]);
                                main_window.draw(histogram_axes_text[i]);
                            }
                        }
                    }
                }
            }
        } else if (main_window_state == 2) { // INPUT
            std::vector<std::wstring>  input_button_strings = {L"1 - ����������, 2 - ������������:",
                                                               L"1 - ������������, 2 - �����:",
                                                               L"������� ������ ��������:",
                                                               L"������� ������:",
                                                               L"������� ���-�� �������:"};
            input_text_show.setString(input_button_strings[enter_press_amount]);
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
                    demo_length_or_collisions_string = std::to_wstring(demo_length_or_collisions);
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
                                set_rus_string(demo_regime_type_text, L"�����: ",
                                               L"����������");
                                regime_type = 1;
                            } else if (event.key.code == 50) {
//                                fprintf(stderr, "AA");
                                set_rus_string(demo_regime_type_text, L"�����: ",
                                               L"������������");
//                                fprintf(stderr, "BB");
                                regime_type = 2;
                            } else if (event.key.code == 13) {
                                was_first_key_press = false;
                                demo_regime_type_text.setFillColor(sf::Color::Black);
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
                                set_rus_string(demo_statistics_type_text, L"�����������: ",
                                               L"������������");
                                mode_curr = 1;
                                histogram_axis_x_string.seekp(0);
                                histogram_axis_x_string << L"���������� ������������";
                                histogram_axis_x_text.setString(histogram_axis_x_string.str());
                                
                            } else if (event.key.code == 50) {
                                set_rus_string(demo_statistics_type_text, L"�����������: ",
                                               L"�����");
                                mode_curr = 2;
                                histogram_axis_x_string.seekp(0);
                                histogram_axis_x_string << L"    ����� ����������   ";
                                histogram_axis_x_text.setString(histogram_axis_x_string.str());
                            } else if (event.key.code == 13) {
                                was_first_key_press = false;
                                demo_statistics_type_text.setFillColor(sf::Color::Black);
                                if (mode_curr == 1) {
                                    set_rus_string(demo_length_or_collisions_text, L"����� ����������: ",
                                                   std::stoi(demo_length_or_collisions_string));
                                } else if (mode_curr == 2) {
                                    set_rus_string(demo_length_or_collisions_text, L"���-�� ������������: ",
                                                   std::stoi(demo_length_or_collisions_string));
                                }
                                main_window.draw(demo_length_or_collisions_text);
                                enter_press_amount += 1;
                            }
                        } else if (enter_press_amount == 2) {
                            if (event.key.code != 13) {
                                if (!was_first_key_press) {
                                    was_first_key_press = true;
                                    demo_length_or_collisions_string = L"";
                                }
                            }
                            demo_length_or_collisions_text.setFillColor(sf::Color::Red);
                            if (mode_curr == 1) {
                                if ((event.key.code >= 48) && (event.key.code < 58)) {
                                    if (demo_length_or_collisions_string.size() <= 3) {
                                        demo_length_or_collisions_string += std::to_wstring(event.text.unicode - 48);
                                    }
                                    if (demo_length_or_collisions_string.length() <= 4) {
                                        set_rus_string(demo_length_or_collisions_text, L"����� ����������: ",
                                                       std::stoi(demo_length_or_collisions_string));
                                    } else {
                                        demo_length_or_collisions_string.pop_back();
                                        set_rus_string(demo_length_or_collisions_text, L"����� ����������: ",
                                                       std::stoi(demo_length_or_collisions_string));
                                    }
                                } else if (event.key.code == 8) {
                                    if (demo_length_or_collisions_string.length() != 0) {
                                        demo_length_or_collisions_string.pop_back();
                                    }
                                    if (demo_length_or_collisions_string.length() >= 1) {
                                        set_rus_string(demo_length_or_collisions_text, L"����� ����������: ",
                                                       std::stoi(demo_length_or_collisions_string));
                                    } else {
                                        set_rus_string(demo_length_or_collisions_text, L"����� ����������: ",
                                                       L"");
                                    }
                                } else if (event.key.code == 13) {
                                    demo_length_or_collisions = std::stoi(demo_length_or_collisions_string);
                                    if (demo_length_or_collisions < 100 / length_coef) {
                                        demo_length_or_collisions = 100 / length_coef;
                                    }
                                    was_first_key_press = false;
                                    set_rus_string(demo_length_or_collisions_text, L"����� ����������: ", demo_length_or_collisions);
                                    demo_length_or_collisions_text.setFillColor(sf::Color::Black);
                                    enter_press_amount += 1;
                                }
                            } else if (mode_curr == 2) {
                                if (event.key.code != 13) {
                                    if (!was_first_key_press) {
                                        was_first_key_press = true;
                                        demo_length_or_collisions_string = L"";
                                    }
                                }
                                demo_length_or_collisions_text.setFillColor(sf::Color::Red);
                                if (((event.key.code >= 48) && (event.key.code < 58)) || (event.key.code == 46)) {
                                    demo_length_or_collisions_string += static_cast<char>(event.text.unicode);
                                    set_rus_string(demo_length_or_collisions_text, L"���-�� ������������: ", std::stoi(demo_length_or_collisions_string));
                                } else if (event.key.code == 8) {
                                    if (demo_length_or_collisions_string.length() != 0) {
                                        demo_length_or_collisions_string.pop_back();
                                    }
                                    if (demo_length_or_collisions_string.length() >= 1) {
                                        set_rus_string(demo_length_or_collisions_text, L"���-�� ������������: ",
                                                       std::stoi(demo_length_or_collisions_string));
                                    } else {
                                        set_rus_string(demo_length_or_collisions_text, L"���-�� ������������: ",
                                                       L"");
                                    }
                                } else if (event.key.code == 13) {
                                    demo_length_or_collisions = std::stoi(demo_length_or_collisions_string);
                                    if (demo_length_or_collisions > 100) {
                                        demo_length_or_collisions = 100;
                                    }
                                    was_first_key_press = false;
                                    set_rus_string(demo_length_or_collisions_text, L"���-�� ������������: ", demo_length_or_collisions);
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
                                if (std::stod(radius_size_string) >= 8) {
                                    radius_size_string = "8";
                                } else if (std::stod(radius_size_string) < 1) {
                                    radius_size_string =  "1";
                                }
                                if (radius_size_string.size() > 4) {
                                    radius_size_string.pop_back();
                                }
                                set_rus_string(radius_size_text, L"������: ",
                                               std::stoi(radius_size_string));
//                                radius_size_text.setString("Radius size: " + radius_size_string);
                            } else if (event.key.code == 8) {
                                if (radius_size_string.length() != 0) {
                                    radius_size_string.pop_back();
                                }
                                if (radius_size_string.length() != 0) {
                                    set_rus_string(radius_size_text, L"������: ",
                                                   std::stoi(radius_size_string));
                                } else {
                                    set_rus_string(radius_size_text, L"������: ",
                                                   L"");
                                }
                            } else if (event.key.code == 13) {
                                radius_molecule = std::stoi(radius_size_string);
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
                                if (amount_molecules_string.size() <= 3) {
                                    set_rus_string(amount_molecules_text, L"���-�� �������: ",
                                                   std::stoi(amount_molecules_string));
                                }
                            } else if (event.key.code == 8) {
                                if (amount_molecules_string.length() != 0) {
                                    amount_molecules_string.pop_back();
                                }
                                if (amount_molecules_string.length() >= 1) {
                                    set_rus_string(amount_molecules_text, L"���-�� �������: ",
                                                   std::stoi(amount_molecules_string));
                                } else {
                                    set_rus_string(amount_molecules_text, L"���-�� �������: ",
                                                   L"");
                                }
                            } else if (event.key.code == 13) {
                                amount_molecule = std::stoi(amount_molecules_string);
                                was_first_key_press = false;
                                amount_molecules_text.setFillColor(sf::Color::Black);
                                enter_press_amount += 1;
                            }
                        }
                        if (enter_press_amount == demo_parameters_amount) {


                            //HISTOGRAM
                            trajectory_max_len = 0;
                            trajectory_min_len = 10000000;
                            collision_max_amount = 0;
                            collision_min_amount = 10000000;
                            // ����� ��������������: ����
                            std::vector<std::vector<double>> theory_distribution = (mode_curr == 1) ? distribution_1(int(demo_length_or_collisions * length_coef), radius_molecule, (float) (box_field_texture.getSize().x * box_field_texture.getSize().y), amount_molecule)  \
                            : distribution_2(int(demo_length_or_collisions), radius_molecule, (float) (box_field_texture.getSize().x * box_field_texture.getSize().y), amount_molecule);
                            if (mode_curr == 1) {
                                collision_max_amount = theory_distribution[theory_distribution.size() - 1][0];
                                if ((collision_max_amount - histogram_bins) > 0) {
                                    collision_min_amount = collision_max_amount - histogram_bins;
                                } else {
                                    collision_min_amount = 0;
                                }
//                                fprintf(stderr, "%f\n", collision_max_amount);
                            } else {
                                trajectory_max_len = theory_distribution[theory_distribution.size() - 1][0];
                                trajectory_min_len = 0;//theory_distribution[0][0];
//                                fprintf(stderr, "%f\n", trajectory_max_len);
                            }
                            for (int i = 0; i < histogram_bins; i++) {
                                statistics_collected = false;
                                full_counts_max = 0;
                                histogram_demo_counts[i] = 0;
                                trajectory_lens[i] =
                                        i  * (trajectory_max_len - trajectory_min_len) / histogram_bins;
                                collision_amounts[i] = i + collision_min_amount;
                                if (mode_curr == 1) {
                                    histogram_axes_text[i].setString(std::to_string(int(collision_amounts[i])));
                                } else {
                                    histogram_axes_text[i].setString(std::to_string(int(trajectory_lens[i] / length_coef)));
                                }
                                for (int j = 0; j < histogram_bins; j++) {
                                    histogram_demo[j].setSize(sf::Vector2f(0, 0));
                                }
                                if (regime_type == 1) {
                                    main_window.draw(histogram_demo[i]);
                                    main_window.draw(histogram_axes_text[i]);
                                }
                            }
                            delete molecule_box;
                            molecule_box = new Box(radius_molecule, bounds, amount_molecule, calc_ms, mode_curr, int(demo_length_or_collisions), demo_length_or_collisions * length_coef);
                            is_molecules_active = true;
                            molecules.clear();
                            molecules = std::vector<sf::CircleShape>(amount_molecule, sf::CircleShape(radius_molecule));
                            {
                                auto m = molecule_box->get_molecules();
                                //srand(1723210);
                                const std::vector<Molecule> &v = m.get();
                                for (int i = 0; i < v.size(); i++) {
                                    molecules[i].setRadius(radius_molecule);
                                    molecules[i].setPosition(v[i].position.first * coef_x_scale, v[i].position.second * coef_y_scale);
                                    molecules[i].setOrigin(radius_molecule, radius_molecule);
                                    molecules[i].setScale(coef_x_scale, coef_y_scale);
                                    molecules[i].setPointCount(100);
                                    if (i < def_obs) {
                                        molecules[i].setFillColor(sf::Color(0, 255, 0));
                                    } else {
                                        molecules[i].setFillColor(sf::Color(rand() % 180, rand() % 180, rand() % 180));
                                    }
                                }
                            }
                            for (int i = 0; i < def_obs; i++) {
                                trajectories[i].clear();
                                for (int i1 = -1; i1 < 2; i1++) {
                                    for (int i2 = -1; i2 < 2; i2++) {
                                        if (!i1 && !i2) {
                                            continue;
                                        }
                                        additional_trajectories[i1 + 1][i2 + 1].clear();
                                    }
                                }
                                break_points[i].clear();
                            }
                            molecule_box->pause();
                            is_molecules_active = false;
                            enter_press_amount = 0;
                            is_all_params_were_passed = true;
                            set_rus_string(demo_input.bText, L"",
                                           L"");
                            main_window_state = 1;
                        }
                    break;
                }
            }
            main_window.clear(sf::Color(255, 255, 255));
            main_window.draw(background_sprite);
            main_window.draw(box_field_sprite);
            main_window.draw(hist_field_sprite);

            if (!is_clear_box) {
                for (int i = 0; i < molecules.size(); i++) {
                    main_window.draw(molecules[i]);
                }
            }
            if (regime_type == 2) {
                for (int i = 0; i < def_obs; i++) {
                    main_window.draw(trajectories[i].data(), trajectories[i].size(), sf::PrimitiveType::LineStrip);

                    for (int i1 = -1; i1 < 2; i1++) {
                        for (int i2 = -1; i2 < 2; i2++) {
                            if (!i1 && !i2) {
                                continue;
                            }
                            main_window.draw(additional_trajectories[i1 + 1][i2 + 1].data(), additional_trajectories[i1 + 1][i2 + 1].size(), sf::PrimitiveType::LineStrip);
                        }
                    }
                }
            }
//            for (int i = theory_distribution_graph.size(); i < theory_distribution_graph.size(); i++) {
//                main_window.draw(theory_distribution_graph[i].data(), theory_distribution[0].size(), sf::PrimitiveType::LinesStrip);
//            }
            main_window.draw(demo_start_stop.sprite);
//            main_window.draw(demo_start_stop.bText);

            main_window.draw(demo_back.sprite);
//            main_window.draw(demo_back.bText);

            main_window.draw(demo_input.sprite);
            main_window.draw(input_text_show);

            main_window.draw(demo_clear.sprite);
//            main_window.draw(demo_clear.bText);

            main_window.draw(demo_regime_type_text);
            main_window.draw(demo_length_or_collisions_text);
            main_window.draw(demo_statistics_type_text);
            main_window.draw(radius_size_text);
            main_window.draw(amount_molecules_text);

            //BORDERS
//            main_window.draw(border1);
//            main_window.draw(border2);
//            main_window.draw(border3);
//            main_window.draw(border4);
//            main_window.draw(border5);
            main_window.draw(histogram_axis_x_text);


        } else if (main_window_state == 3) { // AUTHORS

            main_window.clear(sf::Color(255, 255, 255));
            main_window.draw(background_sprite);
            main_window.draw(background_main_menu_mini_sprite);
            main_window.draw(background_main_menu_title2_sprite);
//            main_window.draw(background_main_menu_title3_sprite);
//            main_window.draw(sf::CircleShape(50, 30));
//            main_window.draw(main_menu_demo.sprite);
//            main_window.draw(main_menu_theory.sprite);
//            main_window.draw(main_menu_authors.sprite);
//            main_window.draw(main_menu_exit.sprite);
//            main_window.draw(main_menu_demo.bText);
//            main_window.draw(main_menu_authors.bText);
//            main_window.draw(main_menu_theory.bText);
//            main_window.draw(main_menu_exit.bText);
//            main_window.draw(authors_title_text1);
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
            main_window.draw(authors_back.sprite);
            main_window.draw(authors_back.bText);

            sf::Event event;
            while (main_window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        main_window.close();
                        exit(0);
                    case sf::Event::MouseButtonPressed:
                        if (event.key.code == sf::Mouse::Left) {
                            sf::Vector2f mouse = main_window.mapPixelToCoords(sf::Mouse::getPosition(main_window));
                            sf::FloatRect bounds_back = authors_back.sprite.getGlobalBounds();
//                            sf::FloatRect bounds_theory = main_menu_theory.sprite.getGlobalBounds();
//                            sf::FloatRect bounds_authors = main_menu_authors.sprite.getGlobalBounds();
//                            sf::FloatRect bounds_exit = main_menu_exit.sprite.getGlobalBounds();
                            if (bounds_back.contains(mouse)) {
//                                std::swap(molecules_activation_button[0], molecules_activation_button[1]);
                                main_window_state = 0;
                            } //else if (bounds_theory.contains(mouse)) {
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
        } else if (main_window_state == 4) {
            main_window.clear(sf::Color(255, 255, 255));
            main_window.draw(background_sprite);
            main_window.draw(cmc_logo_sprite);
            main_window.draw(cmc_logo_text);
            main_window.draw(fiz_logo_sprite);
            main_window.draw(fiz_logo_text);
            main_window.draw(theory_first_page_sprite);
            main_window.draw(authors_back.sprite);
            main_window.draw(authors_back.bText);

            sf::Event event;
            while (main_window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        main_window.close();
                        exit(0);
                    case sf::Event::MouseButtonPressed:
                        if (event.key.code == sf::Mouse::Left) {
                            sf::Vector2f mouse = main_window.mapPixelToCoords(sf::Mouse::getPosition(main_window));
                            sf::FloatRect bounds_back = authors_back.sprite.getGlobalBounds();
                            if (bounds_back.contains(mouse)) {
                                main_window_state = 0;
                            }
                        }
                        break;
                }
            }
        }
        main_window.display();
    }
}