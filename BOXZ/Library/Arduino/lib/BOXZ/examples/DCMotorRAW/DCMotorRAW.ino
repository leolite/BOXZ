//  DCMotorRAW
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:29 July, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. DFRobot L298 Shield 2A
//  2. DFRobot L293 Shield 1A
//  3. Seeed Motor Shield V2.0
//  add initMotor() to analyse and configure your driver board automaticilly
//  add motorRaw() You can control you motor with raw data

/*motorRaw() mode
You can control you motor with raw data(Long int HEX), The format is b0101|0xFF|0xFF
Here is a sample how to control 4 pin or 6 pin driver board, also if you known the sequence you can control other kinds of driver board
If you want to goForward in 4 pin mode, you should send "262143", not "0x3FFFF"
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
  boxz.initMotor();
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
	//because the serial data is ASC format, we should convert to long int.
	//If you want to goForward, you should send "262143", it means "0x3FFFF".
	//but if you sent "0x3FFFF" or "3FFFF", it's not work.
    key = comdata.toInt();
    Serial.println(key);
    comdata = "";
    boxz.motorRaw(key);
  }
}
















