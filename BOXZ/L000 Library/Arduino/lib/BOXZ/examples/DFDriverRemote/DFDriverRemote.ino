//  DFDriverRemote
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:23 July, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. DFRobot L298 Shield 2A
//  2. DFRobot L293 Shield 1A

#include "BOXZ.h"

//int pin_inA = 4;
//int pin_inB = 7;
//int pin_pwmA = 5;
//int pin_pwmB = 6;
int speedA_value = 255;
int speedB_value = 255;
int  key;
int driveID = 0xDF;  //Driver ID is DFROBOT

void setup()
{
  Serial.begin(9600);
  boxz.init(driveID);
  Serial.println("Hello! BOXZ!");
}

void loop()
{
  if(Serial.available() > 0) {    
    key = Serial.read();  
  }
  b_motor_com(key);   
}

void b_motor_com(int keyword){
  switch (keyword){
  case 'w':
    boxz.goForward(speedA_value,speedB_value);
    break;
  case 's':
    boxz.goBackward(speedA_value,speedB_value);
    break;
  case 'a':
    boxz.goLeft(speedA_value,speedB_value);
    break;
  case 'd':
    boxz.goRight(speedA_value,speedB_value);
    break;
  case ' ':
    boxz.stop();
    break;
  default :
    boxz.stop();
    return;
  }
}