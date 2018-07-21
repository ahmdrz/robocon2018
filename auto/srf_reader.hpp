#ifndef SRFREADER
#define SRFREADER

#include <Arduino.h>
#include <Wire.h>
#define CMD (byte)0x00
#define RANGEBYTE 0x02

class SRFReader
{
  private:
    int address;

  public:
    SRFReader() {
      
    }
    
    SRFReader(int add)
    {
        address = add;                        
        Wire.beginTransmission(address);
        Wire.write(0x02);
        Wire.write(0x50);
        Wire.endTransmission();

        Wire.beginTransmission(address);
        Wire.write(0x01);
        Wire.write(0x03);
        Wire.endTransmission();        
    }

    int read()
    {
        int range = 0;

        Wire.beginTransmission(address);
        Wire.write(CMD);
        Wire.write(0x52);
        Wire.endTransmission();

        delay(25);

        Wire.beginTransmission(address);
        Wire.write(RANGEBYTE);
        Wire.endTransmission();

        Wire.requestFrom(address, 2);
        while (Wire.available() < 2)
            ;
        byte highByte = Wire.read();
        byte lowByte = Wire.read();

        range = (highByte << 8) + lowByte;
        return range;
    }
};

#endif
