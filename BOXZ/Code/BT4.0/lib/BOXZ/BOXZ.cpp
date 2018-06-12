/*
BOXZ.h - Library for general robot control.
 Created by Leo.Zhu, 21 July, 2013.
 https://github.com/leolite/BOXZ
 
 License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 http://creativecommons.org/licenses/by-nc-sa/3.0/
 */

/*Define
 
 - Motor RAW data
 Side A = Right motor
 Side B = Left motor
 
 The RAW format is 0xF|0xFF|0xFF
 Byte 1(High): Control bit
 Byte 2-3: SpeedA(Right) from 0x00 to 0xFF
 Byte 4-5(Low): SpeedB(Left) from 0x00 to 0xFF
 
 Servo action type
 0: default mode - start from min(max) position to target, like a sweep.
 1: step mode - turn 10 degree each step.
 2: consecutive mode - start from current position to target.
 
 - Servo RAW data
 Servo01 = Left hand
 Servo02 = Right hand
 
 The RAW format is 0xF|0xFF|0xFF
 BOXZ Pro
 Byte 1(High): Control bit
 Byte 2-3: Servo02(Right) degree from 0x00 to 0xB4(DEC is 180 degree)
 Byte 4-5(Low): Servo01(Left) degree from 0x00 to 0xB4(DEC is 180 degree)
 
 Control bit
 B0000: Empty
 B0001: Left hand servo active, if not ignore PARA degree
 B0010: Right hand servo active
 B0011: Left and Right hand servo active
 */
/******************************* I/O check function ************************************************/
#include "BOXZ.h"
#include <Servo.h> 

/******************************* initialization function ************************************************/

//initialization
void BOXZ::initMotor(int inx, int pwmx)
{
  pinMode(inx,OUTPUT);
  pinMode(pwmx,OUTPUT);
  _inA = inx;
  _pwmA = pwmx;

  _driverMode = 2;
  _driveParaSD = 0;
  _driveParaNB = 1;
  stop();
}

void BOXZ::initMotor(int inx, int pwmx, int inCHx)
{
  pinMode(inx,OUTPUT);
  pinMode(pwmx,OUTPUT);
  if(inCHx == 1){
    _inA = inx;
    _pwmA = pwmx;
  }
  else if(inCHx == 2){
    _inB = inx;
    _pwmB = pwmx;
  }
  else if(inCHx == 3){
    _inC = inx;
    _pwmC = pwmx;
  }
  else if(inCHx == 4){
    _inD = inx;
    _pwmD = pwmx;
  }
  else{
    //error
    _inA = 2;
    _inB = 4;
    _inC = 7;
    _inD = 8;
    _pwmA = 3;
    _pwmB = 5;
    _pwmC = 6;
    _pwmD = 9;
  }

  _driverMode = 2;
  _driveParaSD = 0;
  _driveParaNB = 1;
  _inCHx = inCHx;
  stop();
}

//init for 4 Pin Motor Driver
//BOXZ 4,7,5,6
void BOXZ::initMotor(int inA, int inB, int pwmA, int pwmB)
{
  pinMode(inA,OUTPUT);
  pinMode(inB,OUTPUT);
  pinMode(pwmA,OUTPUT);
  pinMode(pwmB,OUTPUT);
  _inA = inA;
  _inB = inB;
  _pwmA = pwmA;
  _pwmB = pwmB;
  _driverMode = 4;
  _driveParaSD = 0;
  _driveParaNB = 0;
  stop();
}

////init for 6 Pin Motor Driver
void BOXZ::initMotor(int in1, int in2, int in3, int in4, int pwmA, int pwmB)
{
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(pwmA,OUTPUT);
  pinMode(pwmB,OUTPUT);
  _in1 = in1;
  _in2 = in2;
  _in3 = in3;
  _in4 = in4;
  _pwmA = pwmA;
  _pwmB = pwmB;
  _driverMode = 6;
  stop();
}


