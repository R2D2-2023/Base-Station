#include <Arduino.h>
#include "HX711.h"

int pin1 = 2;
int pin2 = 3;
int pin3 = 4;
int pin4 = 5;
int motorPin1 = 8;
int motorPin2 = 9;

HX711 scale;

uint8_t dataPin = SDA;
uint8_t clockPin = SCL;

uint32_t offset = 66483;
float set_Scale = -140.714996;

float volatile f;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
  pinMode(pin4, INPUT);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  Serial.begin(115200);
  scale.begin(dataPin, clockPin);
  scale.set_scale(set_Scale);
  scale.set_offset(offset);



}

void loop() {
  if(!digitalRead(pin4)){
      f = scale.get_units(10);
      if(f < 2000){
        if(!digitalRead(pin1)){
            //Arm goes up.
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, LOW);
        }
        if(!digitalRead(pin2)){
            // Arm is max up.
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, HIGH);

        }
      }else{
        //The load is to high
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, HIGH);
      }
  }else{
    //reverse if robot leaves
    if(digitalRead(pin1)){
        //reverse if robot leaves and he isnt at lowwest possition
     digitalWrite(motorPin2,HIGH);
     digitalWrite(motorPin1, LOW);
    else{
        //if at lowest possition and robot isn't there it stays. 
        digitalWrite(motorPin2,HIGH);
        digitalWrite(motorPin1, HIGH);
    }
    }
  }
}