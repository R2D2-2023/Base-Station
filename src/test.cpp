#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "HX711.h"
#include <wiringSerial.h>
#include <sstream>

int clock_pin = 9;
int data_pin1 = 2;
int data_pin2 = 3;

int light_sensor1 = 25;
int light_sensor2 = 24;

int hall_sensor_1 = 4;
int hall_sensor_2 = 5;

int motor_pin_1 = 21;
int motor_pin_2 = 22;

//int relais_pin = 23; 

HX711 scale_1(clock_pin, data_pin1);
HX711 scale_2(clock_pin, data_pin2);

bool hall_sensor_detected = 0;

bool light_sensor1_detected = 0;
bool light_sensor2_detected = 0;

bool robot_charging = 0;

char serial_port[12];

char serial_text[3]; // max length for serial string.

//bool test= false;

int main() {
   	wiringPiSetup();
   	
	serialOpen("/dev/ttyACM0	", 115200);
	serialFlush(4);

    std::cout << "Hello, World!" << std::endl;
    scale_1.start(1058);// this works to calabrate the scale
    scale_2.start(-185);//  ^^
    pinMode(light_sensor1, INPUT);
	pinMode(light_sensor2, INPUT);
	pinMode(hall_sensor_1, INPUT);
	pinMode(hall_sensor_2, INPUT);
	pinMode(motor_pin_1, OUTPUT);
	pinMode(motor_pin_2, OUTPUT);
	//pinMode(relais_pin, OUTPUT);
	
	digitalWrite(motor_pin_1, HIGH);
	digitalWrite(motor_pin_2, HIGH);
	//digitalWrite(relais_pin, LOW);


	std::cout << "Starting in 3" << std::endl;
	delay(1000);
	std::cout << "Starting in 2" << std::endl;
	delay(1000);
	std::cout << "Starting in 1" << std::endl;
	delay(1000);
    
    while (1){
		serialFlush(4);
		//digitalWrite(relais_pin, test);
		//test = !test;
		
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
		
		if(light_sensor1_detected && light_sensor2_detected){	
			// robot has entered and is in position.
			digitalWrite(motor_pin_1, HIGH);
			digitalWrite(motor_pin_2, LOW);
			while(!hall_sensor_detected){
				if(digitalRead(hall_sensor_1)){
					hall_sensor_detected = true;
				}	
			}
			std::cout << "Robot will move down.\n";
		}
		
	    if (grams1 >= 4000 && grams2 <= 2000){
			digitalWrite(motor_pin_1, LOW);
			digitalWrite(motor_pin_2, LOW);
			std::cout << "Opladen + schrijven" << std::endl;
			delay(1000);
			/*
			try {
				int system_result = system("/home/pi/BaseStationBashSkrippie/uploadArduino");
				if(system_result == -1){
					throw std::runtime_error("system call failed");
				}
			}
			
			catch(const std::exception& error){
				//set error for not uploading. 
				std::cout<< "Error:  " << error.what() << std::endl;
			}*/
			
			delay(5000);
			robot_charging = 1;
		}
		
		while(robot_charging){
			//std::cout << serialDataAvail(4) << std::endl;
			std::cout<<"charging\n";
			for (int i = 0; i <= 3; i++){
				std::cout << "test";
				serial_text[i] = serialGetchar(4);
			}
						
			if(serial_text[0] == 'h' &&serial_text[1] == 'e' && serial_text[2] == 'y'){
				//charging done, arm starts going up. 
				std::cout << "stop charging received\n";
				digitalWrite(motor_pin_1, LOW);
				digitalWrite(motor_pin_2, HIGH);
				light_sensor1_detected = 0;
				light_sensor2_detected = 0;
				robot_charging = 0;
			}
			delay(1000);
			
		}
		
		if (!digitalRead(hall_sensor_1) && !light_sensor1_detected && !light_sensor2_detected && hall_sensor_detected){
			// arm is full retracted
			std::cout << "retracted" << std::endl;			
			digitalWrite(motor_pin_1, LOW);
			digitalWrite(motor_pin_2, LOW);
			hall_sensor_detected = !hall_sensor_detected;
		}
		
		else if (!digitalRead(hall_sensor_2)){
			//arm is fully extended
			std::cout << "FULLY EXTENDED RETRACTING..." << std::endl;
			digitalWrite(motor_pin_1, LOW);
			digitalWrite(motor_pin_2, HIGH);
			light_sensor1_detected = 0;
			light_sensor2_detected = 0;
			// Throw error, there is no robot here, or it became smaller then it used to be. 
			delay(2000);
		}
	delay(100);
	}
    return 0;
}
