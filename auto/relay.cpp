#include "relay.hpp"
#include <Arduino.h>

void RelayContoller::begin()
{
    pinMode(RL_LOW, OUTPUT);
    digitalWrite(RL_LOW, HIGH);
    pinMode(RL_STEP_1, OUTPUT);
    digitalWrite(RL_STEP_1, HIGH);
    pinMode(RL_STEP_2, OUTPUT);
    digitalWrite(RL_STEP_2, HIGH);
    pinMode(RL_STEP_3, OUTPUT);
    digitalWrite(RL_STEP_3, HIGH);
    pinMode(RL_LED, OUTPUT);
    digitalWrite(RL_LED, HIGH);
}

void RelayContoller::led(int state)
{
    digitalWrite(RL_LED, !state);
}

void RelayContoller::sendCommand(byte cmd)
{
    int pin;
    switch (cmd)
    {
    case RELAY_HIGH:
        pin = RL_STEP_1;
        break;
    case RELAY_MEDIUM:
        pin = RL_STEP_2;
        break;
    case RELAY_LOW:
        pin = RL_STEP_3;
        break;
    }

    led(HIGH);
    delay(1000);

    digitalWrite(pin, LOW);
    delay(HIGH_TIME);
    digitalWrite(pin, HIGH);
    digitalWrite(RL_LOW, LOW);
    delay(LOW_TIME);
    digitalWrite(RL_LOW, HIGH);

    led(LOW);
}