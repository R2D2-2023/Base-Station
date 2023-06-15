//
// Created by Erik on 15/06/2023.
//

#ifndef VISUALISE_LED_H
#define VISUALISE_LED_H
#include <SFML/Graphics.hpp>

class Led {
private:
    sf::RenderWindow &w;
    sf::Vector2f position;
    sf::RectangleShape shape;

public:
    Led(sf::RenderWindow &w, sf::Vector2f position);
    void draw(sf::RenderWindow &w);

};


#endif //VISUALISE_LED_H
