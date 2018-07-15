#ifndef MOTOR
#define MOTOR

#include <Arduino.h>
#include "gyro.hpp"

#if ROBOT_MODE == 0
#define MAX_DUTY_CYCLE 242
#else
#define MAX_DUTY_CYCLE 255
#endif

struct Motor
{
    byte motorDir;
    byte motorPin;
};

const Motor motors[4] = {{15, 4}, {14, 5}, {17, 2}, {16, 3}};

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