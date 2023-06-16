//
// Created by Erik on 16-6-2023.
//

#include "ColorWheel.h"
#include <iostream>
ColorWheel::ColorWheel(sf::RenderWindow &w): w(w) {
    current_color = sf::Color(0, 0, 0);
    std::cout << "loop\n";

    makeColors(sf::Color::White, sf::Vector2f(pos.x+size*0,pos.y));
    makeColors(sf::Color::Black, sf::Vector2f(pos.x+size*1,pos.y));
    makeColors(sf::Color::Red, sf::Vector2f(pos.x+size*2,pos.y));
    makeColors(sf::Color::Green, sf::Vector2f(pos.x+size*3,pos.y));
    makeColors(sf::Color::Blue, sf::Vector2f(pos.x+size*4,pos.y));

}

void ColorWheel::changeColor(sf::Color new_color) {
    current_color = new_color;
}

void ColorWheel::makeColors(sf::Color color, sf::Vector2f pos){
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(50,50));
    shape.setFillColor(sf::Color(200,200,200));
    shape.setPosition(pos);

    sf::RectangleShape color_shape;
    color_shape.setSize(sf::Vector2f(30, 30));
    color_shape.setFillColor(color);
    color_shape.setPosition(sf::Vector2f(pos.x + 10, pos.y + 10));
    w.draw(shape);
    w.draw(color_shape);
}

void ColorWheel::colorInterface() {
    if(sf::Mouse::getPosition(w).x > pos.x+size*0 &&
       sf::Mouse::getPosition(w).x < pos.x+size*1 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1 &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        changeColor(sf::Color(255,255,255));
        std::cout << std::to_string(current_color.r) << std::to_string(current_color.g) << std::to_string(current_color.b);
        //change the current drawing color to white
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*1 &&
       sf::Mouse::getPosition(w).x < pos.x+size*2 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1  &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        changeColor(sf::Color(0,0,0));
        //change the current drawing color to black
        std::cout << std::to_string(current_color.r) << std::to_string(current_color.g) << std::to_string(current_color.b);
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*2 &&
       sf::Mouse::getPosition(w).x < pos.x+size*3 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1 &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        changeColor(sf::Color(255,0,0));
        //change the current drawing color to red
        std::cout << std::to_string(current_color.r) << std::to_string(current_color.g) << std::to_string(current_color.b);
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*3 &&
       sf::Mouse::getPosition(w).x < pos.x+size*4 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1&&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        changeColor(sf::Color(0,255,0));
        //change the current drawing color to green
        std::cout << std::to_string(current_color.r) << std::to_string(current_color.g) << std::to_string(current_color.b);
    }

    if(sf::Mouse::getPosition(w).x >= pos.x+size*4 &&
       sf::Mouse::getPosition(w).x < pos.x+size*5 &&
       sf::Mouse::getPosition(w).y > pos.y+size*0 &&
       sf::Mouse::getPosition(w).y < pos.y+size*1 &&
       sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        changeColor(sf::Color(0, 0, 255));
        //change the current drawing color to blue
        std::cout << std::to_string(current_color.r) << std::to_string(current_color.g) << std::to_string(current_color.b);
    }
}

sf::Color ColorWheel::getColor() {
    //std::cout << std::to_string(current_color.r) << std::to_string(current_color.g) << std::to_string(current_color.b);
    return current_color;
}