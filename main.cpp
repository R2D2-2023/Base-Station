#include <iostream>
//#include <SFML/Graphics.hpp>
#include "Led.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    sf::RenderWindow window(sf::VideoMode{320, 480}, "LED Visualisation");

    //Led led(window, sf::Vector2f(5,5));

    while (window.isOpen())
        window.clear();

        //Hier alle draws
    //    led.draw(window);

     //   window.display();

     //   sf::sleep( sf::milliseconds( 20));

        sf::Event event;
        while( window.pollEvent(event) ){
            if( event.type == sf::Event::Closed ){
                window.close();
            }
        }
    return 0;
}
