/*------------------------------------------------------------------
 BOXZ for DFRobot 2A L298 Shield
 
 2012.06.30
 add Servo by Leo
 
 2012.07.13
 add drive by i3water
 
 2012.10.15
 drive update for DFRobot
 
 2012.10.18
 1.drive update(key add q,e)
 2.add function
 3.add flash support
 
 2012.10.23
 1. turn speed lower
 2. exchange 'a','d' with 'q','e'
 3. Dead light on change to light off!
 4. Confuse with int piezo_min = 100;  if the value is OK!!!!!!!!!
 ------------------------------------------------------------------*/

#include <Servo.h> 

/*------------------------------------------------------------------
 define motor
 for DFRobot 2A L298 Shield
 D4  M1 Control
 D5  M1 PWM
 D6  M2 PWM
 D7  M2 Control
 
 ------------------------------------------------------------------*/
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4
int motor_speed = 255; //[modifid]motor speed 150-200,---min:100;max:255
int motor_delay = 400; //[modifid]delay time in step


/*------------------------------------------------------------------
 define servo
 D9  Left hand(servo)
 D10 Right hand(servo)
 ------------------------------------------------------------------*/
Servo hand_l_servo;  // create servo object to control a servo 
Servo hand_r_servo;  // create servo object to control a servo
int hand_l_pos = 90; // 
int hand_r_pos = 90; //
int hand_delay = 1;  //[modifid] speed of hand


/*------------------------------------------------------------------
 function define
 D3  PWM LED 1
 D11 PWM LED 2
 A0  piezo sensor 1
 A1  piezo sensor 1
 
 Spare pin
 D2
 D8
 D12
 D13 BOXZ Status
 A2  Ultrasonic_Ranger Echo
 A3  Ultrasonic_Ranger Trig
 A4  I2C SCL
 A5  I2C SDA
 ------------------------------------------------------------------*/
int led1Pin = 3;        //PWM LED connected to digital pin 3
int led2Pin = 11;       //PWM LED connected to digital pin 11
int piezo1Pin = 0;      //piezo sensor A0
int value_health = 10;   //[variable] LED health bar for IIC
int value_mana = 10;     //[variable] LED mana bar for IIC
int value_piezo_l = 0;   //piezo sensor A0
int value_ranger = 0;    //Ultrasonic Ranger Pin 16,17
int piezo_min = 100;      //[variable] piezo sensor value min

/*------------------------------------------------------------------
 main program
 ------------------------------------------------------------------*/
//----------------------------------------------------------
/*send to flash
 * inspired by D.Mellis printString function. it simply adds the byte(0) at the end 
 * needed by the XMLSocket to know that the transmission is over.
 * read the XMLSocket Actionscript Class documentation for more details
 * edited by Leo for Int(2012.10.18)
 */
void sendIntToFlash (int s) {
  Serial.print(s, BYTE); 
  Serial.print(0, BYTE); //notify the XMLSocket that the comunication is over
}



//----------------------------------------------------------


void setup()
{
  Serial.begin(9600);
  motorInit();
  b_motor_stop();
  b_servo_ini();
  b_ini_function();
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
  b_pro_function();
  // servo_test();
}


/*------------------------------------------------------------------
 motorInit();//initial motorL&motorR;
 motorLrun(cmd,spd);//cmd for BOXZ's action,spd for motorL speed;
 motorRrun(cmd,spd);//cmd for BOXZ's action,spd for motorR speed;
 b_motor_stop();//stop motorL&motorR;
 b_motor_com(keyword);//keyboard "w,a,s,d" for action;space for stop;
 ------------------------------------------------------------------*/
void motorInit(){
  pinMode(4,OUTPUT);//Control DC motor L
  pinMode(5,OUTPUT);//PWM to control speed of DC motor L
  pinMode(7,OUTPUT);//Control DC motor R
  pinMode(6,OUTPUT);//PWM to control speed of DC motor R
}

void motorLrun(int cmd,int spd){  //cmd for FORWARD BACKWARD BRAKE(stop);
  switch (cmd){                    //spd for control PWM and motor speed;
  case FORWARD:
    digitalWrite(4,HIGH);
    analogWrite(5,spd);
    break;
  case BACKWARD:
    digitalWrite(4,LOW);
    analogWrite(5,spd);
    break;
  case BRAKE:
    digitalWrite(4,LOW);
    analogWrite(5,spd);
    break;
  default :
    return;
  }
}

void motorRrun(int cmd,int spd){
  switch (cmd){
  case FORWARD:
    digitalWrite(7,HIGH);
    analogWrite(6,spd);
    break;
  case BACKWARD:
    digitalWrite(7,LOW);
    analogWrite(6,spd);
    break;
  case BRAKE:
    digitalWrite(7,LOW);
    analogWrite(6,spd);
  default :
    return;
  }
}

void b_motor_stop(){
  motorLrun(BRAKE,0);
  motorRrun(BRAKE,0);
}

void b_motor_com(int keyword){
  switch (keyword){
  case 'w':
    motorLrun(FORWARD,motor_speed);
    motorRrun(FORWARD,motor_speed);
    //  Serial.println("FORWARD");
    break;
  case 's':
    motorLrun(BACKWARD,motor_speed);
    motorRrun(BACKWARD,motor_speed);
    //  Serial.println("BACKWARD");
    break;
  case 'q':
    motorLrun(FORWARD,motor_speed);
    motorRrun(FORWARD,motor_speed-20);
    // Serial.println("LEFT");
    break;
  case 'e':
    motorLrun(FORWARD,motor_speed-20);
    motorRrun(FORWARD,motor_speed);
    // Serial.println("RIGHT");
    break;
  case 'a':
    motorLrun(FORWARD,motor_speed-50);
    motorRrun(BACKWARD,motor_speed-50);
    // Serial.println("LEFT");
    break;
  case 'd':
    motorLrun(BACKWARD,motor_speed-50);
    motorRrun(FORWARD,motor_speed-50);
    //  Serial.println("RIGHT");
    break;
  case ' ':
    //  Serial.println("STOP");
    b_motor_stop();
    break;
  default :
    //  Serial.println("STOP");
    b_motor_stop();
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


//------------------function-------------
void b_ini_function(){
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
}

void b_pro_function(){
  //initial
  int m; //temp value for HP
  //input events
  //1. light sensor: speed up

  //2. piezo sensor
  value_piezo_l = analogRead(piezo1Pin);
  if(value_piezo_l > piezo_min)
  {
    if(value_health>1){
      value_health-=1;
      b_value_fixed(); 
      m=33+value_health;
      sendIntToFlash(m);
      //Serial.println(value_health);
      analogWrite(led1Pin, 255);
      analogWrite(led2Pin, 255);
      delay(100);
      for(int n=255;n>=0;n--){
        analogWrite(led1Pin, n);
        analogWrite(led2Pin, n);
        delay(2);
      }
    }
    else{
      analogWrite(led1Pin, 0);
      analogWrite(led2Pin, 0);
    } 

  }
}

void b_value_fixed(){
  //value fix
  if(value_health<0) value_health=0;
  if(value_mana<0) value_mana=0;
  if(value_health>10) value_health=10;
  if(value_mana>10) value_mana=10;

}














