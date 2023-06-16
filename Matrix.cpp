//
// Created by jasie on 15-6-2023.
//

#include "Matrix.h"
#include <iostream>
Matrix::Matrix(sf::RenderWindow &w, sf::Vector2f matrixPos): w(w), matrixPos(matrixPos){
    matrixPos.x *= 10;
    matrixPos.y *= 10; // to make the first and second matrix line up.

    sf::Vector2f pos;
    pos += matrixPos;
    for (int i = 0; i < cols; ++i) {
        std::vector<Led> row;
        row.reserve(rows);
        for (int j = 0; j < rows; ++j) {
            row.emplace_back(w, pos, sf::Color(red, green, blue));
            pos.x += 10;
        }
        pos.x = matrixPos.x;
        matrixVec.emplace_back(row);
        pos.y +=10;
    }
}

void Matrix::test() {
    sf::Color color = sf::Color(red, green, blue);
    for (auto & strip: matrixVec) {
        for (auto & led: strip) {
            led.changeColor(color);
        }
    }
    red --;
    green --;
    blue --;
    if(red <= 0){red=0,green=0,blue = 0;}
}

void Matrix::color(uint8_t redw, uint8_t greenw, uint8_t bluew){
    for (auto & strip : matrixVec){
        for (auto & led : strip) {
            if (red < redw) {
                red++;
            }
            if(green < greenw){
                green++;
            }
            if(blue < bluew) {
                blue++;
            }
            led.changeColor(sf::Color(red, green, blue));
        }
    }
}

void Matrix::draw() {
    std::cout << red << " " << green << " " << blue <<"\n";
    for(auto & strip : matrixVec) {
        for (auto & led : strip) {
            led.draw();
        }
 }

}
