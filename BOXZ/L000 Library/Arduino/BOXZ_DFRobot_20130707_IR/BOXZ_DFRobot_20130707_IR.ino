/*------------------------------------------------------------------
 BOXZ for Seeeduino and OJ 2A L298 Shield
 http://www.seeedstudio.com/
 http://www.openjumper.com/
 
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
 
 2013.03.11
 1. add IR control
 
 2013.03.16
 1. delete flash block
 2.support Arduino 1.0+
 
 2013.03.17
 1. delete line finder
 
 2013.03.18
 1. IRremote for RC06(from www.yaocoo.com) 
 
 2013.03.19
 1. Add Buzzer and LED
 2. IR lib add shift bit
 
 2013.07.07
 1. IR for DFRobot tested
 ------------------------------------------------------------------*/

#include <Servo.h> 
#include <IRremote.h>
#include "IR_Receive.h"
#include "IR_Value.h"

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
int motor_speed_fl = 0;     //motor speed fixed left side
int motor_speed_fr = 0;     //motor speed fixed right side

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
 D3  IR LED
 D11 IR Receiver
 D8  Line finder L
 D12 Line finder R
 D13 IR Status
 
 Spare pin
 D2
 A0  piezo sensor 1
 A1  piezo sensor 2
 A2  Ultrasonic_Ranger Echo
 A3  Ultrasonic_Ranger Trig
 A4  I2C SCL
 A5  I2C SDA
 ------------------------------------------------------------------*/
int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;
int index = 0; //for array
int mapto = 0; //fir mapping
int key = 0;

int buzzerPin = 11; //for Buzzer
int ledPin = 8;  //LED for car go back

long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis = 0;
long interval = 500;           // interval at which to blink (milliseconds)

/*------------------------------------------------------------------
 main program
 ------------------------------------------------------------------*/
int testz = 0;

void setup()
{
  motorInit();
  b_servo_ini();
  b_ini_function();
}


void loop()
{
  irrecv.blink13(1);
  if (irrecv.decode(&results)) {
    //search from IR_Reveive.h and return index
    if(results.value != ir_code_r[0]){
      for(int i=0;i<256;i++){
        if( results.value== ir_code_r[i]){
          index = i;
          break;
        } 
        //watchdog index = stop
        if(i>22){
          index = 0;
          break;
        } 
      }
      key = ir_code_v[index];  
      //      Serial.println(results.value, HEX);
      //      Serial.println(index);
      //     Serial.println(key);
    }
    b_motor_com(key); 
    b_servo_com(key);
    b_car_back(key); 
    irrecv.resume(); // Receive the next value
  }
  b_car_base();
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
    motorLrun(FORWARD,motor_speed-motor_speed_fl);
    motorRrun(FORWARD,motor_speed-motor_speed_fr);
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
  b_motor_stop();
  irrecv.enableIRIn(); // Start the receiver
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);  
  pinMode(ledPin, OUTPUT);  
  Serial.println("Hello! BOXZ!");

}

void b_car_back(int keyword){
  if(keyword == 's'){
    digitalWrite(ledPin,HIGH); 
    digitalWrite(buzzerPin,HIGH);
  }
}

//put this in the loop()
void b_car_base(){
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    digitalWrite(ledPin, LOW);  
    digitalWrite(buzzerPin,LOW);   
  }
  if(key != 's'){
    digitalWrite(ledPin, LOW);  
    digitalWrite(buzzerPin,LOW);  
  }
}






