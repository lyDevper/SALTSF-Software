#include "Servo.h"

Servo servo1;
Servo servo2;

#define dcmotor_pin 11

void setup() {
  pinMode(dcmotor_pin, OUTPUT);
  servo1.attach(10);
  servo2.attach(9);

  servo1.write(0);
  servo2.write(0);

  delay(2000);

  servo1.write(90);
  servo2.write(90);

  delay(2000);

  servo1.write(0);
  servo2.write(0);

  delay(2000);

  servo1.write(90);
  servo2.write(0);

  delay(2000);

}

void loop() {
  // put your main code here, to run repeatedly:
}
