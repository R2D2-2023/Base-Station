int pin1 = 2;
int pin2 = 3;
int pin3 = 4;
int pin4 = 5;
int motorPin1 = 8;
int motorPin2 = 9;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
  pinMode(pin4, INPUT);

  pinMode(motorPin1, OUTPUT);  
  pinMode(motorPin2, OUTPUT);
}

void loop() {
  if(!digitalRead(pin4)){
      if(!digitalRead(pin1)){
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2, LOW);
        //up
      }
      if(!digitalRead(pin2)){
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, HIGH);
        //down
        }
        }else{
          //stop if robot leaves
        digitalWrite(motorPin1, HIGH);
        digitalWrite(motorPin2,HIGH);
          }
    }
