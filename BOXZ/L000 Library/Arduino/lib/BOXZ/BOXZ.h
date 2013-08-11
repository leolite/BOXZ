/*
BOXZ.h - Library for general robot control.
Created by Leo.Zhu, 21 July, 2013.
https://github.com/leolite/BOXZ

License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
http://creativecommons.org/licenses/by-nc-sa/3.0/
*/

/*  Modified record:
	Updata: 20130810
	1. Support for Adafruit Motor Drive
	2. add checkIO_AF()
	3. add initAFMotor()
	
	Updata: 20130809
	1. add motorCom()
	
	Update: 20130730
	1. add goRaws() for control by raw data(String HEX).
	
	Update: 20130729
	1. add goRaw() for control by raw data(Long int HEX).
	
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
#define PREACCELERATION	1  //not ready yet
#define DEFAULT_SPEED	255

/******Pins definitions for DFROBOT L298N and A3906*************/
//_driverMode = 4
//2 control pin and 2 speed pin
#define DF_INA			4
#define DF_INB			7
#define DF_SPEEDA		5
#define DF_SPEEDB		6

/******Pins definitions for SEEED L298N and TB6612FNG*************/
//_driverMode = 6
//4 control pin and 2 speed pin
#define SD_IN1			8
#define SD_IN2			11
#define SD_IN3			12
#define SD_IN4			13
#define SD_SPEEDA		9
#define SD_SPEEDB		10

/******Pins definitions for Adafruit Motor shield*************/
//default active M1 and M2(AF_GROUP = 1); if want to choose M3 and M4, set AF_GROUP = 2
#define AF_GROUP 		1
//_driverMode = 8
//4 control pin and 4 speed pin, with 2 servo(PWM1A and PWM1B)
#define AF_DIR_LATCH	12
#define AF_DIR_CLK		4
#define AF_DIR_EN		7
#define AF_DIR_SER		8
#define AF_PWM0A 		6
#define AF_PWM0B 		5
#define AF_PWM1A 		9
#define AF_PWM1B 		10
#define AF_PWM2A 		11
#define AF_PWM2B 		3
//status for Adafruit Motor Driver 74HC595 data
#define AFM1F 		32;
#define AFM1B 		16;
#define AFM2F 		64;
#define AFM2B 		8;
#define AFM3F 		128;
#define AFM3B 		2;
#define AFM4F 		1;
#define AFM4B 		4;


/**Class for motor control**/
class BOXZ
{
public:
	boolean init();  //Automatic check board
	boolean init(int type);
	void init(int inA, int inB, int pwmA, int pwmB);
	void init(int in1, int in2, int in3, int in4, int pwmA, int pwmB);
	void initAFMotor(); //initialization for Adafruit Motor Driver
	void goForward(int speedA, int speedB);
	void goBackward(int speedA, int speedB);
	void goLeft(int speedA, int speedB);
	void goRight(int speedA, int speedB);
	void goForward();
	void goBackward();
	void goLeft();
	void goRight();
	void stop();
	void motorCom(int keyword); //Support for BOXZ Base
	void motorCom(int keyword, int speedA, int speedB); //Support for BOXZ Base with speed control
	void ServoCom(int keyword); //Support for BOXZ Base
    void goRaw(unsigned long data);
	void goRaws(String datas);
private:
	boolean checkIO_ED(); //IO check for Seeed
	boolean checkIO_DF(); //IO check for DFROBOT
	boolean checkIO_AF(); //IO check for Adafruit
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
	int _AFMstatus; //status for Adafruit Motor Driver 74HC595 data
	int _speedA;
	int _speedB;
};

extern BOXZ boxz;
#endif
