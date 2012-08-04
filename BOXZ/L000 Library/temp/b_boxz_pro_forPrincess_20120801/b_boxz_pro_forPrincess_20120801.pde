/*
BOXZ is is an open source robot platform for interactive entertainment! 
 We connected Arduino,Plexiglass(PMMA) Origami and idea together, 
 so we can easily and quickly build a remote robot by ourself! 
 We can use it to playing football game, racing, fighting or some other interesting things!
 
 Author: Leo Zhu
 Email: zgj0729@163.com
 
 Code license: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
 */

/*
Update
 2012.07.19
 1. del step action, HMI will send a space key after release key
 2. del skill(); now, we only need speed rush!
 
 2012.07.24
 1. add servo position start and end
 2. add eye motor control
 3. add function: b_pro_function(), b_key_function(), b_Ultrasonic_Ranger()
 
 2012.08.01
 1. for Princess add UR and LED
 */

//Ultrasonic Ranger
//return value_ranger is a float!
//I/O: A2,A3 is occupied for Ultrasonic Ranger
//I/O: A0,A1 is occupied for PWM LED
//function: the face of Princess Peach would turn red when you are close enough!
//PWM Resolution = 64

#include <AFMotor.h>
#include <Servo.h> 
#include <Wire.h>

/*
 b_motor_stop(): stop motor
 b_motor_com():  for Manual Operator
 b_servo_ini();
 b_servo_com();
 */

//-------------------define motor----------------------------------------------//
AF_DCMotor motorL(1,MOTOR12_8KHZ);  //connect to M1
AF_DCMotor motorR(2,MOTOR12_8KHZ);  //connect to M2
AF_DCMotor motorZ(3);  //connect to M3 for left eye Component
AF_DCMotor motorX(4);  //connect to M4 for right eye Component
int motor_speed = 200;  //[variable] motor speed 150-200,---min:100;max:255
int value_speed = 254;  //[variable] for skill

//-------------------define servo----------------------------------------------//
Servo hand_l_servo;  // create servo object to control a servo 
Servo hand_r_servo;  // create servo object to control a servo
int hand_l_pos = 90; // hand initial position
int hand_r_pos = 90; // hand initial position
int hand_pos_s = 5;  //[variable] hand start position, range area(0 - 180)
int hand_pos_e = 90; //[variable] hand end position, range area(0 - 180),bigger than start!
int hand_delay = 1;  //[variable] speed of hand

//---------
int value_health = 10;   //[variable] LED health bar for IIC
int value_mana = 10;     //[variable] LED mana bar for IIC
int value_piezo_l = 0;   //piezo sensor A0
int value_piezo_r = 0;   //piezo sensor A1
int value_ranger = 0;    //Ultrasonic Ranger Pin 16,17
int piezo_min = 120;      //[variable] piezo sensor value min



//------------------main program-----------------------------------------------//

void setup()
{
  Serial.begin(9600);
  b_motor_stop();
  b_servo_ini();
  Wire.begin(); // join i2c bus (address optional for master)
  delay(2000);  //waiting time
  Serial.println("Hello! BOXZ!");
}


void loop()
{
  int key;
  int key_func;
  int key_status; //key and status from sensor

  b_pro_function();
  float value_ranger;
  int fixed_ranger;
  value_ranger = b_Ultrasonic_Ranger();
  if(value_ranger < 24){
    fixed_ranger = b_Ranger_Function(value_ranger);
    b_LED_PWM_control(fixed_ranger);
  }
  if(Serial.available() > 0) {    
    key = Serial.read();  
    if(key >= 30 && key <= 122) { 
      if((key=='f')&&(value_mana>1)){
        value_speed = 255;  
        value_mana -= 1;
        if(value_mana<1) value_mana=0;
      }
      b_motor_com(key);  
      b_servo_com(key);
    }
  }
  // servo_test();
}

//---------------------------------motor-------------------------------//
//keyboard "w,a,s,d" for action;space for stop
//keyword "z,x" for eye control clockwise and anticlockwise

void b_motor_stop(){
  motorL.run(RELEASE);
  motorR.run(RELEASE);
  motorZ.run(RELEASE);
  motorX.run(RELEASE);
}

