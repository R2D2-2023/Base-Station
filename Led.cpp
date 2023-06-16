//
// Created by Erik on 15-6-2023.
//

#include "Led.h"
#include <iostream>

Led::Led(sf::RenderWindow &w, sf::Vector2f position, sf::Color color):window(w), position(position), color(color){
    shape.setSize({10,10});
}

void Led::draw(){
  //  std::cout << "LEDtest \t";
    shape.setSize(sf::Vector2f(10,10));
    //std::cout << "LEDtestSize \t";
    shape.setPosition(position);
    shape.setFillColor(color);
    //std::cout << "LEDtestPos \t";
    window.draw(shape);
    //std::cout << "LEDtestDraw \t";
}

void Led::changeColor(sf::Color newColor) {
    shape.setFillColor(newColor);
}