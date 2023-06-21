#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "HX711.h"

int clock_pin = 0;
int data_pin1 = 2;
int data_pin2 = 3;

int hall_sensor_1 = 4;
int hall_sensor_2 = 5;

int motor_pin_1 = 21;
int motor_pin_2 = 22;


HX711 scale_1(clock_pin, data_pin1);
HX711 scale_2(clock_pin, data_pin2);

bool motor1_aan = 0;
bool motor2_aan = 1;
bool sensor_detected = 0;

int main() {
    wiringPiSetup();
    std::cout << "Hello, World!" << std::endl;
    scale_1.start(66483);// this works to calabrate the scale
    scale_2.start(66483);//  ^^
    pinMode(hall_sensor_1, INPUT);
	pinMode(hall_sensor_2, INPUT);
	pinMode(motor_pin_1, OUTPUT);
	pinMode(motor_pin_2, OUTPUT);

	std::cout << "Starting in 3" << std::endl;
	delay(1000);
	std::cout << "Starting in 2" << std::endl;
	delay(1000);
	std::cout << "Starting in 1" << std::endl;
	delay(1000);
    
    while (1){
		//digitalWrite(motor_pin_1, 1);
		//digitalWrite(motor_pin_2, 0);
		//delay(3000);
		//std::cout << "draaien" <<std::endl;
		//digitalWrite(motor_pin_1, 0);
		//digitalWrite(motor_pin_2, 1);
		//delay(3000);
	//}
        int grams1 = scale_1.getAvg(2);
	int grams2 = scale_2.getAvg(2);
        std::cout << "avg1:" << grams1 << std::endl;
	std::cout << "avg2:" << grams2 << std::endl;
        if (grams1 <= -1500 || grams2 <= -1500){
			digitalWrite(motor_pin_1, LOW);
			digitalWrite(motor_pin_2, LOW);
			std::cout << "Opladen + schrijven" << std::endl;
			delay(1000);
			
			system("/home/pi/BaseStationBashSkrippie/uploadArduino");
			delay(5000);
			
			motor1_aan = !motor1_aan; 
			motor2_aan = !motor2_aan;
			digitalWrite(motor_pin_1, motor1_aan) ;
			digitalWrite(motor_pin_2, motor2_aan) ;
			sensor_detected = 0;
		}
		if (!digitalRead(hall_sensor_1)){
			if (sensor_detected != 1){
				std::cout << "Bottom" << std::endl;
				motor1_aan = !motor1_aan; 
				motor2_aan = !motor2_aan;
				digitalWrite(motor_pin_1, motor1_aan) ;
				digitalWrite(motor_pin_2, motor2_aan) ;
				}
			sensor_detected = 1;
		}
		else if (!digitalRead(hall_sensor_2)){
			if (sensor_detected != 0){
				std::cout << "Top" << std::endl;
				motor1_aan = !motor1_aan;
				motor2_aan = !motor2_aan;
				digitalWrite(motor_pin_1, motor1_aan) ;
				digitalWrite(motor_pin_2, motor2_aan) ;
			}
			sensor_detected = 0;
		}
		delay(100);
	}

    return 0;
}
