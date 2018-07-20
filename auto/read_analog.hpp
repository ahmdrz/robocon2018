#ifndef ANALOG
#define ANALOG

#ifndef THRESHOLD
#define THRESHOLD 100
#endif

#include <Arduino.h>

const int sensorPins[10] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
const int errorValues[10] = {-16, -8, -4, -2, 0, 0, 2, 4, 8, 16};

int *readSensors();

class AnalogController
{
private:
  int analogOffsets[10];
  int thresholds[10];
  int sensorValues[10];
  int read(int index);

public:
  AnalogController()
  {
    for (byte i = 0; i < 10; i++)
    {
      analogOffsets[i] = 0;
      thresholds[i] = 0;
      sensorValues[i] = 0;
    }
  }

  void begin();
  void readAll();
  void updateThreshold(int index);
  bool isLine(int index);
  int error();
  int count();
};

#endif
