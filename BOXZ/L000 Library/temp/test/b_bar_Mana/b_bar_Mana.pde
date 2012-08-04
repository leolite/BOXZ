/*
  BOXZ LED Bar for Mana
 
 The circuit:
 * LEDs from pins 3 through 12 to ground
 */


// these constants won't change:

#include <Wire.h>

// int value_mana = 10;
const int ledCount = 10;    // the number of LEDs in the bar graph
int ledPins[] = { 
  3, 4, 5, 6, 7,8,9,10,11,12 };   // an array of pin numbers to which LEDs are attached


void setup() {
  // loop over the pin array and set them all to output:
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    pinMode(ledPins[thisLed], OUTPUT); 
  }

  //*************initial for LED running cartoon****************
  //All LED light
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    // if the array element's index is less than ledLevel,
    // turn the pin for this element on:
    digitalWrite(ledPins[thisLed], HIGH);
  } 
  delay(1000);
  //All LED off
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    // if the array element's index is less than ledLevel,
    // turn the pin for this element on:
    digitalWrite(ledPins[thisLed], LOW);
  } 
  delay(1000);
  //LED light on step by step
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {
    // if the array element's index is less than ledLevel,
    // turn the pin for this element on:
    digitalWrite(ledPins[thisLed], HIGH);
    delay(400);
  } 

  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event

}

void loop() {
  delay(100);
}

void receiveEvent(int howMany)
{
  if(Wire.available() > 0) {    
    int ledLevel =  Wire.receive();

    // loop over the LED array:
    for (int thisLed = 0; thisLed < ledCount; thisLed++) {
      // if the array element's index is less than ledLevel,
      // turn the pin for this element on:
      if (thisLed < ledLevel) {
        digitalWrite(ledPins[thisLed], HIGH);
      } 
      // turn off all pins higher than the ledLevel:
      else {
        digitalWrite(ledPins[thisLed], LOW); 
      }
    }
    //  Serial.println(ledLevel); 
  }
}



