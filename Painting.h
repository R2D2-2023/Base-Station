//
// Created by jasie on 16-6-2023.
//

#ifndef LEDMATRIX_R2D2_PAINTING_H
#define LEDMATRIX_R2D2_PAINTING_H
#include <SFML/Graphics.hpp>
#include "ColorWheel.h"
#include "Matrix.h"
class Painting {
private:
    sf::RenderWindow & w;
    ColorWheel wheel;
    sf::Color current_paint_color;
    Matrix matrix;
public:
    Painting(sf::RenderWindow & w, ColorWheel & wheel, Matrix & matrix);
    sf::Vector2i locateMousePixel();
    void changeLedPixelColor();
};


#endif //LEDMATRIX_R2D2_PAINTING_H
