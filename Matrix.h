//
// Created by jasie on 15-6-2023.
//

#ifndef LEDMATRIX_R2D2_MATRIX_H
#define LEDMATRIX_R2D2_MATRIX_H
#include <array>
#include <SFML/Graphics.hpp>
#include "Led.h"
#include <vector>

class Matrix {
private:
    sf::RenderWindow &w;
    std::vector<std::vector<Led>> matrixVec = {{}};

    sf::Vector2f matrixPos;
    int rows = 32;
    int cols = 16;

    int red = 0;
    int green = 0;
    int blue = 0;

public:
    Matrix(sf::RenderWindow &w, sf::Vector2f matrixPos = {0,0});
    void test();
    void color(uint8_t redw, uint8_t greenw, uint8_t bluew);
    void draw();
};


#endif //LEDMATRIX_R2D2_MATRIX_H
