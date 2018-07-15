#ifndef DYNAMIXEL
#define DYNAMIXEL

#include <Arduino.h>

#ifndef DYNAMIXEL_SERIAL
#define DYNAMIXEL_SERIAL Serial
#endif

#ifndef DYNAMIXEL_BAUD
#define DYNAMIXEL_BAUD 57142
#endif

class DynamixelController
{
  private:
    byte motorID;

  public:
    DynamixelController(byte id)
    {
        motorID = id;
    }

    void begin();
    void setPosition(int position, int speed);
};

#endif