//
// Created by Erik on 16-6-2023.
//

/**
 * @file ColorWheel.cpp
 * @author Erik (you@domain.com)
 * @brief the class colorWheel, this is used to change colors to draw.  
 * @version 0.1
 * @date 2023-07-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "ColorWheel.h"
#include <iostream>

/**
 * @brief Construct a new Color Wheel:: Color Wheel object
 * 
 * @param w a SFML window. 
 */
ColorWheel::ColorWheel(sf::RenderWindow &w): w(w){
    current_color = sf::Color(255, 0, 0);
}

void ColorWheel::changeColor(sf::Color new_color) {
    current_color = new_color;
}

void ColorWheel::makeColors(sf::Color color, sf::Vector2f position){
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(50,50));
    shape.setFillColor(sf::Color(200,200,200));
    shape.setPosition(position);

    sf::RectangleShape color_shape;
    color_shape.setSize(sf::Vector2f(30, 30));
    color_shape.setFillColor(color);
    color_shape.setPosition(sf::Vector2f(position.x + 10, position.y + 10));
    w.draw(shape);
    w.draw(color_shape);
}
/**
 * @brief makes the inferface to choose the colors. 
 * 
 */
void ColorWheel::colorInterface() {
    makeColors(sf::Color::White, sf::Vector2f(pos.x+size*0,pos.y));
    makeColors(sf::Color::Black, sf::Vector2f(pos.x+size*1,pos.y));
    makeColors(sf::Color::Red, sf::Vector2f(pos.x+size*2,pos.y));
    makeColors(sf::Color::Green, sf::Vector2f(pos.x+size*3,pos.y));
    makeColors(sf::Color::Blue, sf::Vector2f(pos.x+size*4,pos.y));
    makeColors(sf::Color(255,0,255), sf::Vector2f(pos.x+size*5,pos.y));
    makeColors(sf::Color(0,255,255), sf::Vector2f(pos.x+size*6,pos.y));

    if(sf::Mouse::getPosition(w).x > pos.x+size*0 &&
        sf::Mouse::getPosition(w).x < pos.x+size*1 &&
        sf::Mouse::getPosition(w).y > pos.y+size*0 &&
        sf::Mouse::getPosition(w).y < pos.y+size*1 &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        changeColor(sf::Color(255,255,255, alpha));
        //change the current drawing color to white
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*1 &&
       sf::Mouse::getPosition(w).x < pos.x+size*2 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1  &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        changeColor(sf::Color(0,0,0, alpha));
        //change the current drawing color to black
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*2 &&
       sf::Mouse::getPosition(w).x < pos.x+size*3 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1 &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        changeColor(sf::Color(255,0,0, alpha));
        //change the current drawing color to red
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*3 &&
       sf::Mouse::getPosition(w).x < pos.x+size*4 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1&&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        changeColor(sf::Color(0,255,0, alpha));
        //change the current drawing color to green
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*4 &&
       sf::Mouse::getPosition(w).x < pos.x+size*5 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1 &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        changeColor(sf::Color(0, 0, 255, alpha));
        //change the current drawing color to blue
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*5 &&
       sf::Mouse::getPosition(w).x < pos.x+size*6 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1 &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        changeColor(sf::Color(255,0,255, alpha));
        //change the current drawing color to purple
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*6 &&
       sf::Mouse::getPosition(w).x < pos.x+size*7 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1 &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        changeColor(sf::Color(0,255,255, alpha));
    }
}
/**
 * @brief sends you the current chosen color which is selected from the color interface. 
 * 
 * @return sf::Color the color to draw.
 */
sf::Color ColorWheel::getColor() {
    return current_color;
}