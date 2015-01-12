//Microduino drive board

//=========M.A=============//
/*
int mic_left_A = 7;
 int mic_right_A = 5;
 int mic_head_A = 8;
 int mic_back_A = 6;
 */
//=========M.B=============//

int mic_left_B = 22;
int mic_right_B =18;
int mic_head_B = 23;
int mic_back_B = 19;

void initDrive() {
  pinMode(mic_left_B,OUTPUT);
  pinMode(mic_right_B,OUTPUT);
  pinMode(mic_head_B,OUTPUT);
  pinMode(mic_back_B,OUTPUT);
  //stop
  digitalWrite(mic_left_B,LOW);
  digitalWrite(mic_right_B,LOW);
  digitalWrite(mic_head_B,LOW);
  digitalWrite(mic_back_B,LOW);
}


void goForward()
{
  digitalWrite(mic_left_B,LOW);
  digitalWrite(mic_right_B,HIGH);
  digitalWrite(mic_head_B,HIGH);
  digitalWrite(mic_back_B,LOW);
}

void goLeft()
{
  digitalWrite(mic_left_B,HIGH);
  digitalWrite(mic_right_B,LOW);
  digitalWrite(mic_head_B,HIGH);
  digitalWrite(mic_back_B,LOW);
}

void goRight()
{
  digitalWrite(mic_left_B,LOW);
  digitalWrite(mic_right_B,HIGH);
  digitalWrite(mic_head_B,LOW);
  digitalWrite(mic_back_B,HIGH);
}

void goBackward()
{
  digitalWrite(mic_left_B,HIGH);
  digitalWrite(mic_right_B,LOW);
  digitalWrite(mic_head_B,LOW);
  digitalWrite(mic_back_B,HIGH);
}

void stop()
{
  digitalWrite(mic_left_B,LOW);
  digitalWrite(mic_right_B,LOW);
  digitalWrite(mic_head_B,LOW);
  digitalWrite(mic_back_B,LOW);
}

//BOXZ base keyword mode function
void motorCom(int keyword)
{
  if(keyword == 'w') goForward();
  if(keyword == 's') goBackward();
  if(keyword == 'a') goLeft();
  if(keyword == 'd') goRight();
  if(keyword == 'q') goForward(); 
  if(keyword == 'e') goForward(); 
  if(keyword == 'z') goBackward();
  if(keyword == 'x') goBackward();
  if(keyword == ' ') stop();
}

