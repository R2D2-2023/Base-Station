//
// Created by jasie en Erik on 15-6-2023.
//

#include "Led.h"
#include <iostream>

Led::Led(sf::RenderWindow &w, sf::Vector2f position, sf::Color color):window(w), position(position), color(color){
    shape.setSize({10,10});
    shape.setFillColor(color);
    shape.setPosition(position);
    shape.setSize(sf::Vector2f(10,10));
}

void Led::draw(){
    shape.setFillColor(color);
    window.draw(shape);
}

void Led::changeColor(sf::Color new_color) {
     color = new_color;
}