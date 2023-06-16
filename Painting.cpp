//
// Created by jasie on 16-6-2023.
//
#include "Painting.h"
#include <iostream>
Painting::Painting(sf::RenderWindow & w, ColorWheel & wheel, Matrix & matrix) : w(w), wheel(wheel), matrix(matrix) {
    current_paint_color = wheel.getColor();
}

sf::Vector2i Painting::locateMousePixel() {
    sf::Vector2i mouse = sf::Mouse::getPosition(w);
    mouse.x = std::tolower(mouse.x/10);
    mouse.y = std::tolower(mouse.y/10);
    return mouse;

}

void Painting::changeLedPixelColor(){
    current_paint_color = wheel.getColor();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
//        std::cout << std::to_string(current_paint_color.r) << " " << std::to_string(current_paint_color.g) << " " << std::to_string(current_paint_color.b) << " " << locateMousePixel().x << " " << locateMousePixel().y << "\n";
        matrix.changePixelColor(locateMousePixel().x, locateMousePixel().y, sf::Color::Blue);
    }
}

