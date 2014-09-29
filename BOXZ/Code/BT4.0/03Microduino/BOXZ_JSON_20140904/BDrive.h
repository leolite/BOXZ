
//int inA =4;
//int inB =7;
//int SPEEDA=5;
//int SPEEDB=6;
/******************************* initialization function ************************************************/

void stop()
{
  /*disable the enble pin, to stop the motor. */
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  //Serial.println("Stop....");
}

//initialization
//init for 4 Pin Motor Driver
//DFRobot 4,7,5,6
void initDrive()
{
  pinMode(4,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  stop();
}


/****************************direction of motion control function*********************************/
//Control BOXZ go forward
void goForward(int speedA, int speedB)
{

  digitalWrite(4,HIGH);
  digitalWrite(7,HIGH);
  analogWrite(5,speedA);
  analogWrite(6,speedB);

}

//Control BOXZ go backward
void goBackward(int speedA, int speedB)
{

  digitalWrite(4,LOW);
  digitalWrite(7,LOW);
  analogWrite(5,speedA);
  analogWrite(6,speedB);

}

//Control BOXZ turn left
void goLeft(int speedA, int speedB)
{

  digitalWrite(4,HIGH);
  digitalWrite(7,LOW);
  analogWrite(5,speedA);
  analogWrite(6,speedB);

}

//Control BOXZ turn right
void goRight(int speedA, int speedB)
{

  digitalWrite(4,LOW);
  digitalWrite(7,HIGH);
  analogWrite(5,speedA);
  analogWrite(6,speedB);

}


/****************************keyword communication function*********************************/
//BOXZ base keyword mode function
void motorCom(int keyword)
{
  if(keyword == 'w') goForward(0xFF,0xFF);
  if(keyword == 's') goBackward(0xFF,0xFF);
  if(keyword == 'a') goLeft(0xFF,0xEE);
  if(keyword == 'd') goRight(0xEE,0xFF);
  if(keyword == 'q') goForward(0xEE,0xFF); 
  if(keyword == 'e') goForward(0xFF,0xEE); 
  if(keyword == 'z') goBackward(0xEE,0xFF);
  if(keyword == 'x') goBackward(0xFF,0xEE);
  if(keyword == ' ') stop();
}




