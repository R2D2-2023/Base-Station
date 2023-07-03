//
// Created by jasie on 16-6-2023.
//
/**
 * @file Painting.cpp
 * @author Erik
 * @brief the class for painting the LED's
 * @version 0.1
 * @date 2023-07-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "Painting.h"
#include <iostream>
#include <cstdlib>

/**
 * @brief Construct a new Painting:: Painting object
 * 
 * @param w a SFML window
 * @param wheel a colorwheel from the class colorwheel. 
 * @param matrix a LED matrix from the class Matrix
 */
Painting::Painting(sf::RenderWindow & w, ColorWheel & wheel, Matrix & matrix) : w(w), wheel(wheel), matrix(matrix){
    current_paint_color = wheel.getColor();
}

/**
 * @brief locates your mouse's x and y position. 
 * 
 * @return sf::Vector2i 
 */
sf::Vector2i Painting::locateMousePixel() {
    sf::Vector2i mouse = sf::Mouse::getPosition(w);
    mouse.x = mouse.x/10;
    mouse.y = mouse.y/10 + 1; //+1 for offset.
    if (mouse.y <= 0)
        mouse.y = 1;
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
    return mouse;
}

/**
 * @brief when pressed on a LED with a color from color wheel it changes the color. 
 * 
 */
void Painting::changeLedPixelColor(){
    current_paint_color = wheel.getColor();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        matrix.changePixelColor(locateMousePixel().x, locateMousePixel().y, current_paint_color);
    }
}
