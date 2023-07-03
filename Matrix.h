//
// Created by jasie on 15-6-2023.
//

#ifndef LEDMATRIX_R2D2_MATRIX_H
#define LEDMATRIX_R2D2_MATRIX_H
#include <array>
#include <SFML/Graphics.hpp>
#include "Led.h"
#include <vector>
#include <iostream>
#include <fstream>

class Matrix {
private:
    sf::RenderWindow &w;
    sf::Vector2f matrix_pos;

    int rows = 16;
    int cols = 32;
public:

    std::vector<std::vector<Led>> matrix_vec = {{}};
    Matrix(sf::RenderWindow &w, sf::Vector2f matrixPos = {0,0});
    void draw();
    void instantColor(int red, int green, int blue);
    void changePixelColor(int x, int y, sf::Color color);

    
    void writeMatrixToFile(const std::string& filename);
};


#endif //LEDMATRIX_R2D2_MATRIX_H
