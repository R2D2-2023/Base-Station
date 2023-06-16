//
// Created by jasie on 15-6-2023.
//

#include "Matrix.h"
Matrix::Matrix(sf::RenderWindow &w, sf::Vector2f matrixPos): w(w), matrix_pos(matrixPos){
    matrixPos.x *= 10;
    matrixPos.y *= 10; // to make the first and second matrix line up.

    sf::Vector2f pos;
    pos += matrixPos;
    for (int i = 0; i < cols; ++i) {
        std::vector<Led> row;
        row.reserve(rows);
        for (int j = 0; j < rows; ++j) {
            row.emplace_back(w, pos);
            pos.x += 10;
        }
        pos.x = matrixPos.x;
        matrix_vec.emplace_back(row);
        pos.y +=10;
    }
}

void Matrix::instantColor(int redw, int greenw, int bluew){
    for (auto & strip : matrix_vec){
        for (auto & led : strip){
//            std::cout << red << " " << green << " " << blue <<"\n";
            led.changeColor(sf::Color(redw, greenw, bluew));
        }
    }
}

void Matrix::draw() {
    for(auto & strip : matrix_vec) {
        for (auto & led : strip) {
            led.draw();
        }
    }
}

void Matrix::changePixelColor(int x, int y, sf::Color color) {
    //std::cout<<"ik verander van kleur"<<"\n";
    matrix_vec[x][y].changeColor(color);
}