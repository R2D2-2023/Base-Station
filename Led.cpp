//
// Created by jasie en Erik on 15-6-2023.
//
/**
 * @file Led.cpp
 * @author Erik Schipper
 * @brief a single Led for a ledmatrix
 * @version 0.1
 * @date 2023-07-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Led.h"
#include <iostream>

/**
 * @brief Construct a new Led:: Led object
 * 
 * @param w a SFML window
 * @param position a starting position for the led. 
 * @param color a sfml color for the led
 */
Led::Led(sf::RenderWindow &w, sf::Vector2f position, sf::Color color):window(w), position(position), color(color){
    shape.setSize({10,10});
    shape.setFillColor(color);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(0.5);
    shape.setPosition(position);
    shape.setSize(sf::Vector2f(10,10));
}

/**
 * @brief draws a single LED on the screen.
 * 
 */
void Led::draw(){
    shape.setFillColor(color);
    window.draw(shape);
}
/**
 * @brief Makes the LED you have selected change its color. 
 * 
 * @param new_color a SFML color
 */
void Led::changeColor(sf::Color new_color) {
     color = new_color;
}