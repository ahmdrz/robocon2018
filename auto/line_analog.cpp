#include "line_analog.hpp"
#include <Arduino.h>
#include <EEPROM.h>

void LineController::begin()
{
    for (byte i = 0; i < 10; i++)
        analogOffsets[i] = read(i);
}

int LineController::read(int index)
{
    int result = analogRead(sensorPins[index]) - analogOffsets[index];
    return result;
}

void LineController::readAll()
{
    for (byte i = 0; i < 10; i++)
    {
        sensorValues[i] = read(i);
    }
}

bool LineController::isLine(int index)
{
    return sensorValues[index] <= (thresholds[index] * 0.7);
}

int LineController::count()
{
    int count = 0;
    for (byte i = 0; i < 10; i++)
        count += isLine(i);
    return count;
}

void LineController::updateThreshold(int index)
{
    int result = read(index);
    if (result < thresholds[index])
        thresholds[index] = result;
}

int LineController::threshold(int index)
{
    return thresholds[index];
}

int LineController::error()
{
    int sum = 0;
    int count = 0;
    for (byte i = 0; i < 10; i++)
    {
        if (isLine(i))
        {
            sum += errorValues[i];
            count++;
        }
    }
    if (count == 0)
    {
        return 0;
    }
    return (sum / count) * 1.5;
}

void LineController::save()
{
    for (byte i = 0; i < 10; i++)
    {
        int lsb = analogOffsets[i] % 256;
        int msb = analogOffsets[i] >> 8;
        EEPROM.write(10 + i, lsb);
        EEPROM.write(20 + i, msb);
    }

    for (byte i = 0; i < 10; i++)
    {
        int lsb = thresholds[i] % 256;
        int msb = thresholds[i] >> 8;
        EEPROM.write(30 + i, lsb);
        EEPROM.write(40 + i, msb);
    }
}

void LineController::load()
{
    for (byte i = 0; i < 10; i++)
    {
        int lsb = EEPROM.read(10 + i);
        int msb = EEPROM.read(20 + i);
        analogOffsets[i] = msb << 8 | lsb;
    }

    for (byte i = 0; i < 10; i++)
    {
        int lsb = EEPROM.read(30 + i);
        int msb = EEPROM.read(40 + i);
        thresholds[i] = msb << 8 | lsb;
    }
}

int LineController::get(int index)
{
    return sensorValues[index];
}