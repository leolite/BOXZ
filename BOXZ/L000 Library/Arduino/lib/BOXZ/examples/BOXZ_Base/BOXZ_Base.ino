//  BOXZ_Base
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:10 Aug, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. DFRobot L298 Shield 2A
//  2. DFRobot L293 Shield 1A
//  3. Seeed Motor Shield V2.0
//  4. Adafruit Motor Drive
//  Key define
//  'w' - Go forward
//  's' - Go backward
//  'a' - Go left
//  'd' - Go right
//  'q' - Go left avertence
//  'e' - Go right avertence

#include "BOXZ.h"

int  key;

void setup()
{
  Serial.begin(9600);
  boxz.init();
  Serial.println("Hello! BOXZ!");
}

void loop()
{
  if(Serial.available() > 0) {    
    key = Serial.read();  
  }
  boxz.motorCom(key);   
}


