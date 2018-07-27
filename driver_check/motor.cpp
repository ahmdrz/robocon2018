#include "motor.hpp"
#include <Arduino.h>

void motorSendCommand(int index, int speed)
{
    byte dir = HIGH;
    if (speed < 0)
    {
        dir = LOW;
        speed = speed * -1;
    }
    if (speed > MAX_DUTY_CYCLE)
        speed = MAX_DUTY_CYCLE;
    digitalWrite(motors[index].motorDir, dir);
    analogWrite(motors[index].motorPin, speed);
}
