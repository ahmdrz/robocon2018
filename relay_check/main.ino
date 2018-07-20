#include "relay.hpp"

RelayContoller relayCtrl;

void setup()
{
    relayCtrl.begin();
}

void loop()
{
    relayCtrl.begin();
    delay(5000);
    digitalWrite(RL_STEP_1, HIGH);
    digitalWrite(RL_STEP_2, HIGH);
    digitalWrite(RL_STEP_3, LOW);
    digitalWrite(RL_LOW, HIGH);
    delay(5000);
}