/*
BOXZ.h - Library for general robot control.
Created by Leo.Zhu, 21 July, 2013.
https://github.com/leolite/BOXZ

License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
http://creativecommons.org/licenses/by-nc-sa/3.0/
*/

/*  Modified record:
	Update: 20130729
	1. add goRaw() for control by raw data.
	
	Update: 20130728
	1. add checkIO_ED() and checkIO_DF() for checking board type
	2. add init() for Automatic choose board type.
*/

#ifndef __BOXZ_H__
#define __BOXZ_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define DEBUG			1
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
	boolean init();  //Automatic check board
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
    void goRaw(unsigned long data);
private:
	boolean checkIO_ED(); //IO check for Seeed
	boolean checkIO_DF(); //IO check for DFROBOT
	//Pin define
	int _inA;
	int _inB;
	int _in1; 
	int _in2; 
	int _in3; 
	int _in4; 
	int _pwmA; 
	int _pwmB;
	int _driverMode;
	//Output value
	int _in1Status;
	int _in2Status;
	int _in3Status;
	int _in4Status;
	int _speedA;
	int _speedB;
};

extern BOXZ boxz;
#endif
