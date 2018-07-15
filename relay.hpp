#ifndef RELAYCTRL
#define RELAYCTRL

#include <Arduino.h>

#define RELAY_HIGH 3
#define RELAY_MEDIUM 2
#define RELAY_LOW 1

#define HIGH_TIME 500
#define LOW_TIME 1500

class RelayContoller
{
  public:
    void begin();

    void sendCommand(byte cmd);
};

#endif