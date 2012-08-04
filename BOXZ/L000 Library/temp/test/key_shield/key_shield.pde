//This input shield use Digital Pin 3,4,5 (3 buttons) and Analog Pin 0,1 (JS) 
// // Upload the code to Arduino
// www.dfrobot.com
// Last modified on 24/12/2009

int x=1;
int y=0;
int button_A= 5;
int button_B= 3;
int button_C= 4;

void setup()
{
  Serial.begin(9600); 
  int i;
  for(i=3;i<=5;i++)
    pinMode(i, INPUT);

}

void loop()
{  
  int output_x;
  int output_y;
  int output_a;
  int output_b;
  int output_c;
  while(1)
  {
    output_x=analogRead(x); //Read Analog input 
    output_y=analogRead(y); 
    if(digitalRead(button_A)==0)    //Check Button A
    {
      output_a=1;
      output_b=0;
      output_c=0;
    }
    if(digitalRead(button_B)==0)   //Check Button B
    {
      output_a=0;
      output_b=1;
      output_c=0;
    }    
    if(digitalRead(button_C)==0)   //Check Button C
    {
      output_a=0;
      output_b=0;
      output_c=1;
    }
    Serial.print("x=");
    Serial.print(output_x);
    Serial.print("y=");
    Serial.print(output_y);
    Serial.print("a=");
    Serial.print(output_a);
    Serial.print("b=");
    Serial.print(output_b);
    Serial.print("c=");
    Serial.println(output_c);
    delay(500);
  }
}


