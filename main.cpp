#include <iostream>
#include <wiringPi.h>
#include "HX711.cpp"

int getAverage(int howMany);

HX711 test(0, 2);

int main() {
    wiringPiSetup();
    std::cout << "Hello, World!" << std::endl;
    test.start(66483, 1);
    while (1){
        std::cout << "avg:" << test.getAvg(10) << std::endl;
        delay(100);
    }
    return 0;
}