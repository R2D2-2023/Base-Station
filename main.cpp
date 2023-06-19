#include "Matrix.h"
#include "ColorWheel.h"
#include "Painting.h"
int main() {
    sf::RenderWindow window(sf::VideoMode{320, 530}, "LED Visualisation",  sf::Style::Close);

    Matrix matrix1(window, sf::Vector2f(0,0));
    Matrix matrix2(window, sf::Vector2f(0,16));
    Matrix matrix3(window, sf::Vector2f(0,32));
    ColorWheel colorwheel(window);
    Painting painting1(window, colorwheel, matrix1);
    Painting painting2(window, colorwheel, matrix2);
    Painting painting3(window, colorwheel, matrix3);

    while (window.isOpen()) {
        window.clear();

        //Hier alle draws
        sf::RectangleShape line(sf::Vector2f(330, 3));
        line.setPosition({0,481});
        window.draw(line);

//        matrix1.instantColor(255, 255, 255);
//        matrix1.writeMatrixToFile();
//        matrix1.changePixelColor(1,1,sf::Color::Blue);
        if(sf::Mouse::getPosition(window).y >= 0 && sf::Mouse::getPosition(window).y < 160 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            std::cout << "Matrix1\n";
            painting1.changeLedPixelColor();
        }
        if(sf::Mouse::getPosition(window).y >= 161 && sf::Mouse::getPosition(window).y < 320 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            std::cout << "Matrix2\n";
            painting2.changeLedPixelColor();
        }
        if(sf::Mouse::getPosition(window).y >= 321 && sf::Mouse::getPosition(window).y < 480 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            std::cout << "Matrix3\n";
            painting3.changeLedPixelColor();
        }

        colorwheel.colorInterface();
        matrix1.draw();
        matrix2.draw();
        matrix3.draw();
        window.display();

        sf::sleep( sf::milliseconds( 20));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                matrix1.writeMatrixToFile("matrix1.txt");
                matrix2.writeMatrixToFile("matrix2.txt");
                matrix3.writeMatrixToFile("matrix3.txt");
                std::cout << "Closed\n";
                window.close();
            }
        }
    }
    return 0;
}