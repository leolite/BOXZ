//  BOXZ for Seeed
//  by Leo.Zhu
//  MotorDriver lib By Frankie.Chu

//Updated 2013.04.27
//1. fixed The Software Servo Library to support Arduino 1.0
//2. the MotorMriver lib goLeft() and goRight() is different with BOXZ.

//Hardware list
//Seeed Seeeduino V3.0 (Atmega 328P) http://www.seeedstudio.com/depot/seeeduino-v30-atmega-328p-p-669.html?cPath=132_133
//Seeed Motor Shield V2.0 http://www.seeedstudio.com/depot/motor-shield-v20-p-1377.html?cPath=132_134
//Seeed Bluetooth Shield http://www.seeedstudio.com/depot/bluetooth-shield-p-866.html?cPath=132_134

/*------------------------------------------------------------------
 I/O define
 D0  BT-TX
 D1  BT-RX
 D2  Spare
 D3  Spare(IR LED sender)
 D4  Spare
 D5  Servo 1 left hand
 D6  Servo 2 right hand
 D7  Spare
 A0  Spare
 A1  Spare
 A2  Spare
 A3  Spare
 A4  I2C-SDA
 A5  I2C-SCL
 
 D9 - D13 for Seeed Motor Shield V2.0
 ------------------------------------------------------------------*/

#include <SoftwareServo.h>
#include "MotorDriver.h"

int testmode = 0;
//------------------------------------------------------------------
//define motor
int motor_speed = 255; //[modifid]motor speed 150-200,---min:100;max:255
int motor_speedf = 50; //Speed fixed for Go left or right

/*------------------------------------------------------------------
 define servo
 ------------------------------------------------------------------*/

int S1_pin = 5;
int S2_pin = 6;
SoftwareServo hand_l_servo;  // create servo object to control a servo 
SoftwareServo hand_r_servo;  // create servo object to control a servo
int hand_l_pos = 90; // 
int hand_r_pos = 90; //
int hand_delay = 1;  //[modifid] speed of hand



void setup()
{
  Serial.begin(9600);
  /*Configure the motor A to control the wheel at the left side.*/
  /*Configure the motor B to control the wheel at the right side.*/
  motordriver.init();
  motordriver.setSpeed(motor_speed,MOTORB);
  motordriver.setSpeed(motor_speed,MOTORA);
  b_servo_ini();
  delay(500);  //waiting time
  Serial.println("Hello! BOXZ!");
}

void loop()
{
  int  key;
  if(Serial.available() > 0) {    
    key = Serial.read();  
    if(key >= 30 && key <= 122) { 
      b_motor_com(key); 
      b_servo_com(key);
    }
  }

}


//---------------------------------motor-------------------------------//
// b_motor_com(keyword);//keyboard "w,a,s,d" for action;space for stop;

void b_motor_com(int keyword){
  switch (keyword){
  case 'w':
    motordriver.setSpeed(motor_speed,MOTORB);
    motordriver.setSpeed(motor_speed,MOTORA);
    motordriver.goForward();
    if(testmode == 1){
      Serial.println("FORWARD");
    }
    break;
  case 's':
    motordriver.setSpeed(motor_speed,MOTORB);
    motordriver.setSpeed(motor_speed,MOTORA);
    motordriver.goBackward();
    if(testmode == 1){
      Serial.println("BACKWARD");
    }
    break;
  case 'q':
    motordriver.setSpeed(motor_speed-motor_speedf,MOTORB);
    motordriver.setSpeed(motor_speed,MOTORA);
    motordriver.goForward();
    if(testmode == 1){
      Serial.println("Go LEFT");
    }
    break;
  case 'e':
    motordriver.setSpeed(motor_speed,MOTORB);
    motordriver.setSpeed(motor_speed-motor_speedf,MOTORA);
    motordriver.goForward();
    if(testmode == 1){
      Serial.println("Go RIGHT");
    }
    break;
  case 'a':
    motordriver.setSpeed(motor_speed,MOTORB);
    motordriver.setSpeed(motor_speed,MOTORA);
    motordriver.goRight();
    if(testmode == 1){
      Serial.println("Turn LEFT");
    }
    break;
  case 'd':
    motordriver.setSpeed(motor_speed,MOTORB);
    motordriver.setSpeed(motor_speed,MOTORA);
    motordriver.goLeft();
    if(testmode == 1){ 
      Serial.println("Turn RIGHT");
    }
    break;
  case ' ':
    motordriver.stop();
    if(testmode == 1){
      Serial.println("STOP");
    }

    break;
  default :
    motordriver.stop();
    if(testmode == 1){
      Serial.println("STOP");
    }
    return;
  }
}


