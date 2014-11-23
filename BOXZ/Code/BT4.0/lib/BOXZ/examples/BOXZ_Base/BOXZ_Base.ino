//  BOXZ_Base
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:10 Aug, 2013

//  website EN: www.boxz.cc
//  website CN: www.diyfun.org
//  QQ Group: 248742803
//  Download: github.com/leolite/BOXZ
//  License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
//  http://creativecommons.org/licenses/by-nc-sa/3.0/

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
  boxz.initMotor();
  Serial.println("Hello! BOXZ!");
}

void loop()
{
  if(Serial.available() > 0) {    
    key = Serial.read();  
  }
  boxz.motorCom(key);   
}


