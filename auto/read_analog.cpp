#include "read_analog.hpp"
#include <Arduino.h>

void AnalogController::begin()
{
    for (byte i = 0; i < 10; i++)
        analogOffsets[i] = read(i);
}

int AnalogController::read(int index)
{
    int result = analogRead(sensorPins[index]) - analogOffsets[index];    
    return result < 0 ? -result : result;
}

void AnalogController::readAll()
{
    for (byte i = 0; i < 10; i++) {
        sensorValues[i] = read(i);        
    }
}

bool AnalogController::isLine(int index)
{
    return sensorValues[index] >= (thresholds[index] * 0.7);
}

int AnalogController::count()
{
    int count = 0;
    for (byte i = 0; i < 10; i++)
        count += isLine(i);
    return count;
}

void AnalogController::updateThreshold(int index)
{
    int result = read(index);
    if (result > thresholds[index])
        thresholds[index] = result;
}

int AnalogController::error()
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
    return sum / count;
}