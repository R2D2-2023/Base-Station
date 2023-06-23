#include "Matrix.h"
#include "ColorWheel.h"
#include "Painting.h"
#include "curl-8.1.2/curl.h"
#include "jsoncpp-1.9.5/json.h"

void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage)
{
    const sf::Vector2u originalImageSize{ originalImage.getSize() };
    const sf::Vector2u resizedImageSize{ resizedImage.getSize() };
    for (unsigned int y{ 0u }; y < resizedImageSize.y; ++y)
    {
        for (unsigned int x{ 0u }; x < resizedImageSize.x; ++x)
        {
            unsigned int origX{ static_cast<unsigned int>(static_cast<double>(x) / resizedImageSize.x * originalImageSize.x) };
            unsigned int origY{ static_cast<unsigned int>(static_cast<double>(y) / resizedImageSize.y * originalImageSize.y) };
            resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode{480, 370}, "LED Visualisation",  sf::Style::Close);

    Matrix matrix1(window, sf::Vector2f(0,0));
    Matrix matrix2(window, sf::Vector2f(16,0));
    Matrix matrix3(window, sf::Vector2f(32,0));
    ColorWheel colorwheel(window);
    Painting painting1(window, colorwheel, matrix1);
    Painting painting2(window, colorwheel, matrix2);
    Painting painting3(window, colorwheel, matrix3);

    sf::RectangleShape line(sf::Vector2f(480, 3));
    line.setPosition({0,320});

    sf::Texture background;
    background.create(481,321);
    sf::Sprite sprite;
    sf::Image image;
    sf::Image resized_image;
    resized_image.create(481,321);
    if (!image.loadFromFile("image.png")){
        std::cout << "No image.png found\n";
    }
    else{
        resizeImage(image, resized_image);
    }

    bool set_as_background = 0;
    bool draw_background = 1;

    if (set_as_background){
        background.update(resized_image);
        sprite.setTexture(background);
    }

    if (draw_background) {
        for (int y = 0; y < matrix1.matrix_vec.size(); ++y) {
            for (int x = 0; x < matrix1.matrix_vec[y].size(); ++x) {
                matrix1.matrix_vec[y][x].changeColor(resized_image.getPixel(x * 10, y * 10));
//            std::cout << std::to_string(resized_image.getPixel(x*10,y*10).r), std::to_string(resized_image.getPixel(x*10,y*10).g), std::to_string(resized_image.getPixel(x*10,y*10).b);
            }
        }
        for (int y = 0; y < matrix2.matrix_vec.size(); ++y) {
            for (int x = 0; x < matrix2.matrix_vec[y].size(); ++x) {
                matrix2.matrix_vec[y][x].changeColor(resized_image.getPixel((x + 16) * 10, y * 10));
//            std::cout << std::to_string(resized_image.getPixel(x*10,y*10).r), std::to_string(resized_image.getPixel(x*10,y*10).g), std::to_string(resized_image.getPixel(x*10,y*10).b);
            }
        }
        for (int y = 0; y < matrix3.matrix_vec.size(); ++y) {
            for (int x = 0; x < matrix3.matrix_vec[y].size(); ++x) {
                matrix3.matrix_vec[y][x].changeColor(resized_image.getPixel((x + 32) * 10, y * 10));
//            std::cout << std::to_string(resized_image.getPixel(x*10,y*10).r), std::to_string(resized_image.getPixel(x*10,y*10).g), std::to_string(resized_image.getPixel(x*10,y*10).b);
            }
        }
    }


    // Main loop.
    while (window.isOpen()) {
        window.clear();
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

        window.draw(sprite);
        colorwheel.colorInterface();
        matrix1.draw();
        matrix2.draw();
        matrix3.draw();
        window.display();


        sf::sleep( sf::milliseconds( 5));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                matrix1.writeMatrixToFile("Paneel1.txt");
                matrix2.writeMatrixToFile("Paneel2.txt");
                matrix3.writeMatrixToFile("Paneel3.txt");
                std::cout << "Closed and Saved.\n";
                window.close();
            }
        }
    }
    return 0;
}

