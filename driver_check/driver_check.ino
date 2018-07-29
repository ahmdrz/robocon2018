#include "motor.hpp"

void setup()
{
  for (byte i = 0; i < 4; i++)
  {
    pinMode(motors[i].motorPin, OUTPUT);
    pinMode(motors[i].motorDir, OUTPUT);
    motorSendCommand(i, 0);
  }
}

void loop()
{
  for (byte j = 0; j < 4; j++)
  {
    for (int i = 0; i > -254; i--)
    {
      motorSendCommand(j, i);
      delay(5);
    }
    for (int i = -254; i < 0; i++)
    {
      motorSendCommand(j, i);
      delay(5);
    }
    for (int i = 0; i < 254; i++)
    {
      motorSendCommand(j, i);
      delay(5);
    }
    for (int i = 254; i > 0; i--)
    {
      motorSendCommand(j, i);
      delay(5);
    }
    delay(500);
  }
}
