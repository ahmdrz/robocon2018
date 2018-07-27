#include "motor.hpp"

void setup()
{
  pinMode(motors[0].motorPin, OUTPUT);
  pinMode(motors[0].motorDir, OUTPUT);
}

void loop()
{
  motorSendCommand(0, -200);
  delay(500);
  motorSendCommand(0, 0);
  delay(500);
  motorSendCommand(0, 200);
  delay(500);
}
