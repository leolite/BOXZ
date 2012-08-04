// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
int value_health = 10;
int value_mana = 10;


void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}


void loop()
{
  value_health-=1;
  value_mana-=2;
  if(value_health<2) value_health=10;
  if(value_mana<2) value_mana=10;
  Wire.beginTransmission(3); // transmit to device #4
  Wire.send(value_health);    // sends one byte  
  Wire.endTransmission();    // stop transmitting
  Wire.beginTransmission(4); // transmit to device #4
  Wire.send(value_mana);    // sends one byte  
  Wire.endTransmission();    // stop transmitting
  delay(1000);
}

