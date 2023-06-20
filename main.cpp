#include "Matrix.h"
#include "ColorWheel.h"
#include "Painting.h"
int main() {
    sf::RenderWindow window(sf::VideoMode{480, 370}, "LED Visualisation",  sf::Style::Close);

    Matrix matrix1(window, sf::Vector2f(0,0));
    Matrix matrix2(window, sf::Vector2f(16,0));
    Matrix matrix3(window, sf::Vector2f(32,0));
    ColorWheel colorwheel(window);
    Painting painting1(window, colorwheel, matrix1);
    Painting painting2(window, colorwheel, matrix2);
    Painting painting3(window, colorwheel, matrix3);

    while (window.isOpen()) {
        window.clear();

        //Hier alle draws
        sf::RectangleShape line(sf::Vector2f(480, 3));
        line.setPosition({0,320});
        window.draw(line);

        if(sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).x < 160 && sf::Mouse::getPosition(window).y <= 320 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
//            std::cout << "Matrix1\n";
            painting1.changeLedPixelColor();
        }
        if(sf::Mouse::getPosition(window).x >= 161 && sf::Mouse::getPosition(window).x < 320 && sf::Mouse::getPosition(window).y <= 320 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
//            std::cout << "Matrix2\n";
            painting2.changeLedPixelColor();
        }
        if(sf::Mouse::getPosition(window).x >= 321 && sf::Mouse::getPosition(window).x < 480 && sf::Mouse::getPosition(window).y <= 320 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
//            std::cout << "Matrix3\n";
            painting3.changeLedPixelColor();
        }

        colorwheel.colorInterface();
        matrix1.draw();
        matrix2.draw();
        matrix3.draw();
        window.display();

        sf::sleep( sf::milliseconds( 10));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                matrix1.writeMatrixToFile("matrix1.txt");
                matrix2.writeMatrixToFile("matrix2.txt");
                matrix3.writeMatrixToFile("matrix3.txt");
                std::cout << "Closed and Saved.\n";
                window.close();
            }
        }
    }
    return 0;
}