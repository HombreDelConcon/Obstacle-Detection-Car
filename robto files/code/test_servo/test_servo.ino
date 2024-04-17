#include <Servo.h>

Servo motor;

int pos = 0;

void setup() {
  // put your setup code here, to run once:
  motor.attach(9);
}

void loop() {
  motor.write(40);
  delay(1000);
  motor.write(90);
  delay(1000);
  motor.write(140);
  delay(1000);
  motor.write(90);
  delay(1000);
  /*  // put your main code here, to run repeatedly:
  for (int i = 0; i < 180; i++){
    motor.write(i);
    delay(20);
  }
  for (int j = 180; j > 0; j--){
    motor.write(j);
    delay(20);
  }*/
}
