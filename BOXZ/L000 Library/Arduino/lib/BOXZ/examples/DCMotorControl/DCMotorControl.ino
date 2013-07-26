//  DCMotorControl
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:23 July, 2013
//  Hardware support list
//  1. DFRobot L298 Shield 2A
//  2. DFRobot L293 Shield 1A

#include "BOXZ.h"

int pin_inA = 4;
int pin_inB = 7;
int pin_pwmA = 5;
int pin_pwmB = 6;
int speedA_value = 150;
int speedB_value = 150;

void setup()
{
  boxz.init(pin_inA,pin_inB,pin_pwmA,pin_pwmB);
}

void loop()
{
  //Simple control
  boxz.goForward();
  delay(2000);
  boxz.stop();
  delay(500);
  boxz.goBackward();
  delay(2000);
  boxz.stop();
  delay(500);
  boxz.goLeft();
  delay(2000);
  boxz.stop();
  delay(500);
  boxz.goRight();
  delay(2000);
  boxz.stop();
  delay(3000);

  //Speed control
  boxz.goForward(speedA_value,speedB_value);
  delay(2000);
  boxz.stop();
  delay(500);
  boxz.goBackward(speedA_value,speedB_value);
  delay(2000);
  boxz.stop();
  delay(500);
  boxz.goLeft(speedA_value,speedB_value);
  delay(2000);
  boxz.stop();
  delay(500);
  boxz.goRight(speedA_value,speedB_value);
  delay(2000);
  boxz.stop();
  delay(500);	
}