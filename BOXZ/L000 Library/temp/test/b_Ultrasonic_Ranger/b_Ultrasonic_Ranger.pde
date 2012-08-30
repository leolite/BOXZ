//Ultrasonic Ranger
//return value_ranger is a float!
//I/O: A2,A3 is occupied


void setup() 
{ 
  Serial.begin(9600); 
} 


void loop() 
{ 
  float value_ranger;
  value_ranger = b_Ultrasonic_Ranger();
  Serial.print(value_ranger); 
  Serial.print("cm"); 
  Serial.println(); 
  delay(1000); 
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

