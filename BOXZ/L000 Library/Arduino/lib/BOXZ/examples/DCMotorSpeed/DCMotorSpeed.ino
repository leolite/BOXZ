//  DCMotorSpeed
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:21 July, 2013
//  https://github.com/leolite/BOXZ
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
  Serial.begin(9600);
  boxz.init(pin_inA,pin_inB,pin_pwmA,pin_pwmB);
  Serial.println("Hello! BOXZ!");
}

void loop()
{
//Speed down
  for (int i=255; i >= 0; i-=10){
    speedA_value = i;
    speedB_value = i;
    boxz.goForward(speedA_value,speedB_value);
    Serial.print("SPEED is: ");
    Serial.println(i);
    delay(1000);
    
  } 

//Speed up
  for (int i=0; i <= 255; i+=10){
    speedA_value = i;
    speedB_value = i;
    boxz.goForward(speedA_value,speedB_value);
    Serial.print("SPEED is: ");
    Serial.println(i);
    delay(1000);
  }
}
