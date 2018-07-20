#include <PS4BT.h>
#include <usbhub.h>
#include <SPI.h>

#include "gyro.hpp"
#include "motor.hpp"
#include "relay.hpp"
#include "read_analog.hpp"

#define DX_STEP_0 0
#define DX_STEP_1 180
#define DX_STEP_2 300
#define ROBOT_SPEED 255

#define REST_DELAY 10000

int robotState = 0;

void findTheLine(byte mode);

byte count_of_elements = 0;
signed short readed_gyro = 0;
Gyro gyro;
MotorController motorCtrl;
RelayContoller relayCtrl;
AnalogController analogCtrl = AnalogController();

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

void manualRobot();

void setup()
{
  Serial.begin(115200);
  Serial.println("Connecting ...");

  motorCtrl.begin();
  gyro.begin();

  relayCtrl.begin();
  relayCtrl.led(HIGH);

  analogCtrl.begin();
  delay(500);
  for (int i = 0; i < 2000; i++)
  {
    bool dir = i > 500 && i < 1500;
    motorCtrl.go(dir ? 180 : 0, 75, gyro.read());
    for (byte j = 0; j < 10; j++)
      analogCtrl.updateThreshold(j);
    delay(1);
  }
  motorCtrl.stop();

  findTheLine(1);

  relayCtrl.led(LOW);

  gyro.zero();

  Serial.println("Setup has been finished");

  // motorCtrl.debug();
}

void loop()
{
  readed_gyro = gyro.read();

  analogCtrl.readAll();
  if (robotState == -1)
  {
    motorCtrl.stop();
  }
  else if (robotState == 0)
  {
    motorCtrl.go(270 - analogCtrl.error(), ROBOT_SPEED, readed_gyro);
    if (analogCtrl.count() > 5)
    {
      robotState = 1;
      motorCtrl.stop();
    }
  }
  else if (robotState == 1)
  {
    motorCtrl.turnLeft(gyro);
    motorCtrl.stop();
    findTheLine(2);
    robotState = 2;
  }
  else if (robotState == 2 || robotState == 3)
  {
    motorCtrl.go(270 - analogCtrl.error(), ROBOT_SPEED, readed_gyro);
    if (analogCtrl.count() > 5)
    {
      if (robotState == 2)
      {
        motorCtrl.stop();
        delay(REST_DELAY);
        robotState = 3;
      }
      else
      {
        motorCtrl.stop();
        motorCtrl.turnLeft(gyro);
        findTheLine(2);

        relayCtrl.sendCommand(RELAY_HIGH);
        motorCtrl.turnLeft(gyro);
        findTheLine(2);
        robotState = 4;
      }
    }
  }
  else if (robotState == 4 || robotState == 5)
  {
    motorCtrl.go(270 - analogCtrl.error(), ROBOT_SPEED, readed_gyro);
    if (analogCtrl.count() > 5)
    {
      if (robotState == 4)
      {
        delay(100);
        motorCtrl.stop();
        robotState = 5;
      }
      else
      {
        motorCtrl.stop();
        motorCtrl.turnLeft(gyro);
        findTheLine(2);
        robotState = 6;
      }
    }
  }
  else if (robotState < 9)
  {
    motorCtrl.go(270 - analogCtrl.error(), ROBOT_SPEED, readed_gyro);
    if (analogCtrl.count() > 5)
    {
      if (robotState == 6)
      {
        motorCtrl.turnLeft(gyro);
        findTheLine(2);
        robotState = 7;
      }
      else if (robotState == 7)
      {
        motorCtrl.stop();
        delay(REST_DELAY);
        robotState = 8;
      }
      else if (robotState == 8)
      {
        motorCtrl.turnLeft(gyro);
        findTheLine(2);
        relayCtrl.sendCommand(RELAY_HIGH);
        motorCtrl.turnRight(gyro);
        findTheLine(2);
        robotState = 9;
      }
    }
  }
  else if (robotState == 9)
  {
    motorCtrl.go(90 + analogCtrl.error(), ROBOT_SPEED / 2, readed_gyro);
    if (analogCtrl.count() > 5)
    {
      motorCtrl.stop();
      motorCtrl.go(270, 100, readed_gyro);
      delay(100);
      motorCtrl.stop();
      delay(REST_DELAY);
      robotState = 10;
      count_of_elements = 0;
    }
  }
  else if (robotState == 10)
  {
    motorCtrl.go(270 - analogCtrl.error(), ROBOT_SPEED, readed_gyro);
    if (analogCtrl.count() > 5)
    {
      count_of_elements++;
      if (count_of_elements == 4)
      {
        motorCtrl.turnLeft(gyro);
        findTheLine(2);
        relayCtrl.sendCommand(RELAY_HIGH);
        motorCtrl.turnLeft(gyro);
        findTheLine(2);
        count_of_elements = 0;
        robotState = -1;
      }
    }
  }
}

void findTheLine(byte mode)
{
  analogCtrl.readAll();
  int err = analogCtrl.error();
  int count = analogCtrl.count();
  int timeout = 0;
  while (timeout++ < 700)
  {
    readed_gyro = gyro.read();
    int speed = 0;
    if (count == 0)
      speed = 70;
    else
      speed = abs(err) * 7;

    if (mode == 1)
      motorCtrl.go(180, speed, readed_gyro);
    else if (mode == 2)
      motorCtrl.go(0, speed, readed_gyro);

    analogCtrl.readAll();
    err = analogCtrl.error();
    count = analogCtrl.count();

    if (count != 0 && err == 0)
      break;
  }
  motorCtrl.stop();
}