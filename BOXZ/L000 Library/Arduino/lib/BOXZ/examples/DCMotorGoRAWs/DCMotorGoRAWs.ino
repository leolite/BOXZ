//  DCMotorGoRAWs
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:30 July, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. DFRobot L298 Shield 2A
//  2. DFRobot L293 Shield 1A
//  3. Seeed Motor Shield V2.0
//  add init() to analyse and configure your driver board automaticilly
//  add goRaws() You can control you motor with raw data string

/*goRaws() mode
You can control you motor with raw data(String HEX). The format is b0101|0xFF|0xFF
Input value is a string.
If you want to goForward in 4 pin mode, you should send "3FFFF", not "0x3FFFF"
Here is a sample how to control 4 pin or 6 pin driver board, also if you known the sequence you can control other kinds of driver board
Byte 1(High): Control bit
Byte 2-3: SpeedA from 0x00 to 0xFF
Byte 4-5(Low): SpeedB  from 0x00 to 0xFF
goForward
4P: 0x3FFFF
6P: 0x9FFFF
goBackward
4P: 0x0FFFF
6P: 0x6FFFF
goLeft
4P: 0x2FFFF
6P: 0xAFFFF
goRight
4P: 0x1FFFF
6P: 0x5FFFF
*/


#include "BOXZ.h"

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
  	//because the serial data is ASC format, we should convert to long int.
	//If you want to goForward, you should send "3FFFF", it means "0x3FFFF".
	//but if you sent "0x3FFFF" or "262143", it's not work.
    boxz.goRaws(comdata);
    comdata = ""; 	//Empty the data string
  }
}
















