//  DCMotorGoRAW
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:29 July, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. DFRobot L298 Shield 2A
//  2. DFRobot L293 Shield 1A
//  3. Seeed Motor Shield V2.0
//  add init() to analyse and configure your driver board automaticilly
//  add goRaw() You can control you motor with raw data

/*goRaw() mode
You can control you motor with raw data, The format is b0101|0xFF|0xFF
Here is a sample how to control 4 pin or 6 pin driver board, also if you known the sequence you can control other kinds of driver board
Byte 1(High): Control bit
Byte 2-3: SpeedA from 0x00 to 0xFF
Byte 4-5(Low): SpeedB  from 0x00 to 0xFF
goForward
4P: 0x3FFFF = 262143
6P: 0x9FFFF = 655359
goBackward
4P: 0x0FFFF = 65535
6P: 0x6FFFF = 458751
goLeft
4P: 0x2FFFF = 196607
6P: 0xAFFFF = 720895
goRight
4P: 0x1FFFF = 131071
6P: 0x5FFFF = 393215
*/


#include "BOXZ.h"

unsigned long key;
String comdata = "";

void setup()
{
  Serial.begin(9600);
  boxz.init();
  Serial.println("Hello! BOXZ!");
}

void loop()
{
  while (Serial.available() > 0)  
  {
    comdata += char(Serial.read());
    delay(2);
  }
  if (comdata.length() > 0)
  {
    Serial.println(comdata);
    key = comdata.toInt();
    Serial.println(key);
    comdata = "";
    boxz.goRaw(key);
  }
}
















