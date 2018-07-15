#include <PS4BT.h>
#include <usbhub.h>
#include <SPI.h>

#define ENABLE_CORRECTION 1
#define ROBOT_MODE 0 // 0 = auto , 1 = manual

#define DYNAMIXEL_SERIAL Serial1
#include "dynamixel.hpp"
#include "gyro.hpp"
#include "motor.hpp"
#include "relay.hpp"

#define DX_STEP_0 0
#define DX_STEP_1 180
#define DX_STEP_2 300

signed short readed_gyro = 0;
Gyro gyro;

MotorController motorCtrl;
RelayContoller relayCtrl;

USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

DynamixelController dxController[2] = {DynamixelController(1), DynamixelController(2)};
byte selectedMotor = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println("Connecting ...");

  motorCtrl.begin();
  gyro.begin();

  if (Usb.Init() == -1)
  {
    Serial.println("Failed to init USB");
    while (true)
      ;
  }

  relayCtrl.begin();

  dxController[0].begin();
  dxController[0].setPosition(DX_STEP_0, 50);
  delay(100);
  dxController[1].begin();
  dxController[1].setPosition(DX_STEP_1, 50);

  Serial.println("Setup has been finished");

  motorCtrl.debug();
}

void loop()
{
  readed_gyro = gyro.read();
  Usb.Task();

  if (PS4.connected())
  {
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

#if ROBOT_MODE == 1
    if (PS4.getButtonClick(R1))
      selectedMotor = 1;
    else if (PS4.getButtonClick(L1))
      selectedMotor = 2;
#else
    if (PS4.getButtonClick(R1))
      motorCtrl.turnRight(gyro);
    else if (PS4.getButtonClick(L1))
      motorCtrl.turnLeft(gyro);
#endif

#if ROBOT_MODE == 1
    if (PS4.getButtonClick(CIRCLE))
      dxController[selectedMotor].setPosition(DX_STEP_0, 50);
    else if (PS4.getButtonClick(TRIANGLE))
      dxController[selectedMotor].setPosition(DX_STEP_1, 50);
    else if (PS4.getButtonClick(SQUARE))
      dxController[selectedMotor].setPosition(DX_STEP_2, 50);
#else
    if (PS4.getButtonClick(CIRCLE))
      relayCtrl.sendCommand(RELAY_HIGH);
    else if (PS4.getButtonClick(TRIANGLE))
      relayCtrl.sendCommand(RELAY_MEDIUM);
    else if (PS4.getButtonClick(SQUARE))
      relayCtrl.sendCommand(RELAY_LOW);
#endif
  }
}
