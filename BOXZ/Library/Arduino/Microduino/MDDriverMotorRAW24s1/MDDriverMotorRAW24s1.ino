//  MDDriverMotorRAW
//  Demo function:The application method to drive the 2x DC motor.
//  Author:Leo.Zhu
//  Date:27 Sep, 2013
//  https://github.com/leolite/BOXZ
//  Hardware support list
//  1. Microduino A3906

#include "BOXZ.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;

// Structure of our payload
struct payload_t
{
  uint32_t ms;
  uint32_t sensorData;
};

static long timer;
uint32_t temp;


int speedA_value = 255;
int speedB_value = 255;
long  key;


void setup()
{
  Serial.begin(9600);
  //A3906 IN1,IN3,IN2,IN4
  boxz.initMotor(7,8,5,6);
  Serial.println("RF24Network/examples/helloworld_rx/");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  Serial.println("Hello! BOXZ!");
}

void loop()
{
  // Pump the network regularly
  network.update();
  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    temp=payload.sensorData;
    //    Serial.print("Received Sensor Data #");
    //    Serial.print(temp);
    key = temp;
    //boxz.motorRaw(key); 
  }
  boxz.motorRaw(key); 

}




