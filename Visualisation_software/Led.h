//
// Created by jasie en Erik on 15-6-2023.
//

#ifndef LEDMATRIX_R2D2_LED_H
#define LEDMATRIX_R2D2_LED_H
#include <SFML/Graphics.hpp>
#include <vector>

class Led {
private:
    sf::RenderWindow &window;
    sf::Vector2f position;
    sf::RectangleShape shape;
    sf::Color color;

public:
    Led(sf::RenderWindow &w, sf::Vector2f position = {0,0}, sf::Color color = sf::Color(0,0,0,50));
    void draw();
    void changeColor(sf::Color new_color);

    sf::Vector2f getPos();

    std::vector<int> getColor();

};


#endif //LEDMATRIX_R2D2_LED_H