void b_motor_com(int keyword){
  //forward
  if(keyword=='w')
  {
    motorL.setSpeed(value_speed);
    motorR.setSpeed(value_speed);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
  }
  //backward
  else if(keyword=='s'){
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed);
    motorL.run(BACKWARD);
    motorR.run(BACKWARD);
    value_speed = 200;
  }
  //turn left
  else if(keyword=='a'){
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed);
    motorL.run(BACKWARD);
    motorR.run(FORWARD);
    value_speed = 200;
  }
  //turn right
  else if(keyword=='d'){
    motorL.setSpeed(motor_speed);
    motorR.setSpeed(motor_speed);
    motorL.run(FORWARD);
    motorR.run(BACKWARD);
    value_speed = 200;
  }
  else if(keyword=='z'){
    motorZ.setSpeed(motor_speed);
    motorX.setSpeed(motor_speed);
    motorZ.run(FORWARD);
    motorX.run(BACKWARD);
    value_speed = 0;
  }
  else if(keyword=='x'){
    motorZ.setSpeed(motor_speed);
    motorX.setSpeed(motor_speed);
    motorZ.run(BACKWARD);
    motorX.run(FORWARD);
    value_speed = 0;
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
    for(pos = hand_pos_s; pos < hand_pos_e; pos += 1)  
    {                                
      hand_l_servo.write(hand_pos_e-pos);          
      delay(hand_delay);                       
    } 
  }
  //left hand down
  else if(keyword=='j'){
    for(pos = hand_pos_e; pos>=hand_pos_s; pos-=1)    
    {                                
      hand_l_servo.write(hand_pos_e-pos);         
      delay(hand_delay);                    
    } 
  }

  //right hand up
  if(keyword=='i')
  {
    for(pos = hand_pos_s; pos < hand_pos_e; pos += 1) 
    {                                 
      hand_r_servo.write(pos);             
      delay(hand_delay);                       
    } 
  }
  //right hand down
  else if(keyword=='k'){
    for(pos = hand_pos_e; pos>=hand_pos_s; pos-=1)    
    {                                
      hand_r_servo.write(pos);             
      delay(hand_delay);                    
    } 
  }
  //both hand up
  else if(keyword=='o'){   
    for(pos = hand_pos_s; pos < hand_pos_e; pos += 1) 
    {    
      hand_l_servo.write(hand_pos_e-pos);          
      hand_r_servo.write(pos);                                      
      delay(hand_delay);                       
    }
  }
  //both hand down
  else if(keyword=='l'){

    for(pos = hand_pos_e; pos>=hand_pos_s; pos-=1)    
    {   
      hand_l_servo.write(hand_pos_e-pos);        
      hand_r_servo.write(pos);    
      delay(hand_delay);                      
    }
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


//----Ultrasonic Ranger-------//
float b_Ultrasonic_Ranger(){
  //maybe next time we need add some parameter, like cm/inches switch
  int initial;
  int TrigPin = 16; 
  int EchoPin = 17; 
  float cm; 

  if(initial == 0){
    pinMode(TrigPin, OUTPUT); 
    pinMode(EchoPin, INPUT); 
    initial = 1;
  }

  if (initial == 1){
    digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin 
    delayMicroseconds(2); 
    digitalWrite(TrigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(TrigPin, LOW); 

    cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm 
    cm = (int(cm * 100.0)) / 100.0; //保留两位小数 
    return cm;
  }
}

//-----------//
void b_pro_function(){
  //initial

  //input events
  //1. light sensor: speed up

  //2. piezo sensor
  value_piezo_l = analogRead(0);
  value_piezo_r = analogRead(1);
  if(value_piezo_l > piezo_min)
  {
    value_health-=1;
    Serial.println("left");
    delay(100);
  }
  if(value_piezo_r > piezo_min)
  {
    value_health-=1;
    Serial.println("right");
    delay(200);
  }

  //3. ranger control
  //  if (b_Ultrasonic_Ranger()<3){
  //    value_speed = 120;
  //  }
  //  else if(b_Ultrasonic_Ranger()<1){
  //    value_speed = 0;
  //    value_health-=1;
  //    b_motor_stop(); //motor stop
  //    return keyf = ' ';  //stop
  //  } 

  //value fix
  if(value_health<1) value_health=0;
  if(value_mana<1) value_mana=0;
  if(value_health>9) value_health=10;
  if(value_mana>9) value_mana=10;

  //output control
  //1. health control
  if (value_health<2){
    value_speed = 0;
    b_motor_stop();
  }


  //5.IIC send out LED bar  
  Wire.beginTransmission(3); // transmit to device #3
  Wire.send(value_health);    // sends one byte  
  Wire.endTransmission();    // stop transmitting
  Wire.beginTransmission(4); // transmit to device #4
  Wire.send(value_mana);    // sends one byte  
  Wire.endTransmission();    // stop transmitting
}



void b_LED_PWM_control(int bright){

  int initial;
  int led_l = 14; 
  int led_r = 15; 


  if(initial == 0){
    pinMode(led_l, OUTPUT); 
    pinMode(led_r, OUTPUT); 
    initial = 1;
  }

  if (initial == 1){
    //PWM Resolution = 64
    for(int i = 0; i < 64; i++)
    {
      if(i < bright)
      {
        digitalWrite(led_l, HIGH);
        digitalWrite(led_r, HIGH);
        delayMicroseconds(30);
      }
      else
      {
        digitalWrite(led_l, LOW);
        digitalWrite(led_r, LOW);
        delayMicroseconds(30);
      }
    }
  }
}



int b_Ranger_Function(float ranger){
  float temp;
  int range;
  if(ranger>20){
    temp = 0;
  }
  else if(ranger>2 && ranger <=20){
    temp = (20- int(ranger))*4;
  }
  else if(ranger<=2){
    temp = (2- int(ranger))*91+72;
  }
  else{
    temp = 0;
  }
  range= int(temp);
  return range;
}





