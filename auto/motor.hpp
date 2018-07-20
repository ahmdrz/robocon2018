#ifndef MOTOR
#define MOTOR

#include <Arduino.h>
#include "gyro.hpp"

struct Motor
{
    byte motorDir;
    byte motorPin;
};

#define MAX_DUTY_CYCLE 255
const Motor motors[4] = {{18, 4}, {19, 5}, {17, 3}, {16, 2}};

class MotorController
{
  private:
    short int last_err_motor;

  public:
    MotorController()
    {
        last_err_motor = 0;
    }
    void begin();
    void stop();
    void turnRight(Gyro gyro);
    void turnLeft(Gyro gyro);
    void go(short angle, byte speed, signed short readed_gyro);

    void debug();
};

void motorSendCommand(int index, int speed);

#endif