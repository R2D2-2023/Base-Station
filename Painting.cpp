//
// Created by jasie on 16-6-2023.
//
#include "Painting.h"
#include <iostream>
#include <cstdlib>
Painting::Painting(sf::RenderWindow & w, ColorWheel & wheel, Matrix & matrix) : w(w), wheel(wheel), matrix(matrix){
    current_paint_color = wheel.getColor();
    matrix.changePixelColor(0,1, sf::Color::Blue);
    //matrix.changePixelColor(2,2,sf::Color(255,0,0));
    //changeLedPixelColor();
}

sf::Vector2i Painting::locateMousePixel() {
    sf::Vector2i mouse = sf::Mouse::getPosition(w);
    mouse.x = mouse.x/10;
    mouse.y = mouse.y/10 + 1; //+1 for offset.
      std::cout << mouse.y << std::endl;
    if (mouse.y <= 0)
        mouse.y = 0;
    if (mouse.y >= 32)
        mouse.y = 32;
    if (mouse.x > 32)
        mouse.x %= 32;
    if(mouse.x == 32)
        mouse.x = 16;
    if (mouse.x >= 16 && mouse.x <= 31)
        mouse.x %= 16;
    if (mouse.x <= 0)
        mouse.x = 0;
    std::cout << mouse.x << std::endl;
    return mouse;
}

void Painting::changeLedPixelColor(){
    current_paint_color = wheel.getColor();
//    matrix.changePixelColor(2,2,sf::Color(255,0,0));
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
//        std::cout << std::to_string(current_paint_color.r) << " " << std::to_string(current_paint_color.g) << " " << std::to_string(current_paint_color.b) << " " << locateMousePixel().x << " " << locateMousePixel().y << "\n";
//        matrix.changePixelColor(locateMousePixel().x, locateMousePixel().y, sf::Color(rand()%255,rand()%255,rand()%255));
    matrix.changePixelColor(locateMousePixel().x, locateMousePixel().y, current_paint_color);
    }
}

