//Input(key to char) and Output
//Output data: {"propty":{"HP":92,"MP":42}} - Output from Arduino to show current status
//Input data1: {"config":{"HP":100, "MP":100}} - send from IOS to config HP and MP
//Input data2: {"config":{"HP":100, "MP":200}}{"action":{"K1":"w", "K2":"a"}}
//Update 2013.11.09
//1. add filter by Leo(说明：当APP只发送K1而没发送K2时，K2会输出无效数值，故增加过滤器)
//Update 2013.11.11
//1. add watch Dog  by Leo(说明：简单的串口中断计数，溢出则清空，可返回重发请求)
#include <aJSON.h>
aJsonStream serial_stream(&Serial);
unsigned long last_print = 0;

int valueHP=100;
int valueMP=100;
char key1;
char key2;

unsigned long last_input = 0;
boolean watchDogEn;
int watchDogLen = 20;
int watchDogCount = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println(F("{\"status\":{\"IF\":\"Hello! AJSON!\"}}"));
}

void loop()
{
  if (millis() - last_print > 2000) {
    /* One second elapsed, send message. */
    aJsonObject *msg = createMessage();
    aJson.print(msg, &serial_stream);
    Serial.println(); /* Add newline. */
    aJson.deleteItem(msg);
    last_print = millis();
    Serial.println(key1);
    Serial.println(key2);
  }
}

void serialEvent() {
  watchDogCount ++;
  //Serial.println(watchDogCount);
  if(watchDogCount >watchDogLen){
     Serial.println(F("{\"status\":{\"OT\":\1\"}}")); //Watch Dog Enable!
    watchDogEn = true;
  }

  if(watchDogEn == true){
    while(Serial.read() >= 0){
    }
    serial_stream.flush();
    watchDogCount = 0;
    watchDogEn = false;
  }

  serial_stream.skip(); // Skip any whitespace
  if (serial_stream.available()) {
    /* Something real on input, let's take a look. */
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}

void ComExecution(aJsonObject *msg)
{
  aJsonObject *config = aJson.getObjectItem(msg, "config"); //property
  if (config) { 
    aJsonObject *config_HP = aJson.getObjectItem(config, "HP"); //HP
    aJsonObject *config_MP = aJson.getObjectItem(config, "MP");//MP

    //Serial.println(config_HP->valueint);//HP为整型值
    //Serial.println(propty->valuestring);//字符串
    //Serial.println(config_MP->valueint);

    int valueHPBuf = config_HP->valueint;
    int valueMPBuf = config_MP->valueint;
    if(valueHPBuf>=0&&valueHPBuf<=100) valueHP=valueHPBuf;
    if(valueMPBuf>=0&&valueMPBuf<=100) valueMP=valueMPBuf; 
  }

  aJsonObject *action = aJson.getObjectItem(msg, "action");
  if(action) { 
    aJsonObject *action_K1 = aJson.getObjectItem(action, "K1"); //key1: Direction control
    aJsonObject *action_K2 = aJson.getObjectItem(action, "K2"); //key2: button and skill
    //Serial.println(action_K1->valuestring);
    //Serial.println(action_K2->valuestring);
    String key1s = action_K1->valuestring;
    String key2s = action_K2->valuestring;
    int key1Buf = key1s.charAt(0);
    int key2Buf = key2s.charAt(0);
    if(key1Buf>=0&&key1Buf<=255) key1=key1Buf;
    if(key2Buf>=0&&key2Buf<=255) key2=key2Buf; 
  }  
}

/* Generate message like: {"propty":{"HP":100, "MP":200}} */
aJsonObject *createMessage()
{
  aJsonObject *msg,*propty;
  msg = aJson.createObject(); 

  int analogValues[2];
  for (int i = 0; i < 2; i++) {
    analogValues[i] = random(0, 5);
  }
  valueHP = valueHP - analogValues[0];
  valueMP = valueMP - analogValues[1];
  aJson.addItemToObject(msg, "propty", propty = aJson.createObject());
  aJson.addNumberToObject(propty ,"HP", valueHP);
  aJson.addNumberToObject(propty ,"MP",valueMP);

  return msg;
}


int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}









