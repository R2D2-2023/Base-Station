int pin1 = 2;
int pin2 = 3;
int pin3 = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, INPUT);
}

void loop() {

  // put your main code here, to run repeatedly:
  // Motor draait de ene kant op.
if(!digitalRead(pin3)){
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    }else{
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
    // motor stopt me}
}
}
