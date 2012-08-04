//thanks to @Ansifa
//pin from 2-17
//ledLevel == 0 is breathing lamp
//ledLevel == 1~254?? 64??
//PWMResolution = 64;??
#include <Wire.h>

int brights[16] = {
  0};  //定义16个引脚的初始亮度，可以随意设置
int StartPWMPin = 2, EndPWMPin = 17; //设置D2~D17为PWM引脚
int PWMResolution = 255; //设置PWM占空比分辨率

void setup()
{
  //定义所有IO端输出
  for(int i = StartPWMPin; i <= EndPWMPin; i++)
  {
    pinMode(i, OUTPUT);
    //随便定义个初始亮度,便于观察
    brights[i] = 255;
  }
  delay(1000);
  test();
  Wire.begin(5);                // join i2c bus with address #5
  Wire.onReceive(receiveEvent); // register event

}


void loop() {
   test();
  //  delay(100);
}


void receiveEvent(int howMany)
{
  int ledLevel;
  if(Wire.available() > 0) 
  {    

    ledLevel =  Wire.receive();

    //for read value
    for(int i = StartPWMPin; i <= EndPWMPin; i++)
    {
      if((brights[i]++) == PWMResolution) brights[i] = ledLevel;
    }

    for(int i = 0; i <= PWMResolution; i++)  //i是计数一个PWM周期
    {
      for(int j = StartPWMPin; j <= EndPWMPin; j++) //每个PWM周期均遍历所有引脚
      {
        if(i < brights[j])
        {
          digitalWrite(j, HIGH);
          delayMicroseconds(2);
        }
        else
        {
          digitalWrite(j, LOW);
          delayMicroseconds(2);
        }
      }
    }
  }
}


void test(){
  for(int i = StartPWMPin; i <= EndPWMPin; i++)
  {
    if((brights[i]++) == PWMResolution) brights[i] = 0;
  }


  for(int i = 0; i <= PWMResolution; i++)  //i是计数一个PWM周期
  {
    for(int j = StartPWMPin; j <= EndPWMPin; j++) //每个PWM周期均遍历所有引脚
    {
      if(i < brights[j])
      {
        digitalWrite(j, HIGH);
        delayMicroseconds(2);
      }
      else
      {
        digitalWrite(j, LOW);
        delayMicroseconds(2);
      }
    }
  }
}