//init for 8 Pin Adafruit Motor Driver
void BOXZ::initAFMotor(){
  pinMode( AF_DIR_LATCH, OUTPUT) ;
  pinMode( AF_DIR_CLK, OUTPUT) ;
  pinMode( AF_DIR_EN, OUTPUT) ;
  pinMode( AF_DIR_SER, OUTPUT) ;
  digitalWrite( AF_DIR_EN, LOW);
  if(AF_GROUP == 2)
  {
    _in1Status = AFM3F;
    _in2Status = AFM3B;
    _in3Status = AFM4F;
    _in4Status = AFM4B;
    _pwmA = AF_PWM0A;
    _pwmB = AF_PWM0B;
    //if(DEBUG ==1){
    //  Serial.println(F("Motor Group 02 actived"));
    //}
  }
  //Default mode Group 1 actived M1 and M2
  else{
    _in1Status = AFM1F;
    _in2Status = AFM1B;
    _in3Status = AFM2F;
    _in4Status = AFM2B;
    _pwmA = AF_PWM2B; 
    _pwmB = AF_PWM2A;
    //if(DEBUG ==1){
    //  Serial.println(F("Motor Group 01 actived"));
    //}
  }
  _driverMode = 8;
  stop();
}

//init
void  BOXZ::initMotor()
{
  initMotor(BOXZ_INA,BOXZ_INB,BOXZ_SPEEDA,BOXZ_SPEEDB);
}

//Version 2 = BOXZ L9110 Red board; 3 = OCROBOT
void BOXZ::initMotor(int ver)
{
  //BOXZ V2 L9110 red board
  if(ver == 2){
    initMotor(2,3,1);
    initMotor(4,5,2);
    //initMotor(7,6,3);
    //initMotor(8,9,4);
    initMotorType(1); 
  }
  //OCROBOT DRV8833
  else if(ver == 3){
    initMotor(BOXZ_INA,BOXZ_INB,BOXZ_SPEEDA,BOXZ_SPEEDB);
    initMotorType(2); 
  }
  else if(ver == 4){
     //initMotor(2,3,1);
    initMotor(4,5,2);
    initMotor(7,6,3);
    //initMotor(8,9,4);
    initMotorType(1); 
  }
  else{
    initMotor(BOXZ_INA,BOXZ_INB,BOXZ_SPEEDA,BOXZ_SPEEDB);

  }
}


//TYPE 0 = standard; 1 = L9110; 2 = DRV8833
void BOXZ::initMotorType(int type){
  if(type == 1){
    _driveParaSD = 0;
    _driveParaNB = 1;
    //_driveParaBB = 0;
  }
  else if(type == 2){
    _driveParaSD = 1;
    _driveParaNB = 0;
    //_driveParaBB = 0;
  }
  else
  {
    _driveParaSD = 0;
    _driveParaNB = 0;
    //_driveParaBB = 0;
  }
}


