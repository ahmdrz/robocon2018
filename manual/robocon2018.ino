#include <PS4BT.h>
#include <usbhub.h>
#include <SPI.h>

#include "dynamixel.hpp"
#include "gyro.hpp"
#include "motor.hpp"

#define DX_STEP_0 10
#define DX_STEP_1 205
#define DX_STEP_2 300

signed short readed_gyro = 0;
bool connected = false;
Gyro gyro;
MotorController motorCtrl;

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

void setup()
{
  Serial1.begin(57142);

  motorCtrl.begin();
  gyro.begin();

  if (Usb.Init() == -1)
  {
    Serial.println("Failed to init USB");
    while (true)
      ;
  }

  // motorCtrl.debug();
}

void loop()
{
  readed_gyro = gyro.read();
  Usb.Task();

  if (!PS4.connected())
  {
    connected = false;
    return;
  }

  if (!connected)
  {    
    if (PS4.getButtonPress(SHARE))
      return;
    connected = true;
  }

  byte leftHatX = PS4.getAnalogHat(LeftHatX);
  byte leftHatY = PS4.getAnalogHat(LeftHatY);
  byte rightHatX = PS4.getAnalogHat(RightHatX);

  if (leftHatX > 137 || leftHatX < 117 || leftHatY > 137 || leftHatY < 117)
  {
    int x = 128 - leftHatX;
    int y = 128 - leftHatY;
    float degree = degrees(atan2(y, x));
    if (degree < 0)
      degree = 360 + degree;
    int speed = PS4.getAnalogButton(R2);
    motorCtrl.go(degree, speed, readed_gyro);
  }
  else if (rightHatX > 137 || rightHatX < 117)
  {
    int x = (128 - rightHatX) * 1.5;
    for (byte i = 0; i < 4; i++)
      motorSendCommand(i, x);
    gyro.zero();
  }
  else if (PS4.getButtonPress(UP))
    motorCtrl.go(90, 255, readed_gyro);
  else if (PS4.getButtonPress(LEFT))
    motorCtrl.go(0, 255, readed_gyro);
  else if (PS4.getButtonPress(DOWN))
    motorCtrl.go(270, 255, readed_gyro);
  else if (PS4.getButtonPress(RIGHT))
    motorCtrl.go(180, 255, readed_gyro);
  else
    motorCtrl.stop();

  if (PS4.getButtonPress(R1) || PS4.getButtonPress(L1))
  {
    byte selectedMotor = PS4.getButtonPress(R1) ? 1 : 2;
    byte offset = selectedMotor == 1 ? 40 : 0;
    if (PS4.getButtonClick(CROSS))
      dxSetPosition(selectedMotor, DX_STEP_0 + offset, 80);
    else if (PS4.getButtonClick(SQUARE))
      dxSetPosition(selectedMotor, DX_STEP_1 + offset, 80);
    else if (PS4.getButtonClick(TRIANGLE))
      dxSetPosition(selectedMotor, DX_STEP_2 + offset, 80);
  }

  if (PS4.getButtonClick(SHARE))
    motorCtrl.turnLeft(gyro);
  else if (PS4.getButtonClick(OPTIONS))
    motorCtrl.turnRight(gyro);
}
