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
    std::vector<std::vector<Led>> matrix_vec = {{}};

    sf::Vector2f matrix_pos;
    int rows = 32;
    int cols = 16;

public:
    Matrix(sf::RenderWindow &w, sf::Vector2f matrixPos = {0,0});
    void draw();
    void instantColor(int red, int green, int blue);
    void changePixelColor(int x, int y, sf::Color color);

    void writeMatrixToFile(std::string filename){
        std::ofstream file;
        file.open(filename);
        for (int i = 0; i < matrix_vec.size(); ++i) {
            for (int j = 0; j < matrix_vec[i].size(); ++j) {
                file << matrix_vec[i][j].getColor()[0] << "," << matrix_vec[i][j].getColor()[1] << "," << matrix_vec[i][j].getColor()[2] << "\n";
            }
//            file << "\n";
        }
        file.close();
    }
};


#endif //LEDMATRIX_R2D2_MATRIX_H
