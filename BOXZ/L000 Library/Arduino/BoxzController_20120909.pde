/*
BOXZ is is an open source robot platform for interactive entertainment! 
 We connected Arduino,Plexiglass(PMMA) Origami and your idea together, 
 so we can easily and quickly build a remote robot by ourself! 
 We can use it to playing football game, racing, fighting or some other interesting things!
 
 Author: Guangjun Zhu
 Email: zgj0729@163.com
 
 Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 */

/*
Update
 
 2012.09.08 By Leo 
 1. update 'a' and 'd' for turning slant
 2. add 'q' and 'e' for turning around
 */

#include <AFMotor.h>
#include <Servo.h> 

/*
 b_motor_stop(): stop motor
 b_motor_com():  for Manual Operator
 b_servo_ini();
 b_servo_com();
 b_skill();
 */

//-------------------define motor----------------------------------------------//
AF_DCMotor motorL(1,MOTOR12_8KHZ);  //connect to M1
AF_DCMotor motorR(2,MOTOR12_8KHZ);  //connect to M2
int motor_speed = 255; //[modifid]motor speed 150-200,---min:100;max:255
int value_speed = 255; //[modifid]delay time in step

//-------------------define servo----------------------------------------------//
Servo hand_l_servo;  // create servo object to control a servo 
Servo hand_r_servo;  // create servo object to control a servo
int hand_l_pos = 90; // 
int hand_r_pos = 90; //
int hand_delay = 1;  //[modifid] speed of hand

//------------------main program-----------------------------------------------//

void setup()
{
  Serial.begin(9600);
  b_motor_stop();
  b_servo_ini();
  delay(2000);  //waiting time
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
  // servo_test();
}

//---------------------------------motor-------------------------------//
//keyboard "w,a,s,d" for action;space for stop

void b_motor_stop(){
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}

void b_motor_com(int keyword){
  //forward
  if(keyword=='w')
  {    
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
  }
  //backward
  else if(keyword=='s'){
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed);
    motorL.run(BACKWARD);
    motorR.run(BACKWARD);
  }
  //forward left
  else if(keyword=='a')
  {
    motorL.setSpeed(motor_speed-50);
    motorR.setSpeed(motor_speed);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
  }
  //forward right
  else if(keyword=='d')
  {
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed-50);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
  }
  //turn left
  else if(keyword=='q'){
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed);
    motorL.run(BACKWARD);
    motorR.run(FORWARD);
  }
  //turn right
  else if(keyword=='e'){
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed);
    motorL.run(FORWARD);
    motorR.run(BACKWARD);
  }
  //Stop
  else if(keyword==' '){
    b_motor_stop();
  }
  else{
    b_motor_stop();
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
  hand_l_servo.attach(9);  // attaches the servo on pin 9 to the servo object 
  hand_r_servo.attach(10);  // attaches the servo on pin 10 to the servo object 
  hand_l_servo.write(hand_l_pos); 
  hand_r_servo.write(hand_r_pos); 
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
    } 
  }
  //left hand down
  else if(keyword=='j'){
    for(pos = 180; pos>=1; pos-=1)    
    {                                
      hand_l_servo.write(180-pos);         
      delay(hand_delay);                    
    } 
  }

  //right hand up
  if(keyword=='i')
  {
    for(pos = 0; pos < 180; pos += 1) 
    {                                 
      hand_r_servo.write(pos);             
      delay(hand_delay);                       
    } 
  }
  //right hand down
  else if(keyword=='k'){
    for(pos = 180; pos>=1; pos-=1)    
    {                                
      hand_r_servo.write(pos);             
      delay(hand_delay);                    
    } 
  }
  //both hand up
  else if(keyword=='o'){   
    for(pos = 0; pos < 180; pos += 1) 
    {    
      hand_l_servo.write(180-pos);          
      hand_r_servo.write(pos);                                      
      delay(hand_delay);                       
    }
  }
  //both hand down
  else if(keyword=='l'){

    for(pos = 180; pos>=1; pos-=1)    
    {   
      hand_l_servo.write(180-pos);        
      hand_r_servo.write(pos);    
      delay(hand_delay);                      
    }
  }
  else  //left hand up step
  if(keyword=='U')
  {
    hand_l_pos -=10;  
    hand_l_pos = max(hand_l_pos,0);
    hand_l_servo.write(hand_l_pos);          
    delay(hand_delay);                       

  }
  //left hand down step
  else if(keyword=='J'){
    hand_l_pos +=10;      
    hand_l_pos = min(hand_l_pos,180);    
    hand_l_servo.write(hand_l_pos);         
    delay(hand_delay);                    

  }

  //right hand up step
  if(keyword=='I')
  {
    hand_r_pos +=10;     
    hand_r_pos = min(hand_r_pos,180);   
    hand_r_servo.write(hand_r_pos);             
    delay(hand_delay);                       
  }
  //right hand down step
  else if(keyword=='K'){
    hand_r_pos -=10;      
    hand_r_pos = max(hand_r_pos,0);    
    hand_r_servo.write(hand_r_pos);             
    delay(hand_delay);                    
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
  }
  else{ 
    // no action
  }
}



void servo_test(){
  Serial.print("Left Pos:");
  Serial.print(hand_l_pos);
  Serial.print(" ; Right Pos:");
  Serial.println(hand_r_pos);
  delay(1000);
}

void b_skill(int keyword){
  int pos;
  //rush
  if(keyword=='f'){
    //both hand down 
    for(pos = 90; pos>=1; pos-=1)    
    {   
      hand_l_servo.write(180-pos);        
      hand_r_servo.write(pos);    
      delay(hand_delay);                      
    }
    //go forward
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
    //both hand up  
    for(pos = 0; pos < 180; pos += 1) 
    {    
      hand_l_servo.write(180-pos);          
      hand_r_servo.write(pos);                                      
      delay(hand_delay);                       
    }
    delay(500); 
    b_motor_stop();
    delay(100); 
  }
}




