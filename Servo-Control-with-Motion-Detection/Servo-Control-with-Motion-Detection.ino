#include <Servo.h>

int movPin = 8;
int servoPin = 9;
int movement;

Servo servo;

void setup() {
  servo.attach(servoPin);
  pinMode(movPin, INPUT);
}

void loop() {
  movement = digitalRead(movPin);

  if(movement == HIGH) {
    servo.write(150);
    delay(250);
    
    servo.write(30);
    delay(250);
    
    servo.write(150);
    delay(250);
    
    servo.write(30);
    delay(250);
    
    servo.write(150);
    delay(250);
    
    servo.write(30);
    delay(250);

    servo.write(90);
  } else {
    motor.write(90);
  }
}
