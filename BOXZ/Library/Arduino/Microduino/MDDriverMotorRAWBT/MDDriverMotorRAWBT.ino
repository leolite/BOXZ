//  tested OK
//  MDDriverMotorRAW
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:18 Oct, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. Microduino A3906

#include "BOXZ.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10); // RX, TX

unsigned long keyW = 0x33311;
unsigned long keyS = 0x0AAFF;
unsigned long keyA = 0x133FF;
unsigned long keyD = 0x2CC33;
unsigned long keySTOP = 0x3FFFF;
int speedA_value = 255;
int speedB_value = 255;
int keyIn;
long  key;
long keyOld;


void setup()
{
  Serial.begin(9600);
  //A3906 IN1,IN3,IN2,IN4
  boxz.initMotor(7,8,5,6);
  Serial.println("Hello! BOXZ!");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop()
{

  if(mySerial.available() > 0) {    
    keyIn = mySerial.read();  
    b_motor_com(keyIn); 
  }

  boxz.motorRaw(key); 
  if(key != keyOld){
    keyOld = key;
    Serial.println(key);
  }
}

void b_motor_com(int keyword){
  switch (keyword){
  case 'w':
    key =keyW;

    break;
  case 's':
    key =keyS;
    break;
  case 'a':
    key =keyA;
    break;
  case 'd':
    key =keyD;
    break;
  case ' ':
    key = keySTOP;
    break;
  default :
    key = keySTOP;
    return;
  }
}





