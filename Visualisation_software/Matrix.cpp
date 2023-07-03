//
// Created by jasie on 15-6-2023.
//
/**
 * @file Matrix.cpp
 * @author Jasper Middendorp
 * @brief 
 * @version 0.1
 * @date 2023-07-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "Matrix.h"
/**
 * @brief Construct a new Matrix:: Matrix object
 * 
 * @param w a SFML window
 * @param matrixPos the starting pos on the sfml window. 
 */
 Matrix::Matrix(sf::RenderWindow &w, sf::Vector2f matrixPos): w(w), matrix_pos(matrixPos){
    matrixPos.x *= 10; // to make the first and second matrix line up.
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

/**
 * @brief changes the color of all leds in the matrix.
 * 
 * @param redw an int between 0 and 255 for the red value
 * @param greenw an int between 0 and 255 for the green value
 * @param bluew an int between 0 and 255 for the blue value
 */
void Matrix::instantColor(int redw, int greenw, int bluew){
    for (auto & strip : matrix_vec){
        for (auto & led : strip){
            led.changeColor(sf::Color(redw, greenw, bluew));
        }
    }
}

/**
 * @brief draws the matrix on the screen by drawing each LED.
 * 
 */
void Matrix::draw() {
    for(auto & strip : matrix_vec) {
        for (auto & led : strip) {
            led.draw();
        }
    }
}

/**
 * @brief the function to change a single LED's color. 
 * 
 * @param x the X posistion of the LED you want to change
 * @param y the Y posistion of the LED you want to change
 * @param color the new Color for the LED.
 */
void Matrix::changePixelColor(int x, int y, sf::Color color) {
    matrix_vec[y][x].changeColor(color);
}

/**
 * @brief whites the current colors of the leds from the matrix in a .txt file. 
 * 
 * @param filename the filename you want. 
 */
void Matrix::writeMatrixToFile(const std::string& filename){
        std::ofstream file;
        file.open(filename);
        for (int i = 0; i < matrix_vec.size(); ++i) {
            for (int j = 0; j<matrix_vec[i].size(); ++j) {
                file << matrix_vec[i][j].getColor()[0] << "," << matrix_vec[i][j].getColor()[1] << "," << matrix_vec[i][j].getColor()[2] << "\n";
            }
        }
        file.close();
    }
