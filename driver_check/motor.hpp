#ifndef MOTOR
#define MOTOR

#include <Arduino.h>

struct Motor
{
    byte motorDir;
    byte motorPin;
};

#define MAX_DUTY_CYCLE 255
const Motor motors[4] = {{3, 2}};

void motorSendCommand(int index, int speed);

#endif