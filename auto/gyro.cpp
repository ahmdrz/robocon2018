#include "gyro.hpp"
#include <Wire.h>
#include <Arduino.h>

void Gyro::begin()
{
    Wire.begin();
    offset();
    delay(3000);
    zero();
    delay(1000);
}

void Gyro::offset()
{
    Wire.beginTransmission(gyroAddress);
    Wire.write(0x01);
    Wire.endTransmission();
}

void Gyro::zero()
{
    Wire.beginTransmission(gyroAddress);
    Wire.write(0x00);
    Wire.endTransmission();
}

signed short Gyro::read()
{
    byte lsb = 0;
    byte msb = 0;

    Wire.beginTransmission(gyroAddress);
    Wire.requestFrom(gyroAddress, 2);
    while (!Wire.available())
    {
    };
    lsb = Wire.read();
    msb = Wire.read();
    Wire.endTransmission();
    signed short result = msb << 8 | lsb;
    return result;
}