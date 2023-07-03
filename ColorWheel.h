//
// Created by jasie on 16-6-2023.
//

#ifndef LEDMATRIX_R2D2_COLORWHEEL_H
#define LEDMATRIX_R2D2_COLORWHEEL_H
#include <SFML/Graphics.hpp>

// this class is to choose a color to paint with.
class ColorWheel {
private:
    sf::Vector2f pos = sf::Vector2f(0.0,321.0);
    float size = 50;
    sf::Color current_color;
    sf::RenderWindow &w;
    void changeColor(sf::Color new_color);
    void makeColors(sf::Color color, sf::Vector2f position);
    int alpha = 255;

public:
    ColorWheel(sf::RenderWindow &w);
    void colorInterface();
    sf::Color getColor();
};


#endif //LEDMATRIX_R2D2_COLORWHEEL_H
