//  BOXZ_Pro
//  Demo function:The application method to drive the 2x DC motor and 2x Servo
//  Author:Leo.Zhu
//  Date:2017-06-29

//  website EN: www.boxz.cc
//  website CN: www.diyfun.org
//  QQ Group: 248742803
//  Download: github.com/leolite/BOXZ
//  License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
//  http://creativecommons.org/licenses/by-nc-sa/3.0/


//  Hardware support list
//  BOXZ V3 OCROBOT

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
//  ' ' - release/stop

#include "BOXZ.h"
int  key;
int valueV1; //motor speed left
int valueV2; //motor speed right

void setup()
{
  Serial.begin(9600);
  boxz.initMotor(3);
  boxz.initServo();
  Serial.println("Hello! BOXZ!");

  valueV1 = 0x99;
  valueV2 = 0x99;
}

void loop()
{
  if(Serial.available() > 0) {    
    key = Serial.read();  
    boxz.servoCom(key); 
    boxz.motorCom(key,valueV1,valueV2); 
  }
}



