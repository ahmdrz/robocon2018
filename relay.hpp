#ifndef RELAYCTRL
#define RELAYCTRL

#include <Arduino.h>

#ifndef RL_LOW
#define RL_LOW 24
#endif

#ifndef RL_STEP_1
#define RL_STEP_1 28
#endif

#ifndef RL_STEP_2
#define RL_STEP_2 26
#endif

#ifndef RL_STEP_3
#define RL_STEP_3 28
#endif

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