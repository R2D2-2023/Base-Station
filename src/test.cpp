#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "HX711.h"

int clock_pin = 9;
int data_pin1 = 2;
int data_pin2 = 3;

int light_sensor1 = 25;
int light_sensor2 = 24;

int hall_sensor_1 = 4;
int hall_sensor_2 = 5;

int motor_pin_1 = 21;
int motor_pin_2 = 22;


HX711 scale_1(clock_pin, data_pin1);
HX711 scale_2(clock_pin, data_pin2);

bool motor1_aan = 0;
bool motor2_aan = 1;
bool hall_sensor_detected = 0;

bool light_sensor1_detected = 0;
bool light_sensor2_detected = 0;

int main() {
   	wiringPiSetup();
    	std::cout << "Hello, World!" << std::endl;
    	scale_1.start(1058);// this works to calabrate the scale
    	scale_2.start(-185);//  ^^
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
		std::cout << "avg1:" << grams1 << "\t";
		std::cout << "avg2:" << grams2 << std::endl;
		
		if (digitalRead(light_sensor1)){
			//robot started entering
			light_sensor1_detected = true;
			std::cout << "Sensor1\n";
		}
		if (digitalRead(light_sensor2)){
			//robot has entered the basestation. The arm can go down now. 
			light_sensor2_detected = true;
			std::cout << "Sensor2\n";
		}
		
	    if (grams1 >= 4000 || grams2 <= 2000){
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
			hall_sensor_detected = 0;
		}
		if(light_sensor1_detected && light_sensor2_detected){
			// robot has entered and is in position.
			digitalWrite(motor_pin_1, motor1_aan);
			digitalWrite(motor_pin_2, motor2_aan);
			std::cout << "Robot will move down.\n";
		}
		
		if (!digitalRead(hall_sensor_1)){
			// arm is full retracted
			if (hall_sensor_detected != 1){
				//std::cout << "retracted" << std::endl;
				motor1_aan = !motor1_aan; 
				motor2_aan = !motor2_aan;
				digitalWrite(motor_pin_1, motor1_aan) ;
				digitalWrite(motor_pin_2, motor2_aan) ;
			}
		hall_sensor_detected = 1;
		}
		else if (!digitalRead(hall_sensor_2)){
			if (hall_sensor_detected != 0){
				//arm is fully extended
				//std::cout << "extended" << std::endl;
				motor1_aan = !motor1_aan;
				motor2_aan = !motor2_aan;
				digitalWrite(motor_pin_1, motor1_aan) ;
				digitalWrite(motor_pin_2, motor2_aan) ;
			}
		hall_sensor_detected = 0;
		}
		
	delay(100);
	}

    return 0;
}
