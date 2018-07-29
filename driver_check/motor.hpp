#ifndef MOTOR
#define MOTOR

#include <Arduino.h>

struct Motor
{
    byte motorDir;
    byte motorPin;
};

#define MAX_DUTY_CYCLE 255
const Motor motors[4] = {{18, 4}, {19, 5}, {17, 3}, {16, 2}};

void motorSendCommand(int index, int speed);

#endif