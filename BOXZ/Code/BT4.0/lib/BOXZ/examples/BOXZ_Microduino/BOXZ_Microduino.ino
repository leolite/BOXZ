//  BOXZ Mini
//  Author:Leo.Zhu

//  website EN: www.boxz.cc
//  website CN: www.diyfun.org
//  QQ Group: 248742803
//  Download: github.com/leolite/BOXZ
//  License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
//  http://creativecommons.org/licenses/by-nc-sa/3.0/


//  Hardware support list
//  1. Microduino Robot board

//tested ok by Leo 2014.12.26

#include <EEPROM.h>
#include <aJSON.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX
int pinRx = 4;
int pinTx = 5;


aJsonStream serial_stream(&mySerial);

//Serial speed config
//unsigned long serialSpeed = 115200; //9600 for HC
unsigned long serialSpeed = 9600;

int testmode =0; // 0: Disable; 1: testMode
boolean serialDataDone = true; //shift bit for testmode output serial
//hardware define
int pinIO[]={
  2,3,A0,A1,A2,A3,11,12}; //pin 14,15,16,17
int pinMotor[]={
  4,7,5,6};
int pinServo[]={
  9,10};
int pinSoftSerial = 8;


//value Data
// - CF:Config Byte 00 - 08
int valueME; //Message
int valueHP; //Health para
int valueMP; //Magic para
int valueVB; //Visu Byte


// - AT:Action
int valueK1; //Button key 1 for BOXZ direction control
int valueK2; //Button key 2 for function key A/B/C/D
int valueV1; //motor speed left
int valueV2; //motor speed right

//temp Value
int valueHPtemp; //Health para
int valueMPtemp; //Magic para

//for JSON
boolean watchDogEn;
int watchDogCountLimit = 3;
int watchDogCount = 0;

//add for connection online timer by Leo
boolean watchDogTimerEn = false;
static unsigned long watchDogTimer = 0;
static unsigned long watchDogLimit = 5000;

//System RAM
uint16_t biggest;



void setup()
{
  pinMode(pinTx,OUTPUT);
  pinMode(pinRx,INPUT_PULLUP);
  initDrive();
  //boxz.initServo();
  mySerial.begin(serialSpeed);
  initJSON();

  //test function. APP should send {"AT":{"V1":255}} and {"AT":{"V2":255}}   2014.09.23 add by Leo

  //ini..
  valueV1 = 0xFF;
  valueV2 = 0xFF;
  valueHP = 0xFF;
  valueMP = 0xFF;
}


void loop()
{
  //Serial data input(include watch dog function and JSON data input process)
  //This block will update the data of BOXZ(such as HP/MP....)
  serialDataInput();

  //process area start
  //process data at serial data input finished
  if(serialDataDone == true){
    motorCom(valueK1); //2014.09.26 updata to here. 
    //boxz.servoCom(valueK2);
  }

  //process data every loop cycle
  heartbeat();//2014.09.02 add by orge_c
  testFunction(testmode); //2014.09.02 add by orge_c

  //userdefined();

  //value limit, fix the value out of range
  valueCheck();
  //Serial data output, send JSON data out
  serialDataOutput();
  //Reset serial data done
  serialDataReset();
}




