#include <PS4BT.h>
#include <usbhub.h>
#include <SPI.h>

#include "gyro.hpp"
#include "motor.hpp"
#include "relay.hpp"
#include "line_analog.hpp"
#include "srf_reader.hpp"

#define DX_STEP_0 0
#define DX_STEP_1 180
#define DX_STEP_2 300
#define SH_PIN 10

#define REST_DELAY 2000
#define FRONT_WALL_DISTANCE 800
#define SETUP_PIN 46
#define ROBOT_SPEED 200

int robotState = 1;
byte count_of_elements = 0;
signed short readed_gyro = 0;
Gyro gyro;
MotorController motorCtrl;
RelayContoller relayCtrl;
LineController lineCtrl = LineController();
SRFReader rightSRF;
SRFReader leftSRF;

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

void manualRobot();

void setup()
{
  motorCtrl.begin();
  Serial.begin(115200);
  Serial.println("Working on setup function ...");
  pinMode(SETUP_PIN, INPUT);

  Wire.begin();

  rightSRF = SRFReader(0x70);
  leftSRF = SRFReader(0x7A);

  relayCtrl.begin();
  relayCtrl.led(HIGH);
  gyro.begin();

  if (digitalRead(SETUP_PIN))
  {
    lineCtrl.begin();
    for (int i = 0; i < 2000; i++)
    {
      bool dir = i > 500 && i < 1500;
      motorCtrl.go(dir ? 180 : 0, 75, gyro.read());
      for (byte j = 0; j < 10; j++)
        lineCtrl.updateThreshold(j);
      delay(1);
    }
    findTheLine(1);
    motorCtrl.stop();
    lineCtrl.save();
  }
  delay(2000);

  lineCtrl.load();

  relayCtrl.led(LOW);
  gyro.zero();

  Serial.println("Setup has been finished !");
}

void loop()
{
  readed_gyro = gyro.read();
  lineFollower();
  // Serial.println(readed_gyro);
  // printLineSensors();
  // delay(20);
}

void printLineSensors()
{
  lineCtrl.readAll();
  Serial.println("-----------------------------");
  for (byte i = 0; i < 10; i++)
  {
    Serial.print(lineCtrl.isLine(i));
    Serial.print(" ");
  }
  Serial.print(lineCtrl.count());
  Serial.println();
}

void goingToFrontWall(int position)
{
  int err = ((rightSRF.read() + leftSRF.read()) / 2) - position;
  while (abs(err) > 50)
  {
    motorCtrl.go(err > 0 ? 270 : 90, 60, gyro.read());

    err = ((rightSRF.read() + leftSRF.read()) / 2) - position;
  }
  motorCtrl.stop();
}

void forwardToLine()
{
  lineCtrl.readAll();
  while (lineCtrl.count() < 7)
  {
    motorCtrl.go(270 - lineCtrl.error(), 75, gyro.read());
    lineCtrl.readAll();
  }
  motorCtrl.stop();
}

void backToLine()
{
  lineCtrl.readAll();
  while (lineCtrl.count() < 7)
  {
    motorCtrl.go(90 + lineCtrl.error(), 75, gyro.read());
    lineCtrl.readAll();
  }
  motorCtrl.stop();
}

void eqaulFrontWall()
{
  int timeout = 0, sigma = 0, last_err = 0;
  while (timeout++ < 60)
  {
    int err = leftSRF.read() - rightSRF.read() - 30;
    if (last_err != 0)
      err = last_err * 0.1 + err * 0.9;
    last_err = err;

    if (abs(err) < 5)
      break;
    motorCtrl.turnCenter(err * 0.4 + sigma * 0.0025);
  }
  motorCtrl.stop();
}

