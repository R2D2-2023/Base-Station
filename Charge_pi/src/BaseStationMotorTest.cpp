#include <wiringPi.h>
#include <stdio.h>
#include <iostream>
#include "HX711.h"

int pin1 = 2;
int pin2 = 3;
int pin4 = 5;
int motorPin1 = 8;
int motorPin2 = 9;

HX711 scale;

uint8_t dataPin = 0;
uint8_t clockPin = 1;

uint32_t offset = 66483;
float set_Scale = -140.714996;

float volatile f;


int main() {
  // put your setup code here, to run once:
  wiringPiSetup();
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin4, INPUT);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  
  scale.begin(dataPin, clockPin);
  scale.set_scale(set_Scale);
  scale.set_offset(offset);

while(1) {
  f = scale.get_units(5);
  std::cout << "load:" << f << std::endl;
  delay(250);

  if(!digitalRead(pin4)){
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      f = scale.get_units(10);
      std::cout << "pin 4 goed \n";
      if(f < 2000){
        std::cout << "load cell goed \n";
        if(!digitalRead(pin1)){
          std::cout << "pin 1 goed, arm up\n";
            //Arm goes up.
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, LOW);
        }
        if(!digitalRead(pin2)){
          std::cout << "pin 2 goed, arm still \n";
            // Arm is max up.
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);

        }
      }else{
        std::cout << "load cell too high \n";
        //The load is to high
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
      }
  }else{
    std::cout << "robot left\n";
    //reverse if robot leaves
    if(digitalRead(pin1)){
      std::cout << "robot left, arm going down \n"; 
        //reverse if robot leaves and he isnt at lowwest possition
     digitalWrite(motorPin2,HIGH);
     digitalWrite(motorPin1, LOW);
    }else{
        //if at lowest possition and robot isn't there it stays.
        std::cout << "robot left, arm is down \n";
        digitalWrite(motorPin2,LOW);
        digitalWrite(motorPin1, LOW);
    }
  }
}
}
