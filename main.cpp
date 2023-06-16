#include "Matrix.h"
#include "ColorWheel.h"
#include "Painting.h"
int main() {
    sf::RenderWindow window(sf::VideoMode{320, 530}, "LED Visualisation",  sf::Style::Close);

    Matrix matrix1(window);
    Matrix matrix2(window, sf::Vector2f(0,16));
    ColorWheel colorwheel(window);
    Painting painting(window, colorwheel, matrix1);
    matrix1.instantColor(255, 0, 255);
    matrix1.writeMatrixToFile();

    while (window.isOpen()) {
        window.clear();

        //Hier alle draws
        sf::RectangleShape line(sf::Vector2f(330, 3));
        line.setPosition({0,481});
        window.draw(line);

//        matrix1.instantColor(255, 255, 255);
//        matrix1.writeMatrixToFile();
        matrix1.draw();
        matrix2.draw();
        colorwheel.colorInterface();
        painting.changeLedPixelColor();
        window.display();

        sf::sleep( sf::milliseconds( 100));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    return 0;
}
