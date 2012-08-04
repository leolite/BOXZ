int value_piezo_l = 0;   //piezo sensor A0
int value_piezo_r = 0;   //piezo sensor A1
int value_ranger = 0;    //Ultrasonic Ranger Pin 16,17
int piezo_min = 100;      //[variable] piezo sensor value min


void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  delay(2000);  //waiting time
  Serial.println("Hello! BOXZ! piezo test starting!");
}

void loop()
{
  //2. piezo sensor
  value_piezo_l = analogRead(0);
  value_piezo_r = analogRead(1);


  if(value_piezo_l > piezo_min) {    
    Serial.print("left:");
    Serial.print(value_piezo_l);
    Serial.print("; MIN:");
    Serial.println(piezo_min);
  }

  if(value_piezo_r > piezo_min) {    
    Serial.print("right:");
    Serial.print(value_piezo_r);
    Serial.print("; MIN:");
    Serial.println(piezo_min);
  }

  if(value_piezo_l > piezo_min) digitalWrite(13, HIGH); 
  if(value_piezo_r > piezo_min) digitalWrite(13, LOW); 

}



