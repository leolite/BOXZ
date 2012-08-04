//Ultrasonic Ranger
//return value_ranger is a float!
//I/O: A2,A3 is occupied for Ultrasonic Ranger
//I/O: A0,A1 is occupied for PWM LED
//function: the face of Princess Peach would turn red when you are close enough!
//PWM Resolution = 64

void setup() 
{ 
  Serial.begin(9600); 
} 


void loop() 
{ 
  float value_ranger;
  int fixed_ranger;
  value_ranger = b_Ultrasonic_Ranger();
  if(value_ranger < 24){
    fixed_ranger = b_Ranger_Function(value_ranger);
    b_LED_PWM_control(fixed_ranger);
  }
  else{
    delay(100);
  }
} 


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




