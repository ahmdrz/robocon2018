#include "motor.hpp"
#include "gyro.hpp"

#include <Arduino.h>

signed long motor_val[91] = {0, 34, 69, 104, 139, 174, 209, 243, 278, 312, 347, 381, 415, 449, 483, 517, 551, 584, 618, 651, 684, 716, 749, 781, 813, 845, 876, 907, 938, 969, 1000, 1030, 1059, 1089, 1118, 1147, 1175, 1203, 1231, 1258, 1285,
                             1312, 1338, 1363, 1389, 1414, 1438, 1462, 1486, 1509, 1532, 1554,
                             1576, 1597, 1618, 1638, 1658, 1677, 1696, 1714, 1732, 1749, 1765, 1782, 1797, 1812, 1827,
                             1841, 1854, 1867, 1879, 1891, 1902, 1912, 1922, 1931, 1940, 1948, 1956, 1963, 1969, 1975,
                             1980, 1985, 1989, 1992, 1995, 1997, 1998, 1999, 2000};

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

void MotorController::begin()
{
    for (byte i = 0; i < 4; i++)
    {
        pinMode(motors[i].motorPin, OUTPUT);
        pinMode(motors[i].motorDir, OUTPUT);
        motorSendCommand(i, 0);
    }
}

void MotorController::debug()
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

void MotorController::stop()
{
    for (byte i = 0; i < 4; i++)
        motorSendCommand(i, 0);
}

void MotorController::turnLeft(Gyro gyro)
{
    gyro.zero();
    delay(10);
    signed short err = 90 - gyro.read(), sigma = 0, last_err = 0;
    int timeout = 0;
    int speed = 0;
    while (timeout++ < 5000)
    {
        speed = err * 5 + sigma * 0.0015 + (last_err - err) * 2;
        if (speed > 255)
            speed = 255;
        for (byte i = 0; i < 4; i++)
            motorSendCommand(i, speed);

        last_err = err;
        err = 90 - gyro.read();

        if (abs(err) < 2)
            break;
        else if (abs(err) < 15)
            sigma += err;
    }
    gyro.zero();
    delay(50);
    stop();
}

void MotorController::turnRight(Gyro gyro)
{
    gyro.zero();
    delay(10);
    signed short err = 90 + gyro.read(), sigma = 0, last_err = 0;
    int timeout = 0;
    int speed = 0;
    while (timeout++ < 5000)
    {
        speed = err * 5 + sigma * 0.0015 + (last_err - err) * 2;
        if (speed > 255)
            speed = 255;
        for (byte i = 0; i < 4; i++)
            motorSendCommand(i, -speed);

        last_err = err;
        err = 90 + gyro.read();

        if (abs(err) < 2)
            break;
        else if (abs(err) < 15)
            sigma += err;
    }
    gyro.zero();
    delay(50);
    stop();
}

void MotorController::turnCenter(int speed)
{
    for (byte i = 0; i < 4; i++)
        motorSendCommand(i, speed);
}

void MotorController::go(short angle, byte speed, signed short readed_gyro)
{
    signed short sum = 0, err = -readed_gyro;
    sum = err * 8 + last_err_motor * 3;
    last_err_motor = err;

    angle = angle + 90;
    if (angle > 360)
        angle = angle - 360;

    speed = speed > 0 ? speed : 0;
    speed = speed > 255 ? 255 : speed;

    float divisor = 0.0, sp = speed / 20.0;
    signed long angle_motor[4], speeds[4];

    if (speed != 0)
    {
        angle_motor[0] = angle - 45;
        angle_motor[1] = angle - 135;
        angle_motor[2] = angle - 225;
        angle_motor[3] = angle - 315;

        for (byte i = 0; i < 4; i++)
        {
            if (angle_motor[i] < 0)
                angle_motor[i] += 360;
            if (angle_motor[i] >= 0 && angle_motor[i] <= 90)
                speeds[i] = -motor_val[angle_motor[i]];
            else if (angle_motor[i] > 90 && angle_motor[i] <= 180)
                speeds[i] = -motor_val[90 - (angle_motor[i] - 90)];
            else if (angle_motor[i] > 180 && angle_motor[i] <= 270)
                speeds[i] = motor_val[(angle_motor[i] - 180)];
            else
                speeds[i] = motor_val[90 - (angle_motor[i] - 270)];
        }

        divisor = abs(speeds[0]);
        if (divisor < abs(speeds[1]))
            divisor = abs(speeds[1]);
        if (divisor < abs(speeds[2]))
            divisor = abs(speeds[2]);
        if (divisor < abs(speeds[3]))
            divisor = abs(speeds[3]);

        if (divisor > 0)
            divisor /= speed;
        else
            divisor /= -speed;

        for (byte i = 0; i < 4; i++)
        {
            speeds[i] /= divisor;
            speeds[i] += sum;
            if (speeds[i] > 255)
                speeds[i] = 255;
            if (speeds[i] < -255)
                speeds[i] = -255;

            motorSendCommand(i, speeds[i]);
        }
    }
}