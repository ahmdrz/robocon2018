#include "dynamixel.hpp"

void DynamixelController::setPosition(int position, int speed)
{
    unsigned char loVel = speed % 256;
    unsigned char hiVel = speed >> 8;
    unsigned char loPos = position % 256;
    unsigned char hiPos = position >> 8;
    unsigned char checksum = (~(motorID + 7 + 3 + 30 + loVel + hiVel + loPos + hiPos)) & 0xFF;
    unsigned char packet[11] = {0xFF, 0xFF, motorID, 0x07, 0x03, 0x1E, position, position >> 8, speed, speed >> 8, checksum};
    for (char i = 0; i < 11; i++)
        DYNAMIXEL_SERIAL.write(packet[i]);
}

void DynamixelController::begin()
{
    DYNAMIXEL_SERIAL.begin(DYNAMIXEL_BAUD);
}