/****************************direction of motion control function*********************************/
//Control BOXZ go forward
void BOXZ::goForward()
{
  //_driveParaNB = 1;
  if(_driverMode == 2){
    digitalWrite(_inA,LOW);
    digitalWrite(_inB,LOW);
    digitalWrite(_pwmA,DEFAULT_SPEED);
    digitalWrite(_pwmB,DEFAULT_SPEED);
    if(_inCHx>2){
      digitalWrite(_inC,LOW);
      digitalWrite(_inD,LOW);
      digitalWrite(_pwmC,DEFAULT_SPEED);
      digitalWrite(_pwmD,DEFAULT_SPEED);
    }
    //if(DEBUG == 1) Serial.println("FORWARD");
  }
  else if(_driverMode == 4){
    if(_driveParaSD == 1){
      digitalWrite(_inA,HIGH);
      digitalWrite(_inB,HIGH);
      analogWrite(_pwmA,255-DEFAULT_SPEED);
      analogWrite(_pwmB,255-DEFAULT_SPEED);
    }
    else{
      digitalWrite(_inA,HIGH);
      digitalWrite(_inB,HIGH);
      analogWrite(_pwmA,DEFAULT_SPEED);
      analogWrite(_pwmB,DEFAULT_SPEED);
    }
    //if(DEBUG == 1) Serial.println("FORWARD");
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
    digitalWrite(_in3,LOW);
    digitalWrite(_in4,HIGH);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //if(DEBUG == 1) Serial.println("FORWARD");
  }
  else if(_driverMode == 8){
    _AFMstatus = _in1Status | _in3Status;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //Serial.println(_AFMstatus);
    //if(DEBUG == 1) Serial.println("FORWARD");
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
}

//Control BOXZ go backward
void BOXZ::goBackward()
{
  //_driveParaNB = 1;
  if(_driverMode == 2){
    digitalWrite(_inA,HIGH);
    digitalWrite(_inB,HIGH);
    digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,LOW);
    if(_inCHx>2){
      digitalWrite(_inC,HIGH);
      digitalWrite(_inD,HIGH);
      digitalWrite(_pwmC,LOW);
      digitalWrite(_pwmD,LOW);
    }
    //if(DEBUG == 1) Serial.println("BACKWARD");
  }
  else if(_driverMode == 4){
    digitalWrite(_inA,LOW);
    digitalWrite(_inB,LOW);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //if(DEBUG == 1) Serial.println("BACKWARD");
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
    digitalWrite(_in3,HIGH);
    digitalWrite(_in4,LOW);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //if(DEBUG == 1) Serial.println("BACKWARD");
  }
  else if(_driverMode == 8){
    _AFMstatus = _in2Status | _in4Status;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //Serial.println(_AFMstatus);
    //if(DEBUG == 1) Serial.println("BACKWARD");
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
}

//Control BOXZ turn left
void BOXZ::goLeft()
{
  //_driveParaNB = 1;
  if(_driverMode == 2){
    digitalWrite(_inA,LOW);
    digitalWrite(_inB,HIGH); //20170701
    digitalWrite(_pwmA,DEFAULT_SPEED);
    digitalWrite(_pwmB,LOW);
    if(_inCHx>2){
      digitalWrite(_inC,LOW);
      digitalWrite(_inD,HIGH); //20170701
      digitalWrite(_pwmC,DEFAULT_SPEED);
      digitalWrite(_pwmD,LOW);
    }
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else if(_driverMode == 4){
    if(_driveParaSD == 1){
      digitalWrite(_inA,LOW);
      digitalWrite(_inB,HIGH);
      analogWrite(_pwmA,DEFAULT_SPEED);
      analogWrite(_pwmB,255-DEFAULT_SPEED);
    }
    else{
      digitalWrite(_inA,HIGH);
      digitalWrite(_inB,LOW);
      analogWrite(_pwmA,DEFAULT_SPEED);
      analogWrite(_pwmB,DEFAULT_SPEED);
    }
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
    digitalWrite(_in3,HIGH);
    digitalWrite(_in4,LOW);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else if(_driverMode == 8){
    _AFMstatus = _in2Status | _in3Status;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //Serial.println(_AFMstatus);
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
}

//Control BOXZ turn right
void BOXZ::goRight()
{
  //_driveParaNB = 1;
  if(_driverMode == 2){
    digitalWrite(_inA,HIGH); //20170701
    digitalWrite(_inB,LOW);
    digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,DEFAULT_SPEED);
    if(_inCHx>2){
      digitalWrite(_inC,HIGH); //20170701
      digitalWrite(_inD,LOW);
      digitalWrite(_pwmC,LOW);
      digitalWrite(_pwmD,DEFAULT_SPEED);
    }
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else if(_driverMode == 4){
    if(_driveParaSD == 1){
      digitalWrite(_inA,HIGH);
      digitalWrite(_inB,LOW);
      analogWrite(_pwmA,255-DEFAULT_SPEED);
      analogWrite(_pwmB,DEFAULT_SPEED);
    }
    else{
      digitalWrite(_inA,LOW);
      digitalWrite(_inB,HIGH);
      analogWrite(_pwmA,DEFAULT_SPEED);
      analogWrite(_pwmB,DEFAULT_SPEED);
    }
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
    digitalWrite(_in3,LOW);
    digitalWrite(_in4,HIGH);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else if(_driverMode == 8){
    _AFMstatus = _in1Status | _in4Status;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    analogWrite(_pwmA,DEFAULT_SPEED);
    analogWrite(_pwmB,DEFAULT_SPEED);
    //Serial.println(_AFMstatus);
    //if(DEBUG == 1) Serial.println("RIGHT");
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
}


/****************************direction of speed control function*********************************/
//Control BOXZ go forward with speed control
void BOXZ::goForward(int speedA, int speedB)
{
  //_driveParaNB = 1;
  if(_driverMode == 2){
    digitalWrite(_inA,LOW);
    digitalWrite(_inB,LOW);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
    if(_inCHx>2){
      digitalWrite(_inC,LOW);
      digitalWrite(_inD,LOW);
      analogWrite(_pwmC,speedA);
      analogWrite(_pwmD,speedB);
    }
    //if(DEBUG == 1) Serial.println("FORWARD");
  }
  else if(_driverMode == 4){
    if(_driveParaSD == 1){
      digitalWrite(_inA,HIGH);
      digitalWrite(_inB,HIGH);
      analogWrite(_pwmA,255-speedA);
      analogWrite(_pwmB,255-speedB);
    }
    else{
      digitalWrite(_inA,HIGH);
      digitalWrite(_inB,HIGH);
      analogWrite(_pwmA,speedA);
      analogWrite(_pwmB,speedB);
    }
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
    digitalWrite(_in3,LOW);
    digitalWrite(_in4,HIGH);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else if(_driverMode == 8){
    _AFMstatus = _in1Status | _in3Status;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
}

//Control BOXZ go backward with speed control
void BOXZ::goBackward(int speedA, int speedB)
{
  //_driveParaNB = 1;
  //not support speed control
  if(_driverMode == 2){
    digitalWrite(_inA,HIGH);
    digitalWrite(_inB,HIGH);
    digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,LOW);
    if(_inCHx>2){
      digitalWrite(_inC,HIGH);
      digitalWrite(_inD,HIGH);
      digitalWrite(_pwmC,LOW);
      digitalWrite(_pwmD,LOW);
    }
    //if(DEBUG == 1) Serial.println("BACKWARD");
  }
  else if(_driverMode == 4){
    digitalWrite(_inA,LOW);
    digitalWrite(_inB,LOW);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
    digitalWrite(_in3,HIGH);
    digitalWrite(_in4,LOW);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else if(_driverMode == 8){
    _AFMstatus = _in2Status | _in4Status;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
}

//Control BOXZ turn left with speed control
void BOXZ::goLeft(int speedA, int speedB)
{
  //_driveParaNB = 1;
  if(_driverMode == 2){
    digitalWrite(_inA,LOW);
    digitalWrite(_inB,HIGH); //20170701
    analogWrite(_pwmA,speedA);
    digitalWrite(_pwmB,LOW);
    if(_inCHx>2){
      digitalWrite(_inC,LOW);
      digitalWrite(_inD,HIGH); //20170701
      analogWrite(_pwmC,speedA);
      digitalWrite(_pwmD,LOW);
    }
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else if(_driverMode == 4){
    if(_driveParaSD == 1){
      digitalWrite(_inA,LOW);
      digitalWrite(_inB,HIGH);
      analogWrite(_pwmA,speedB);
      analogWrite(_pwmB,255-speedA);
    }
    else{
      digitalWrite(_inA,HIGH);
      digitalWrite(_inB,LOW);
      analogWrite(_pwmA,speedA);
      analogWrite(_pwmB,speedB);
    }
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,HIGH);
    digitalWrite(_in2,LOW);
    digitalWrite(_in3,HIGH);
    digitalWrite(_in4,LOW);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else if(_driverMode == 8){
    _AFMstatus = _in2Status | _in3Status;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
}

//Control BOXZ turn right with speed control
void BOXZ::goRight(int speedA, int speedB)
{
  //_driveParaNB = 1;
  if(_driverMode == 2){
    digitalWrite(_inA,HIGH); //20170701
    digitalWrite(_inB,LOW);
    digitalWrite(_pwmA,LOW);
    analogWrite(_pwmB,speedB);
    if(_inCHx>2){
      digitalWrite(_inC,HIGH); //20170701
      digitalWrite(_inD,LOW);
      digitalWrite(_pwmC,LOW);
      analogWrite(_pwmD,speedB);
    }
    //if(DEBUG == 1) Serial.println("LEFT");
  }
  else if(_driverMode == 4){
    if(_driveParaSD == 1){
      digitalWrite(_inA,HIGH);
      digitalWrite(_inB,LOW);
      analogWrite(_pwmA,255-speedB);
      analogWrite(_pwmB,speedA);
    }
    else{
      digitalWrite(_inA,LOW);
      digitalWrite(_inB,HIGH);
      analogWrite(_pwmA,speedA);
      analogWrite(_pwmB,speedB);
    }
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,LOW);
    digitalWrite(_in2,HIGH);
    digitalWrite(_in3,LOW);
    digitalWrite(_in4,HIGH);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else if(_driverMode == 8){
    _AFMstatus = _in1Status | _in4Status;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    analogWrite(_pwmA,speedA);
    analogWrite(_pwmB,speedB);
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
}

/****************************stop function*********************************/
void BOXZ::stop()
{
  if(_driverMode == 2){
    digitalWrite(_inA,LOW);
    digitalWrite(_inB,LOW);
    digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,LOW);
    if(_inCHx>2){
      digitalWrite(_inC,LOW);
      digitalWrite(_inD,LOW);
      digitalWrite(_pwmC,LOW);
      digitalWrite(_pwmD,LOW);
    }
    //if(DEBUG == 1) Serial.println("FORWARD");
  }
  else if(_driverMode == 4){
    /*disable the enble pin, to stop the motor. */
    digitalWrite(_inA,LOW);
    digitalWrite(_inB,LOW);
    digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,LOW);
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,HIGH);
    digitalWrite(_in2,HIGH);
    digitalWrite(_in3,HIGH);
    digitalWrite(_in4,HIGH);
    digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,LOW);
  }
  else if(_driverMode == 8){
    _AFMstatus = 0xFF;
    digitalWrite(AF_DIR_LATCH, LOW);
    shiftOut(AF_DIR_SER, AF_DIR_CLK, LSBFIRST, _AFMstatus);   
    digitalWrite(AF_DIR_LATCH, HIGH);
    digitalWrite(_pwmA,LOW);
    digitalWrite(_pwmB,LOW);
  }
  else{
    //if(DEBUG == 1) Serial.println(F("ERROR:UNKNOWN MODE"));
  } 
  //  if(DEBUG == 1) Serial.println("STOP");
}



/****************************RAW control function*********************************/


/*motorRaw() mode
 You can control you motor with raw data(Long int HEX), The format is 0xF|0xFF|0xFF
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

void BOXZ::motorRaw(unsigned long data)
{

  _speedA = lowByte(data); //Right speed
  _speedB = highByte(data); //Left speed
  _in4Status = bitRead(data, 16); 
  _in3Status = bitRead(data, 17); 
  _in2Status = bitRead(data, 18); 
  _in1Status = bitRead(data, 19);
  //Output
  if(_driverMode == 4){
    digitalWrite(_inA,_in3Status);
    digitalWrite(_inB,_in4Status);
    analogWrite(_pwmA,_speedA);
    analogWrite(_pwmB,_speedB);
  }
  else if(_driverMode == 6){
    digitalWrite(_in1,_in1Status);
    digitalWrite(_in2,_in2Status);
    digitalWrite(_in3,_in3Status);
    digitalWrite(_in4,_in4Status);
    analogWrite(_pwmA,_speedA);
    analogWrite(_pwmB,_speedB);
  }
}

/*motorRaws() mode
 You can control you motor with raw data(String HEX). The format is 0xF|0xFF|0xFF
 Input value is a string.
 If you want to goForward in 4 pin mode, you should send "3FFFF", not "0x3FFFF"
 */
void BOXZ::motorRaws(String datas)
{
  int rawLength = 5;      //valid length of string
  int data;           //character of data
  unsigned long datain =0;    //data input 0xFFFFF
  unsigned long power =1;     //power of 16 such as 16^0, 16^1, 16^2...
  int datasl = datas.length();  //Data string length

    //String HEX to Long HEX
  if (datasl >0 && datasl <= rawLength)
  {
    for(int n=datasl-1;n>=0;n--){
      data = datas.charAt(n);
      if(data>=48 && data <=57){
        datain += (data-48)*power;
      }
      else if(data>=65 && data <=70){
        datain += (data-65+10)*power;
      }
      else{
        datain += 0;
      }
      power *= 16;
    }
    //action
    boxz.motorRaw(datain);
  }
}

/****************************initialization function for Servo*********************************/
void BOXZ::initServo(){
  int pin01 = SERVO_PIN01;
  int pin02 = SERVO_PIN02;
  servo01.attach(pin01);  
  servo02.attach(pin02);  
  _servoPos01 = SERVO_POS01;
  _servoPos02 = SERVO_POS02;
  _servoPosMin = SERVO_POSMIN;
  _servoPosMax = SERVO_POSMAX;
  _servoDelay = SERVO_DELAY;
  servo01.write(_servoPos01); 
  servo02.write(_servoPos02); 
}

// initialization servo with pin define
void BOXZ::initServo(int pin01,int pin02){
  servo01.attach(pin01); 
  servo02.attach(pin02); 
  _servoPos01 = SERVO_POS01;
  _servoPos02 = SERVO_POS02;
  _servoPosMin = SERVO_POSMIN;
  _servoPosMax = SERVO_POSMAX;
  _servoDelay = SERVO_DELAY;
  servo01.write(_servoPos01); 
  servo02.write(_servoPos02); 
}

// initialization servo with pin define and range limit
void BOXZ::initServo(int pin01,int pin02, int posMin, int posMax){
  servo01.attach(pin01);
  servo02.attach(pin02);
  _servoPos01 = SERVO_POS01;
  _servoPos02 = SERVO_POS02;
  _servoPosMin = posMin;
  _servoPosMax = posMax;
  _servoDelay = SERVO_DELAY;
  servo01.write(_servoPos01); 
  servo02.write(_servoPos02); 
}


/****************************action function for Servo*********************************/

//action for servo
//Left hand up(default action) From Max to Min
void BOXZ::servo01Up(){
  for(_servoPos01 = _servoPosMax; _servoPos01 >= _servoPosMin; _servoPos01 -= 1)  
  {                                
    servo01.write(_servoPos01);          
    delay(_servoDelay);                       
  } 
}

//Left hand down(default action) From Min to Max
void BOXZ::servo01Down(){
  for(_servoPos01 = _servoPosMin; _servoPos01 <= _servoPosMax; _servoPos01 += 1)  
  {                                
    servo01.write(_servoPos01);          
    delay(_servoDelay);                       
  } 
}

//Right hand up(default action) From Min to Max
void BOXZ::servo02Up(){
  for(_servoPos02 = _servoPosMin; _servoPos02 <= _servoPosMax; _servoPos02 += 1)  
  {                                
    servo02.write(_servoPos02);          
    delay(_servoDelay);                       
  } 
}

//Right hand down(default action) From Max to Min
void BOXZ::servo02Down(){
  for(_servoPos02 = _servoPosMax; _servoPos02 >= _servoPosMin; _servoPos02 -= 1)  
  {                                
    servo02.write(_servoPos02);          
    delay(_servoDelay);                       
  } 
}

/****************************action(Type) function for Servo*********************************/

//Left hand up(type 1 = step; type 2 = consecutive; else default mode)
void BOXZ::servo01Up(int type){
  if(type == 1){
    _servoPos01 = servo01.read();
    _servoPos01 -=10;  
    _servoPos01 = max(_servoPos01,_servoPosMin);
    servo01.write(_servoPos01);          
    delay(_servoDelay);  
  }  
  else if(type == 2){
    for(_servoPos01 = servo01.read(); _servoPos01 >= _servoPosMin; _servoPos01 -= 1)  
    {                                
      servo01.write(_servoPos01);          
      delay(_servoDelay);                       
    } 
  }
  else{
    servo01Up();
  }
}

//Left hand down(type 1 = step; type 2 = consecutive; else default mode)
void BOXZ::servo01Down(int type){
  if(type == 1){
    _servoPos01 = servo01.read();
    _servoPos01 +=10;  
    _servoPos01 = min(_servoPos01,_servoPosMax);
    servo01.write(_servoPos01);          
    delay(_servoDelay); 
  }  
  else if(type == 2){
    for(_servoPos01 = servo01.read(); _servoPos01 <= _servoPosMax; _servoPos01 += 1)  
    {                                
      servo01.write(_servoPos01);          
      delay(_servoDelay);                       
    } 
  }
  else{
    servo01Down();
  }
}


//Right hand up(type 1 = step; type 2 = consecutive; else default mode)
void BOXZ::servo02Up(int type){
  if(type == 1){
    _servoPos02 = servo02.read();
    _servoPos02 +=10;  
    _servoPos02 = min(_servoPos02,_servoPosMax);
    servo02.write(_servoPos02);          
    delay(_servoDelay); 
  }  
  else if(type == 2){
    for(_servoPos02 = servo02.read(); _servoPos02 <= _servoPosMax; _servoPos02 += 1)  
    {                                
      servo02.write(_servoPos02);          
      delay(_servoDelay);                       
    } 
  }
  else{
    servo02Up();
  }
}

//Right hand down(type 1 = step; type 2 = consecutive; else default mode)
void BOXZ::servo02Down(int type){
  if(type == 1){
    _servoPos02 = servo02.read();
    _servoPos02 -=10;  
    _servoPos02 = max(_servoPos02,_servoPosMin);
    servo02.write(_servoPos02);          
    delay(_servoDelay); 
  }  
  else if(type == 2){
    for(_servoPos02 = servo02.read(); _servoPos02 >= _servoPosMin; _servoPos02 -= 1)  
    {                                
      servo02.write(_servoPos02);          
      delay(_servoDelay);                       
    } 
  }
  else{
    servo02Down();
  }
}

/*
 You can control you servo with raw data(Long int HEX), The format is 0xF|0xFF|0xFF
 Define
 - Servo RAW data
 Servo01 = Left hand
 Servo02 = Right hand
 
 The RAW format is 0xF|0xFF|0xFF
 BOXZ Pro
 Byte 1(High): Control bit
 Byte 2-3: Servo02(Right) degree from 0x00 to 0xB4(DEC is 180 degree)
 Byte 4-5(Low): Servo01(Left) degree from 0x00 to 0xB4(DEC is 180 degree)
 
 Control bit
 B0000: Empty
 B0001: Left hand servo active, if not ignore PARA degree
 B0010: Right hand servo active
 B0011: Left and Right hand servo active
 
 BOXZ MAX(further function)
 Byte 1(High): Control bit
 Byte 2-3: ServoR0/1/2/3(Right Group) degree from 0x00 to 0xB4(DEC is 180 degree)
 Byte 4-5(Low): ServoL0/1/2/3(Left Group) degree from 0x00 to 0xB4(DEC is 180 degree)
 
 Control bit
 B0100: L0-Left Arm Group Servo ID 0
 B0101: L1-Left Arm Group Servo ID 1
 B0110: L2-Left Arm Group Servo ID 2
 B0111: L3-Left Arm Group Servo ID 3
 B1000: R0-Right Arm Group Servo ID 0
 B1001: R1-Right Arm Group Servo ID 1
 B1010: R2-Right Arm Group Servo ID 2
 B1011: R3-Right Arm Group Servo ID 3
 
 Servo raw type
 - (further function)servoRawtype(int type)
 0: default mode(BOXZ Pro) - arrived new position at same time
 1: (spare)speed mode? - same speed
 2: (spare)
 3: (spare)BOXZ MAX?
 
 
 If you want to control both of servo hands up
 you should send "262143", not "0x3FFFF"
 But because of SERVO_POSMAX is 160, it will stop at 160 degree
 Or you also could send "237728", this means 0x3A0A0 and 0xA0 is 160 in DEC
 */
void BOXZ::servoRaw(unsigned long data){
  //Input RAW data
  _servoTar01 = lowByte(data);
  _servoTar02 = highByte(data);
  _servoAct01 = bitRead(data, 16);
  _servoAct02 = bitRead(data, 17);
  _servoPos01 = servo01.read();
  _servoPos02 = servo02.read();
  //Global variable
  _servoPosMax = SERVO_POSMAX;
  _servoPosMin = SERVO_POSMIN;
  _servoFrame = SERVO_FRAME;
  //limit value from 20 to 160 degree
  _servoTar01 = min(_servoTar01,_servoPosMax);
  _servoTar01 = max(_servoTar01,_servoPosMin);
  _servoTar02 = min(_servoTar02,_servoPosMax);
  _servoTar02 = max(_servoTar02,_servoPosMin); 
  //Calculate degree
  if(_servoAct01 ==1 || _servoAct02 ==1){
    _servoFra01 = int((10*(_servoTar01 - _servoPos01))/_servoFrame);
    _servoFra02 = int((10*(_servoTar02 - _servoPos02))/_servoFrame);
    for(int i = 0;i <= _servoFrame; i++){
      _servoDis01 = 600+10*_servoPos01 + i*_servoFra01;
      _servoDis02 = 600+10*_servoPos02 + i*_servoFra02;
      if(_servoAct01 ==1) servo01.writeMicroseconds(_servoDis01); 
      if(_servoAct02 ==1) servo02.writeMicroseconds(_servoDis02);   
      delay(_servoDelay);  
    }
  }
}

/*servoRaws() mode
 You can control you servo with raw data(String HEX). The format is 0xF|0xFF|0xFF
 Input value is a string.
 If you want to control both of servo hands up
 you should send "3FFFF", not "0x3FFFF"
 But because of SERVO_POSMAX is 160, it will stop at 160 degree
 Or you also could send "3A0A0", this means 0x3A0A0 and 0xA0 is 160 in DEC
 */
void BOXZ::servoRaws(String datas)
{
  int rawLength = 5;      //valid length of string
  int data;           //character of data
  unsigned long datain =0;    //data input 0xFFFFF
  unsigned long power =1;     //power of 16 such as 16^0, 16^1, 16^2...
  int datasl = datas.length();  //Data string length

    //String HEX to Long HEX
  if (datasl >0 && datasl <= rawLength)
  {
    for(int n=datasl-1;n>=0;n--){
      data = datas.charAt(n);
      if(data>=48 && data <=57){
        datain += (data-48)*power;
      }
      else if(data>=65 && data <=70){
        datain += (data-65+10)*power;
      }
      else{
        datain += 0;
      }
      power *= 16;
    }
    //DEBUG MODE
    //action
    servoRaw(datain);
  }
}
/****************************keyword communication function*********************************/
//BOXZ base keyword mode function
void BOXZ::motorCom(int keyword)
{
	
  if(keyword == 'w' || keyword == 'A') goForward();
  if(keyword == 's' || keyword == 'B') goBackward();
  if(keyword == 'a' || keyword == 'C') goLeft();
  if(keyword == 'd' || keyword == 'D') goRight();
  
  if(keyword == 'q' || keyword == 'F') {
	  if(_driveParaSD == 1){
		  goForward(0,DEFAULT_SPEED); 
	  }else{
		  goForward(DEFAULT_SPEED,0); 
	  }
  }
  if(keyword == 'e' || keyword == 'N') {
	  if(_driveParaSD == 1){
		  goForward(DEFAULT_SPEED,0); 
	  }else{
		  goForward(0,DEFAULT_SPEED); 
	  }
  }
  if(keyword == 'z') goBackward(0,DEFAULT_SPEED); 
  if(keyword == 'x') goBackward(DEFAULT_SPEED,0); 
  if(keyword == ' ' || keyword == 'K' || keyword == 'L' || keyword == 'I' || keyword == 'J') stop();
}

//SpeedA is the speed of left motor
//SpeedB is the speed of right motor
void BOXZ::motorCom(int keyword, int speedA, int speedB)
{
  if(keyword == 'w') goForward(speedA,speedB);
  if(keyword == 's') goBackward(speedA,speedB);
  if(keyword == 'a') goLeft(speedA, speedB);
  if(keyword == 'd') goRight(speedA,speedB);
  if(keyword == 'q') {
	  if(_driveParaSD == 1){
		  goForward(0,speedA); 
	  }else{
		  goForward(speedA,0); 
	  }
  }
  if(keyword == 'e') {
	  if(_driveParaSD == 1){
		  goForward(speedB,0); 
	  }else{
		  goForward(0,speedB);
	  }
  }
  if(keyword == 'z') goBackward(0,speedB);
  if(keyword == 'x') goBackward(speedA,0);
  if(keyword == ' ') stop();
}

void BOXZ::motorCom(int speedA, int speedB)
{

  if(speedA >= 0 && speedB >= 0) {
    goForward(speedA,speedB);
  }
  if(speedA <= 0 && speedB <= 0) {
    speedA = -speedA;
    speedB = -speedB;
    goBackward(speedA,speedB);
  }
  if(speedA <= 0 && speedB >= 0) {
    speedA = -speedA;
    goLeft(speedA,speedB);
  }
  if(speedA >= 0 && speedB <= 0) {
    speedB = -speedB;
    goRight(speedA,speedB);
  }
  if(speedA <= 100 && speedB <= 100) stop();
}

void BOXZ::servoCom(int keyword){
  switch (keyword){
  case 'u':
    servo01Up();
    break;
  case 'i':
    servo02Up();
    break;
  case 'j':
    servo01Down();
    break;
  case 'k':
    servo02Down();
    break;
  case 'o':
    servoRaw(0x3FF00);
    break;
  case 'l':
    servoRaw(0x300FF);
    break;
  case 'U':
    servo01Up(1);
    break;
  case 'I':
    servo02Up(1);
    break;
  case 'J':
    servo01Down(1);
    break;
  case 'K':
    servo02Down(1);
    break;
  case 'O':
    servo01Up(1);
    servo02Up(1);
    break;
  case 'L':
    servo01Down(1);
    servo02Down(1);
    break;
  default :
    //Do nothing
    return;
  }
}

//servoTar01 is the servo degree of left hand
//servoTar02 is the servo degree of Right hand
void BOXZ::servoCom(int servoTar01, int servoTar02){
  //Global variable
  _servoPosMax = SERVO_POSMAX;
  _servoPosMin = SERVO_POSMIN;
  _servoFrame = SERVO_FRAME;
  //limit value from 20 to 160 degree
  servoTar01 = min(servoTar01,_servoPosMax);
  servoTar01 = max(servoTar01,_servoPosMin);
  servoTar02 = min(servoTar02,_servoPosMax);
  servoTar02 = max(servoTar02,_servoPosMin); 
  //Calculate degree
  _servoFra01 = int((10*(servoTar01 - _servoPos01))/_servoFrame);
  _servoFra02 = int((10*(servoTar02 - _servoPos02))/_servoFrame);
  for(int i = 0;i <= _servoFrame; i++){
    _servoDis01 = 600+10*_servoPos01 + i*_servoFra01;
    _servoDis02 = 600+10*_servoPos02 + i*_servoFra02;
    servo01.writeMicroseconds(_servoDis01); 
    servo02.writeMicroseconds(_servoDis02);   
    delay(_servoDelay);  
  }
}

BOXZ boxz;



