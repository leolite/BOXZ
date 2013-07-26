//  Author:Leo.Zhu
//  Date:21 July, 2013
//
//
//
//  Modified record:
//
/*******************************************************************************/
#include "BOXZ.h"


//initialization
//DFRobot 4,7,5,6
void BOXZ::init(int inA, int inB, int pwmA, int pwmB)
{
	pinMode(inA,OUTPUT);
	pinMode(inB,OUTPUT);
	pinMode(pwmA,OUTPUT);
	pinMode(pwmB,OUTPUT);
	stop();
	_inA = inA;
	_inB = inB;
	_pwmA = pwmA;
	_pwmB = pwmB;
	_driverMode = 4;
}

void BOXZ::init(int in1, int in2, int in3, int in4, int pwmA, int pwmB)
{
	pinMode(in1,OUTPUT);
	pinMode(in2,OUTPUT);
	pinMode(in3,OUTPUT);
	pinMode(in4,OUTPUT);
	pinMode(pwmA,OUTPUT);
	pinMode(pwmB,OUTPUT);
	stop();
	_in1 = in1;
	_in2 = in2;
	_in3 = in3;
	_in4 = in4;
	_pwmA = pwmA;
	_pwmB = pwmB;
	_driverMode = 6;
}

boolean BOXZ::init(int type)
{
if(type ==0xDF){
//checking I/O

//Define I/O
	init(DF_INA,DF_INB,DF_SPEEDA,DF_SPEEDB);
//Output
	if(DEBUG ==1){
		Serial.println("DFRobot L298N shield checked done");
	}
	return true;
}
if(type ==0xED){
//checking I/O

//Define I/O
	init(SD_IN1,SD_IN2,SD_IN3,SD_IN4,SD_SPEEDA,SD_SPEEDB);
//Output
	if(DEBUG ==1){
		Serial.println("Seeed L298N shield checked done");
	}
	return true;
}
else
	if(DEBUG ==1){
		Serial.println("Fault:Unknown type driver board");
	}
    return false;
}

/*************************************************************/


void BOXZ::goForward()
{
if(_driverMode == 4){
	digitalWrite(_inA,HIGH);
    digitalWrite(_inB,HIGH);
	analogWrite(_pwmA,DEFAULT_SPEED);
	analogWrite(_pwmB,DEFAULT_SPEED);
	if(DEBUG == 1) Serial.println("FORWARD");
}else if(_driverMode == 6){
	digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
	digitalWrite(_in3,LOW);
    digitalWrite(_in4,HIGH);
	analogWrite(_pwmA,DEFAULT_SPEED);
	analogWrite(_pwmB,DEFAULT_SPEED);
	if(DEBUG == 1) Serial.println("FORWARD");
	}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
}

void BOXZ::goBackward()
{
if(_driverMode == 4){
	digitalWrite(_inA,LOW);
    digitalWrite(_inB,LOW);
	analogWrite(_pwmA,DEFAULT_SPEED);
	analogWrite(_pwmB,DEFAULT_SPEED);
	if(DEBUG == 1) Serial.println("BACKWARD");
}else if(_driverMode == 6){
	digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
	digitalWrite(_in3,HIGH);
    digitalWrite(_in4,LOW);
	analogWrite(_pwmA,DEFAULT_SPEED);
	analogWrite(_pwmB,DEFAULT_SPEED);
	if(DEBUG == 1) Serial.println("BACKWARD");
	}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
}

void BOXZ::goLeft()
{
if(_driverMode == 4){
	digitalWrite(_inA,HIGH);
    digitalWrite(_inB,LOW);
	analogWrite(_pwmA,DEFAULT_SPEED);
	analogWrite(_pwmB,DEFAULT_SPEED);
	if(DEBUG == 1) Serial.println("LEFT");
}else if(_driverMode == 6){
	digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
	digitalWrite(_in3,HIGH);
    digitalWrite(_in4,LOW);
	analogWrite(_pwmA,DEFAULT_SPEED);
	analogWrite(_pwmB,DEFAULT_SPEED);
	if(DEBUG == 1) Serial.println("LEFT");
	}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
}

