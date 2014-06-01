//  MDDriverMotorRAW
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:27 Sep, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. Microduino A3906

// W ----- A
// S ----- D
// both button is stop

#include "BOXZ.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

// How often to send 'hello world to the other unit
const unsigned long interval = 15000; //ms

// When did we last send?
unsigned long last_sent;

// How many have we sent already
//unsigned long packets_sent;

// Structure of our payload
struct payload_t
{
  uint32_t ms;
  uint32_t sensorData;
};


uint32_t temp;
int key;
int key2;
//int pin_inA = 4;
//int pin_inB = 7;
//int pin_pwmA = 5;
//int pin_pwmB = 6;
int speedA_value = 255;
int speedB_value = 255;

int driveID = 0xDF;  //Driver ID is DFROBOT
unsigned long keyW = 0x33311;
unsigned long keyS = 0x0AAFF;
unsigned long keyA = 0x133FF;
unsigned long keyD = 0x2CC33;
unsigned long keySTOP = 0x3FFFF;
void setup()
{
  Serial.begin(9600);
  Serial.println("RF24Network/examples/helloworld_tx/");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  //key 3,4,5,6
  for(int i =3; i<7; i++){
    pinMode(i,INPUT);
  }

  Serial.println("Hello! BOXZ!");
}

void loop()
{
  // Pump the network regularly
  network.update();
  keyAction();
  if(key2 != key){
    Serial.println(key);
    key2 = key;
    b_motor_com(key);
    payload_t payload = { 
      temp,temp    };  
    RF24NetworkHeader header(/*to node*/ other_node);
    network.write(header,&payload,sizeof(payload)); 
    Serial.println(temp);
  }
}

void keyAction(){
  if(key == ' '){
   delay(200); 
  }
  if(digitalRead(3) == 1){
    key = 'a';
  }
  if(digitalRead(4) == 1){
    key = 'd';
  }
  if(digitalRead(5) == 1){
    key = 'w';
  }
  if(digitalRead(6) == 1){
    key = 's';
  }
  if(digitalRead(3) == 1&&digitalRead(4) == 1){
    key = ' ';
  }
  if(digitalRead(5) == 1&&digitalRead(6) == 1){
    key = ' ';
  }
}
void b_motor_com(int keyword){
  switch (keyword){
  case 'w':
    temp =keyW;
    break;
  case 's':
    temp =keyS;
    break;
  case 'a':
    temp =keyA;
    break;
  case 'd':
    temp =keyD;
    break;
  case ' ':
    temp = keySTOP;
    break;
  default :
    temp = keySTOP;
    return;
  }
}







