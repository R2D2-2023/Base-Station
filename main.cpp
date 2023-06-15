#include <iostream>
#include "Matrix.h"

int main() {
    sf::RenderWindow window(sf::VideoMode{320, 480}, "LED Visualisation");

    Matrix matrix1(window);
    Matrix matrix2(window, sf::Vector2f(0,16));

    while (window.isOpen()) {
        window.clear();

        //Hier alle draws
        matrix1.draw();
        matrix2.draw();
//        matrix1.color(255, 255, 255);
//        matrix2.color(255, 0, 255);
        window.display();

//        led.changeColor(sf::Color(255,255,0));
        sf::sleep( sf::milliseconds( 20));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    return 0;
}
