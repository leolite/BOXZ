//  BOXZ_Pro
//  Demo function:The application method to drive the 2x DC motor and 2x Servo
//  Author:Leo.Zhu
//  Date:10 Aug, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. DFRobot ROMEO
//  Key define
//  'w' - Go forward
//  's' - Go backward
//  'a' - Go left
//  'd' - Go right
//  'q' - Go left avertence
//  'e' - Go right avertence
//  'u' - Left hand raise up	'U' - Left hand step raise up 
//  'i' - Right hand raise up	'I' - Right hand step raise up 
//  'j' - Left hand drop down	'J' - Left hand step drop down 
//  'k' - Right hand drop down	'K' - Right hand step drop down 
//  'o' - Both hand raise up	'O' - Both hand step raise up 
//  'l' - Both hand drop down	'L' - Both hand step drop down


#include "BOXZ.h"
int  key;

void setup()
{
  Serial1.begin(9600);
  boxz.initMotor();
  boxz.initServo();
  Serial1.println("Hello! BOXZ!");
}

void loop()
{
  if(Serial1.available() > 0) {    
    key = Serial1.read();  
    boxz.servoCom(key); 
    boxz.motorCom(key); 
  }
}


