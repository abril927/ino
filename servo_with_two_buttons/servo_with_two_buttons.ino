// C++ code
//

#include <Servo.h>

const int SPEED = 1;
const int SERVO = 9;
const int BUTTON_LEFT = 10;
const int BUTTON_RIGHT = 11;

int pos = 90;

Servo servo;

void setup()
{
  servo.attach(SERVO, 500, 2500);
  pinMode(BUTTON_LEFT, INPUT);
  pinMode(BUTTON_RIGHT, INPUT);
}

void loop()
{
  int velocity = 0;
  if (digitalRead(BUTTON_LEFT) == HIGH) { velocity += SPEED; }
  if (digitalRead(BUTTON_RIGHT) == HIGH) { velocity -= SPEED; }
  
  if (velocity != 0) {
    pos += velocity;
    if (pos > 180) { pos = 180; }
    if (pos < 0) { pos = 0; }
    servo.write(pos);
  }
  delay(10);
}