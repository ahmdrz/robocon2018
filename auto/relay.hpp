#ifndef RELAYCTRL
#define RELAYCTRL

#include <Arduino.h>

#ifndef RL_LED
#define RL_LED 49
#endif

#ifndef RL_LOW
#define RL_LOW 33
#endif

#ifndef RL_STEP_1
#define RL_STEP_1 31
#endif

#ifndef RL_STEP_2
#define RL_STEP_2 35
#endif

#ifndef RL_STEP_3
#define RL_STEP_3 37
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

  void led(int state);
  void sendCommand(byte cmd);
};

#endif