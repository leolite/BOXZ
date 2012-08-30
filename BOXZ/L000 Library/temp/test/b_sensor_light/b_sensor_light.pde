/*
D2 for light sensor!
 */

void setup() {                
  pinMode(2, INPUT);  
  pinMode(13, OUTPUT);     
}

void loop() {
  if(digitalRead(2)== 1){
    digitalWrite(13, HIGH);   // set the LED on
  }
  else if(digitalRead(2)== 0){
    digitalWrite(13, LOW);    // set the LED off
  }
}

