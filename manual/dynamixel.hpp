#ifndef DYNAMIXEL
#define DYNAMIXEL

#ifndef DYNAMIXEL_BAUD
#define DYNAMIXEL_BAUD 57142
#endif

void dxSetPosition(int motorID, int position, int speed);

#endif