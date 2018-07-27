#ifndef ANALOG
#define ANALOG

#ifndef THRESHOLD
#define THRESHOLD 100
#endif

#include <Arduino.h>

const int sensorPins[10] = {A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};
const int errorValues[10] = {-16, -8, -4, -2, 0, 0, 2, 4, 8, 16};

int *readSensors();

class LineController
{
private:
  int analogOffsets[10];
  int thresholds[10];
  int sensorValues[10];
  int read(int index);

public:
  LineController()
  {
    for (byte i = 0; i < 10; i++)
    {
      analogOffsets[i] = 0;
      thresholds[i] = 255;
      sensorValues[i] = 0;
    }
  }

  int get(int index);
  void begin();
  void readAll();
  void updateThreshold(int index);
  bool isLine(int index);
  int error();
  int count();
  int threshold(int index);
  void save();
  void load();
};

#endif
