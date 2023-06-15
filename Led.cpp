//
// Created by Erik on 15/06/2023.
//

#include "Led.h"

Led::Led(sf::RenderWindow &w, sf::Vector2f position):w(w), position(position){}

void Led::draw(sf::RenderWindow &w){
    shape.setPosition(position);
    w.draw(shape);
}