/*
BOXZ.h - Library for 2x DC motor control.
Created by Leo.Zhu, 21 July, 2013.
Released into the public domain.
*/

#ifndef __BOXZ_H__
#define __BOXZ_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define DEBUG			0
#define DEFAULT_SPEED	255
#define PREACCELERATION	1  //not ready yet


/******Pins definitions for DFROBOT L298N and A3906*************/
//2 control pin and 2 speed pin
#define DF_INA			4
#define DF_INB			7
#define DF_SPEEDA		5
#define DF_SPEEDB		6

/******Pins definitions for SEEED L298N and TB6612FNG*************/
//4 control pin and 2 speed pin
#define SD_IN1			8
#define SD_IN2			11
#define SD_IN3			12
#define SD_IN4			13
#define SD_SPEEDA		9
#define SD_SPEEDB		10





/**Class for motor control**/
class BOXZ
{
public:
	boolean init(int type);
	void init(int inA, int inB, int pwmA, int pwmB);
	void init(int in1, int in2, int in3, int in4, int pwmA, int pwmB);
	void goForward(int speedA, int speedB);
	void goBackward(int speedA, int speedB);
	void goLeft(int speedA, int speedB);
	void goRight(int speedA, int speedB);
	void goForward();
	void goBackward();
	void goLeft();
	void goRight();
	void stop();
//  void driverRaw(Byte status, int pwmA, int pwmB);
private:
	int _inA;
	int _inB;
	int _in1; 
	int _in2; 
	int _in3; 
	int _in4; 
	int _pwmA; 
	int _pwmB;
	int _driverMode;
};

extern BOXZ boxz;
#endif
