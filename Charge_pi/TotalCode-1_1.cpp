#include <Arduino.h>
#include "HX711.h"

int pin1 = 2;
int pin2 = 3;
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
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin4, INPUT);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  Serial.begin(115200);
  scale.begin(dataPin, clockPin);
  scale.set_scale(set_Scale);
  scale.set_offset(offset);
}

void loop() {
  f = scale.get_units(10);
//  Serial.print("load: ");
//  Serial.println(f);
//  Serial.print("h1: ");
//  Serial.println(digitalRead(pin1));
//  Serial.print("h2: ");
//  Serial.println(digitalRead(pin2));
//  Serial.print("h4: ");
//  Serial.println(digitalRead(pin4));
  delay(250);

  if(!digitalRead(pin4)){
      digitalWrite(motorPin1, LOW);
      digitalWrite(motorPin2, LOW);
      f = scale.get_units(10);
      Serial.println("pin4 Goed");
      if(f < 2000){
        Serial.println("Loadcel goed");
        if(!digitalRead(pin1)){
          Serial.println("pin1 goed, arm Up");
            //Arm goes up.
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, LOW);
        }
        if(!digitalRead(pin2)){
          Serial.println("pin2 goed, Arm Stil");
            // Arm is max up.
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);

        }
      }
      else{
        Serial.println(f);
        Serial.println("Load is to HIGH");
        //The load is to high
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
      }
  }
  else{
    Serial.println("Robot left");
    //reverse if robot leaves
    if(digitalRead(pin1)){
      Serial.println("Robot left, arm going down");
        //reverse if robot leaves and he isnt at lowwest possition
     digitalWrite(motorPin2,HIGH);
     digitalWrite(motorPin1, LOW);
    }
    else{
        //if at lowest possition and robot isn't there it stays.
        Serial.println("robot left, arm is down. ");
        digitalWrite(motorPin2,LOW);
        digitalWrite(motorPin1, LOW);
    }
  }
}