//---------------------------------servo-------------------------------//
/*
Single click
 u - Left hand up
 i - Right hand up
 o - both hand up
 j - Left hand down
 k - Right hand down
 l - both hand down
 
 Double click: each hand sweep
 shift + key:  step action
 */

void b_servo_ini(){
  hand_l_servo.attach(S1_pin);  // attaches the servo on pin to the servo object 
  hand_r_servo.attach(S2_pin);  // attaches the servo on pin to the servo object 
  hand_l_servo.write(hand_l_pos); 
  hand_r_servo.write(hand_r_pos); 
  delay(10);
  SoftwareServo::refresh(); 
}

void b_servo_com(int keyword){
  int pos;
  //left hand up
  if(keyword=='u')
  {
    for(pos = 0; pos < 180; pos += 1)  
    {                                
      hand_l_servo.write(180-pos);          
      delay(hand_delay); 
      SoftwareServo::refresh();      
    } 
  }
  //left hand down
  else if(keyword=='j'){
    for(pos = 180; pos>=1; pos-=1)    
    {                                
      hand_l_servo.write(180-pos);         
      delay(hand_delay);  
      SoftwareServo::refresh();      
    } 
  }

  //right hand up
  if(keyword=='i')
  {
    for(pos = 0; pos < 180; pos += 1) 
    {                                 
      hand_r_servo.write(pos);             
      delay(hand_delay);  
      SoftwareServo::refresh();      
    } 
  }
  //right hand down
  else if(keyword=='k'){
    for(pos = 180; pos>=1; pos-=1)    
    {                                
      hand_r_servo.write(pos);             
      delay(hand_delay);  
      SoftwareServo::refresh();      
    } 
  }
  //both hand up
  else if(keyword=='o'){   
    for(pos = 0; pos < 180; pos += 1) 
    {    
      hand_l_servo.write(180-pos);          
      hand_r_servo.write(pos);                                      
      delay(hand_delay); 
      SoftwareServo::refresh();      
    }
  }
  //both hand down
  else if(keyword=='l'){

    for(pos = 180; pos>=1; pos-=1)    
    {   
      hand_l_servo.write(180-pos);        
      hand_r_servo.write(pos);    
      delay(hand_delay);   
      SoftwareServo::refresh();      
    }
  }
  else  //left hand up step
  if(keyword=='U')
  {
    hand_l_pos -=10;  
    hand_l_pos = max(hand_l_pos,0);
    hand_l_servo.write(hand_l_pos);          
    delay(hand_delay);                       
    SoftwareServo::refresh();
  }
  //left hand down step
  else if(keyword=='J'){
    hand_l_pos +=10;      
    hand_l_pos = min(hand_l_pos,180);    
    hand_l_servo.write(hand_l_pos);         
    delay(hand_delay);                    
    SoftwareServo::refresh();
  }

  //right hand up step
  if(keyword=='I')
  {
    hand_r_pos +=10;     
    hand_r_pos = min(hand_r_pos,180);   
    hand_r_servo.write(hand_r_pos);             
    delay(hand_delay);  
    SoftwareServo::refresh();    
  }
  //right hand down step
  else if(keyword=='K'){
    hand_r_pos -=10;      
    hand_r_pos = max(hand_r_pos,0);    
    hand_r_servo.write(hand_r_pos);             
    delay(hand_delay);     
    SoftwareServo::refresh();    
  } 

  //both hand up step
  else if(keyword=='O'){
    hand_l_pos +=10;    
    hand_r_pos +=10;    
    hand_l_pos = max(hand_r_pos,0); 
    hand_r_pos = min(hand_r_pos,180); 
    hand_l_servo.write(180-hand_l_pos);          
    hand_r_servo.write(hand_r_pos);                                      
    delay(hand_delay);    
    SoftwareServo::refresh();    
  }
  //both hand down step
  else if(keyword=='L'){
    hand_l_pos -=10;    
    hand_r_pos -=10; 
    hand_l_pos = min(hand_r_pos,180); 
    hand_r_pos = max(hand_r_pos,0); 
    hand_l_servo.write(180-hand_l_pos);        
    hand_r_servo.write(hand_r_pos);    
    delay(hand_delay);    
    SoftwareServo::refresh();
  }
  else{ 
    // no action
  }
  if(testmode ==1){
    Serial.print("Left Pos:");
    Serial.print(hand_l_pos);
    Serial.print(" ; Right Pos:");
    Serial.println(hand_r_pos);
  }

}






