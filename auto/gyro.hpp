#ifndef GYRO_MPU6050
#define GYRO_MPU6050

#ifndef GYRO_ADDRESS
#define GYRO_ADDRESS 0x12
#endif

class Gyro
{
  private:
    int gyroAddress;

  public:
    Gyro()
    {
        gyroAddress = GYRO_ADDRESS;
    }

    Gyro(int address)
    {
        gyroAddress = address;
    }

    void begin();
    void zero();
    void offset();
    signed short read();
};

#endif