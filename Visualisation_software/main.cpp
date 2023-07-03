#include "Dropbox.h"
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
        std::cerr << "No image.png found\nIf you want to use an image, place 'image.png' in the same folder as .exe\n";
    }
    else{
        resizeImage(image, resized_image);
    }

    std::cout << "To save file locally and upload to dropbox, close the drawing window.\n To close without saving, close the console.\n";

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
            }
        }
        for (int y = 0; y < matrix2.matrix_vec.size(); ++y) {
            for (int x = 0; x < matrix2.matrix_vec[y].size(); ++x) {
                matrix2.matrix_vec[y][x].changeColor(resized_image.getPixel((x + 16) * 10, y * 10));
            }
        }
        for (int y = 0; y < matrix3.matrix_vec.size(); ++y) {
            for (int x = 0; x < matrix3.matrix_vec[y].size(); ++x) {
                matrix3.matrix_vec[y][x].changeColor(resized_image.getPixel((x + 32) * 10, y * 10));
            }
        }
    }


    // Main loop.
    while (window.isOpen()) {
        window.clear();
        window.draw(line);


        if(sf::Mouse::getPosition(window).x >= 0 && sf::Mouse::getPosition(window).x < 160 && sf::Mouse::getPosition(window).y <= 320 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            painting1.changeLedPixelColor();
        }
        if(sf::Mouse::getPosition(window).x >= 161 && sf::Mouse::getPosition(window).x < 320 && sf::Mouse::getPosition(window).y <= 320 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            painting2.changeLedPixelColor();
        }
        if(sf::Mouse::getPosition(window).x >= 321 && sf::Mouse::getPosition(window).x < 480 && sf::Mouse::getPosition(window).y <= 320 && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
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
                std::string paneel1_txt = "paneel1.txt";
                std::string paneel2_txt = "paneel2.txt";
                std::string paneel3_txt = "paneel3.txt";
                matrix1.writeMatrixToFile(paneel1_txt);
                matrix2.writeMatrixToFile(paneel2_txt);
                matrix3.writeMatrixToFile(paneel3_txt);
                dropbox.run(paneel1_txt);
                dropbox.run(paneel2_txt);
                dropbox.run(paneel3_txt);
                window.close();
                std::cout << "Closed and Saved.\n";
            }
        }
    }
    return 0;
}