void lineFollower()
{
  lineCtrl.readAll();
  // printLineSensors();
  if (robotState == -1)
  {
    motorCtrl.stop();
  }
  else if (robotState == 0)
  {
    // start moving
    motorCtrl.go(270 - lineCtrl.error(), 100, readed_gyro);
    if (lineCtrl.count() == 0)
    {
      motorCtrl.turnRight(gyro);
      findTheLine(2);
      robotState = 1;
    }
  }
  else if (robotState == 1)
  {
    motorCtrl.go(270 - lineCtrl.error(), 255, readed_gyro);
    if (lineCtrl.count() > 5) // + 1
    {
      motorCtrl.stop();
      motorCtrl.turnLeft(gyro);
      findTheLine(2);
      robotState = 2;
    }
  }
  else if (robotState == 2 || robotState == 3)
  {
    motorCtrl.go(270 - lineCtrl.error(), 170, readed_gyro);
    if (lineCtrl.count() > 5) // +2
    {
      if (robotState == 2)
      {
        backToLine();
        waitForBall();
        delay(REST_DELAY);
        robotState = 3;
      }
      else
      {
        motorCtrl.stop();
        motorCtrl.turnLeft(gyro);
        findTheLine(1);

        goingToFrontWall(600);
        eqaulFrontWall();
        gyro.zero();
        relayCtrl.sendCommand(RELAY_HIGH);
        goingToFrontWall(3000);
        motorCtrl.turnLeft(gyro);
        findTheLine(1);
        robotState = 4;
      }
    }
  }
  else if (robotState == 4 || robotState == 5)
  {
    motorCtrl.go(270 - lineCtrl.error(), 230, readed_gyro);
    if (lineCtrl.count() > 5) // + 3
    {
      if (robotState == 4)
      {
        delay(100);
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
    int speed = 255;
    if (robotState == 7)
      speed = 100;
    motorCtrl.go(270 - lineCtrl.error(), speed, readed_gyro);
    if (lineCtrl.count() > 5) // + 5
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
        backToLine();
        waitForBall();
        delay(REST_DELAY);
        robotState = 8;
      }
      else if (robotState == 8)
      {
        motorCtrl.turnLeft(gyro);
        findTheLine(2);
        goingToFrontWall(800);
        eqaulFrontWall();
        gyro.zero();
        relayCtrl.sendCommand(RELAY_MEDIUM);
        goingToFrontWall(3000);
        motorCtrl.turnRight(gyro);
        findTheLine(1);
        robotState = 9;
        count_of_elements = 0;
      }
    }
  }
  else if (robotState == 9)
  {
    motorCtrl.go(90 + lineCtrl.error(), 100, readed_gyro);
    if (lineCtrl.count() > 5)
    {
      count_of_elements++;
      delay(200);
      if (count_of_elements == 2)
      {
        motorCtrl.stop();
        waitForBall();
        delay(REST_DELAY);
        gyro.zero();
        robotState = 10;
        count_of_elements = 0;
      }
    }
  }
  else if (robotState == 10)
  {
    motorCtrl.go(270 - lineCtrl.error(), ROBOT_SPEED, readed_gyro);
    if (lineCtrl.count() > 5)
    {
      count_of_elements++;
      delay(2e4 / ROBOT_SPEED);
      if (count_of_elements == 5)
      {
        motorCtrl.turnLeft(gyro);
        findTheLine(2);
        goingToFrontWall(600);
        eqaulFrontWall();
        gyro.zero();
        relayCtrl.sendCommand(RELAY_LOW);
        goingToFrontWall(3000);
        motorCtrl.turnLeft(gyro);
        count_of_elements = 0;
        robotState = 11;
      }
    }
  }
  else if (robotState == 11)
  {
    motorCtrl.go(270 - lineCtrl.error(), ROBOT_SPEED, readed_gyro);
    {
      if (lineCtrl.count() > 5)
      {
        count_of_elements++;
        delay(2e4 / ROBOT_SPEED);
      }
      if (count_of_elements == 5)
      {
        motorCtrl.turnRight(gyro);
        findTheLine(1);
        robotState = 12;
        count_of_elements = 0;
      }
    }
  }
  else if (robotState == 12)
  {
    motorCtrl.go(270 - lineCtrl.error(), ROBOT_SPEED, readed_gyro);
    if (lineCtrl.count() == 0)
    {
      motorCtrl.turnRight(gyro);
      robotState = -1;
    }
  }
}

void waitForBall()
{
  motorCtrl.stop();
  int count = 0;
  while (count < 3)
  {
    if (digitalRead(SH_PIN))
    {
      count += 1;
      delay(10);
    }
  }
}

void findTheLine(byte mode)
{
  lineCtrl.readAll();
  int err = lineCtrl.error();
  int count = lineCtrl.count();
  int timeout = 0;
  while (timeout++ < 300)
  {
    readed_gyro = gyro.read();
    int speed = 0;
    if (count == 0)
      speed = 50;
    else
      speed = abs(err) * 5;

    if (mode == 1)
      motorCtrl.go(180, speed, readed_gyro);
    else if (mode == 2)
      motorCtrl.go(0, speed, readed_gyro);

    lineCtrl.readAll();
    err = lineCtrl.error();
    count = lineCtrl.count();

    if (count != 0 && err == 0)
      break;
  }
  motorCtrl.stop();
}