void BOXZ::goRight()
{
if(_driverMode == 4){
	digitalWrite(_inA,LOW);
    digitalWrite(_inB,HIGH);
	analogWrite(_pwmA,DEFAULT_SPEED);
	analogWrite(_pwmB,DEFAULT_SPEED);
	if(DEBUG == 1) Serial.println("LEFT");
}else if(_driverMode == 6){
	digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
	digitalWrite(_in3,LOW);
    digitalWrite(_in4,HIGH);
	analogWrite(_pwmA,DEFAULT_SPEED);
	analogWrite(_pwmB,DEFAULT_SPEED);
	if(DEBUG == 1) Serial.println("LEFT");
	}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
}

/**********************SPEED Control******************************/
void BOXZ::goForward(int speedA, int speedB)
{
if(_driverMode == 4){
	digitalWrite(_inA,HIGH);
    digitalWrite(_inB,HIGH);
	analogWrite(_pwmA,speedA);
	analogWrite(_pwmB,speedB);
	if(DEBUG == 1) {
    Serial.print(speedA);
    Serial.print(",");
    Serial.print(speedB);
    Serial.println(",FORWARD");
  }
}else if(_driverMode == 6){
	digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
	digitalWrite(_in3,LOW);
    digitalWrite(_in4,HIGH);
	analogWrite(_pwmA,speedA);
	analogWrite(_pwmB,speedB);
	if(DEBUG == 1) {
    Serial.print(speedA);
    Serial.print(",");
    Serial.print(speedB);
    Serial.println(",FORWARD");
  }
}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
}


void BOXZ::goBackward(int speedA, int speedB)
{
if(_driverMode == 4){
	digitalWrite(_inA,LOW);
    digitalWrite(_inB,LOW);
	analogWrite(_pwmA,speedA);
	analogWrite(_pwmB,speedB);
	if(DEBUG == 1)   {
    Serial.print(speedA);
    Serial.print(",");
    Serial.print(speedB);
    Serial.println(",BACKWARD");
  }
}else if(_driverMode == 6){
	digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
	digitalWrite(_in3,HIGH);
    digitalWrite(_in4,LOW);
	analogWrite(_pwmA,speedA);
	analogWrite(_pwmB,speedB);
	if(DEBUG == 1)   {
    Serial.print(speedA);
    Serial.print(",");
    Serial.print(speedB);
    Serial.println(",BACKWARD");
  }
}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
}

void BOXZ::goLeft(int speedA, int speedB)
{
if(_driverMode == 4){
	digitalWrite(_inA,HIGH);
    digitalWrite(_inB,LOW);
	analogWrite(_pwmA,speedA);
	analogWrite(_pwmB,speedB);
	if(DEBUG == 1) {
    Serial.print(speedA);
    Serial.print(",");
    Serial.print(speedB);
    Serial.println(",LEFT");
  }
}else if(_driverMode == 6){
	digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
	digitalWrite(_in3,HIGH);
    digitalWrite(_in4,LOW);
	analogWrite(_pwmA,speedA);
	analogWrite(_pwmB,speedB);
	if(DEBUG == 1) {
    Serial.print(speedA);
    Serial.print(",");
    Serial.print(speedB);
    Serial.println(",LEFT");
  }
}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
}

void BOXZ::goRight(int speedA, int speedB)
{
if(_driverMode == 4){
	digitalWrite(_inA,LOW);
    digitalWrite(_inB,HIGH);
	analogWrite(_pwmA,speedA);
	analogWrite(_pwmB,speedB);
	if(DEBUG == 1) {
    Serial.print(speedA);
    Serial.print(",");
    Serial.print(speedB);
    Serial.println(",RIGHT");
  }
}else if(_driverMode == 6){
	digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
	digitalWrite(_in3,LOW);
    digitalWrite(_in4,HIGH);
	analogWrite(_pwmA,speedA);
	analogWrite(_pwmB,speedB);
	if(DEBUG == 1) {
    Serial.print(speedA);
    Serial.print(",");
    Serial.print(speedB);
    Serial.println(",RIGHT");
  }
}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
}

void BOXZ::stop()
{
if(_driverMode == 4){
	/*Unenble the pin, to stop the motor. */
	digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,LOW);
}else if(_driverMode == 6){
	digitalWrite(_in1,HIGH);
    digitalWrite(_in2,HIGH);
	digitalWrite(_in3,HIGH);
    digitalWrite(_in4,HIGH);
	digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,LOW);
}else{
	if(DEBUG == 1) Serial.println("FAULT:UNKNOWN MODE");
	}	
//	if(DEBUG == 1) Serial.println("STOP");
}


BOXZ boxz;