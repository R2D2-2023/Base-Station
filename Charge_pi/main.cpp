#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "HX711.h"
#include <wiringSerial.h>
#include <sstream>
#include <string>

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

int state = 0; //0 = idle, 1 = robot entering, 2 = moving down, 3 = charging, 4 = going up

char serial_port[12];
char serial_text[3]; // max length for serial string.


void setup(){
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
}

int main() {
	setup();
	int timer = millis();
	while (1){
		switch(state){
				case 0:{
					if (digitalRead(light_sensor1)){
						//robot started entering
						std::cout << "Sensor1\n";
						state = 1;
					}
				break;}
				case 1:{
					timer = millis();
					if(millis() - timer >= 5000){
						state = 0;
					}
					if (digitalRead(light_sensor2)){
						//robot has entered
						std::cout << "Sensor2\n";
						digitalWrite(motor_pin_1, HIGH);
						digitalWrite(motor_pin_2, LOW);
						state = 2;
					}
				break;}
				case 2:{
					std::cout << "Robot will move down.\n";
					if(!digitalRead(hall_sensor_2)){
						std::cout << "FULLY EXTENDED RETRACTING..." << std::endl;
						digitalWrite(motor_pin_1, LOW);
						digitalWrite(motor_pin_2, HIGH);
						state = 4;
					}
					
					int grams1 = scale_1.getAvg(2);
					int grams2 = scale_2.getAvg(2);
					std::cout << "avg1:" << grams1 << "\t";
					std::cout << "avg2:" << grams2 << std::endl;
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
						state = 3;
					}
				break;}
				case 3:{
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
						state = 4;
					}

				break;}
				case 4:{
					if (!digitalRead(hall_sensor_1)){
						// arm is full retracted
						std::cout << "retracted" << std::endl;			
						digitalWrite(motor_pin_1, LOW);
						digitalWrite(motor_pin_2, LOW);
						state = 0;
					}
				break;}
				
		}
		delay(1000);
	}
}
