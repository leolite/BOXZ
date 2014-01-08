#include <EEPROM.h>
#include <aJSON.h>
#include "boxzRomeo.h"

//2013.12.01
//tested with Limit switch!
//Test ok without BOXZ lib
//Fixed watchDog for ROMEO with Leonardo, not support serialEvent.
aJsonStream serial_stream(&Serial1);

//int driveType = 0xDF;
int serialSpeed = 9600; //9600 for BT2.0 and 115200 for BLE 4.0
int serial1Speed = 115200; //serial1 for ROMEO

int testmode =0; // 0: Disable; 1: status; 2: freeRAM; 3:Offline Test(not ready)
static unsigned long cycleTimer = 0;
static unsigned long cycleLimit = 2000;
static unsigned long watchDogTimer = 0;
static unsigned long watchDogLimit = 2000;
//unsigned long timerLed = 0;
//unsigned long timerBuz = 0;

//hardware define
int pinLed[]={
  A0,A1,A2,A3}; //pin 14 = FL,15 = FR,16 = BL,17 = BR
int pinKey[]={
  8,9,10,11,12,13};
int pinBuz = 3;
int rangeLed = 4; //actived range of LED
int rangeKey = 2; //actived range of Limit switch

//LED
boolean statusLed1 = false;
boolean statusLed2 = false;
boolean statusLed3 = false;
int statusLedBuf;
int statusLedBufold;
int statusLed[]={
  0x03,0x0C,0x05,0x0A,0x05,0x0A,0x05,0x0A,0x00,0xFF}; //0 to 7 for key1 8 is off, 9 is on
int defineLedKey[]={
  'w','s','a','d','q','e','z','x',' ','u','i','o','j','k','l'};//only for LED and Buz(0 to 7 for Key1,8 is ' ' and 9 to 14 for Key2)
int rangeLedKey = 9;

//Buzzer
int tones[] = {
  261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//mid C  C#   D    D#   E    F    F#   G    G#   A
int rangeTones = 10;
boolean statusBuz1 = false;
boolean statusBuz2 = false;

//impact checking
boolean defineKeyON = LOW;
boolean dodgemCrashed = false; 
boolean dodgemCrashedOld; 
int statusKeyBuf;
int statusKeyBufOld;

//value Data
int valueHP;
int valueMP;
int valueHPratio = 6;
char valuekey1;  //user-define data from APP
char valuekey2;  //user-define data from APP
int valuePara1; //user-define data from APP
int valuePara2; //user-define data from APP
int valueLimit = 255;

boolean watchDogEn;
int watchDogLen = 2000;
int watchDogCount = 0;

//address of EEPROM 
int addressID = 64;//Config enable
int boxzIDValue = 1264;
int boxzIDReq = 1;
boolean boxzIDChecked = false;

void setup()
{

  Serial.begin(9600);
  Serial1.begin(115200);//to BLE 115200
  initAJSON();
  initMotorDF();
  initDodgem();
  initLed();
  initBuz();
  Serial1.println(F("{\"status\":{\"IF\":\"Hello! BOXZ!\"}}"));
}


void loop()
{
	if(valueHP<=1) valueHP = 1;
  serialData();
  commAJSON();
  commDodgem(); 
  motorCom(valuekey1);

  commLed(valuekey1,valuekey2,valuePara2);
  commBuz(valuekey2,valuePara2); 
}

void serialData() {
  if(watchDogCount >watchDogLen){
    Serial1.println(F("{\"status\":{\"OT\":1}}")); //Watch Dog Enable!
    watchDogEn = true;
  }

  if(watchDogEn == true){
    while(Serial1.read() >= 0){
    }
    serial_stream.flush();
    watchDogCount = 0;
    watchDogEn = false;
  }

  if (serial_stream.available()) {
    /* First, skip any accidental whitespace like newlines. */
    serial_stream.skip();
  }

  if (serial_stream.available()) {
    /* Something real on input, let's take a look. */
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
    watchDogCount ++;
  }
}


void commAJSON(){
  if ((millis() >= cycleTimer + cycleLimit)||(dodgemCrashedOld!=dodgemCrashed)) {
    cycleTimer = millis();
    aJsonObject *msg = createMessage();
    aJson.print(msg, &serial_stream);
    Serial1.println(); /* Add newline. */
    aJson.deleteItem(msg);
    dodgemCrashedOld = dodgemCrashed;
  }
}



void initAJSON(){
  //default value
  //futher function get data from APP
  valueHP = 100;
  valueMP = 100;
  valuePara1 = 0xFF;
  valuePara2 = 0xAA;
  valuekey1 = ' ';
  valuekey2 = ' ';
}



//initialization of LED
void initLed(){
  for(int i = 0; i< rangeLed ; i++){
    pinMode(pinLed[i], OUTPUT);  
  }
  //if(testmode == 1) Serial.println(F("LED Init Done!"));
}

void initBuz(){
  pinMode(pinBuz, OUTPUT);  
  for (int i = 0; i < rangeTones; i++)
  {
    tone(pinBuz, tones[i]);
    delay(200);
  }
  noTone(pinBuz);
  //if(testmode == 1) Serial.println(F("Buz Init Done!"));
}

void initDodgem(){
  for(int i = 0; i< rangeKey ; i++){
    pinMode(pinKey[i], INPUT);  
  }
  //if(testmode == 1) Serial.println(F("Key Init Done!"));
}

/* Generate message like: {"propty":{"HP":100, "MP":100}} */
aJsonObject *createMessage()
{
  aJsonObject *msg,*propty;
  msg = aJson.createObject(); 
  aJson.addItemToObject(msg, "propty", propty = aJson.createObject());
  aJson.addNumberToObject(propty ,"HP", valueHP);
  aJson.addNumberToObject(propty ,"MP",valueMP);
  if(boxzIDChecked == true) {
    aJson.addNumberToObject(propty ,"ID",boxzIDValue);
    boxzIDChecked = false; 
  }
  return msg;
}

void ComExecution(aJsonObject *msg)
{
  aJsonObject *config = aJson.getObjectItem(msg, "config"); //property
  if (config) { 
    aJsonObject *config_HP = aJson.getObjectItem(config, "HP"); //HP
    aJsonObject *config_MP = aJson.getObjectItem(config, "MP");//MP
    aJsonObject *config_ID = aJson.getObjectItem(config, "ID");//Not public ver! ID checking

      //Serial.println(config_HP->valueint);//HP为整型值
    //Serial.println(propty->valuestring);//字符串

    int valueHPBuf = config_HP->valueint;
    int valueMPBuf = config_MP->valueint;
    int boxzIDBuf = config_ID->valueint;
    if(valueHPBuf>=0&&valueHPBuf<=valueLimit) valueHP=valueHPBuf;
    if(valueMPBuf>=0&&valueMPBuf<=valueLimit) valueMP=valueMPBuf; 
    if(boxzIDBuf == boxzIDReq){ 
      EEPROM.write(addressID, boxzIDValue);
      boxzIDChecked = true; 
    }
    else{
      boxzIDBuf = 178; //0xB2
    }
  }

  aJsonObject *action = aJson.getObjectItem(msg, "action");
  if(action) { 
    aJsonObject *action_K1 = aJson.getObjectItem(action, "K1"); //key1: Direction control
    aJsonObject *action_K2 = aJson.getObjectItem(action, "K2"); //key2: button and skill
    aJsonObject *action_P1 = aJson.getObjectItem(action, "P1"); //parameter1: Direction control(futher function)
    aJsonObject *action_P2 = aJson.getObjectItem(action, "P2"); //parameter2: button and skill

    String key1s = action_K1->valuestring;
    String key2s = action_K2->valuestring;
    int para1Buf = action_P1->valueint;
    int para2Buf = action_P2->valueint;
    int key1Buf = key1s.charAt(0);
    int key2Buf = key2s.charAt(0);
    if(key1Buf>=0&&key1Buf<=valueLimit) valuekey1=key1Buf;
    if(key2Buf>=0&&key2Buf<=valueLimit) valuekey2=key2Buf; 
    if(para1Buf>=0&&para1Buf<=valueLimit) valuePara1=para1Buf;  //0xFF is max value
    if(para2Buf>=0&&para2Buf<=valueLimit) valuePara2=para2Buf;  //0xFF is max value, buzzer need mapping 
  }  
}


//Led control
//Updated 2013.11.07 11:00
//tested OK!! data and timer control from APP
void commLed(int keyA, int keyB, int Buffer){
  //level 0 Crashed status
  if(dodgemCrashed == false){
    statusLedBuf = 0; 
  }

  //level 2 Direction status
  for(int i = 0; i < rangeLedKey ; i++){
    if(i >= rangeLedKey -1){
      if(keyA != defineLedKey[i]){
        statusLed3 = false;
        break;
      }
    }
    if(keyA == defineLedKey[i]){
      statusLedBuf = statusLed[i];
      statusLed3 = true;
      break;
    }
  }

  if(dodgemCrashed == true){
    statusLedBuf = 0xFF; 
  }

  //level 1 Key input control
  //Button release
  if(keyB == defineLedKey[8]){
    statusLed1 = false;
    statusLed2 = false;
  }

  //Button A light on and off control  
  if(keyB == defineLedKey[9]){
    statusLed1 = true;
    statusLed2 = false;
  }

  //Button B light flash control
  if(keyB == defineLedKey[10]){
    statusLed1 = false;
    statusLed2 = true;
  }

  //Status -> Buffer
  if(statusLed1 == true){
    statusLedBuf = Buffer; 
  }
  if(statusLed2 == true){
    statusLedBuf = Buffer;
  }




  //test function
  if(testmode == 1){
    if(statusLedBufold != statusLedBuf){
      Serial.print("statusLedBuf: ");
      Serial.println(statusLedBuf);
      statusLedBufold = statusLedBuf;
    }
  }

  //Output
  for(int i=0; i<rangeLed ; i++){
    digitalWrite(pinLed[i], bitRead(statusLedBuf,i));
  }
}


//Buzzer function
//defineLedKey[10] = j; [11] = k
//Updated 2013.11.07 11:00
//tested OK! data and timer control from APP
void commBuz(int keyB, int Buffer){
  unsigned int Data = map(Buffer,0,255,0,5000);
  //Button release
  if(keyB == defineLedKey[8]){
    statusBuz1 = false;
    statusBuz2 = false;
  }

  //checking key input
  if(keyB == defineLedKey[12]){
    statusBuz1 = true;
    statusBuz2 = false;
  }

  if(keyB == defineLedKey[13]){
    statusBuz1 = false;
    statusBuz2 = true;
  }

  //play sound
  if(statusBuz1 == true || statusBuz2 == true){
    tone(pinBuz, Data);
  }
  else{
    noTone(pinBuz);
  }

  //test function
  if(testmode == 1){
    if(statusBuz1 == true || statusBuz2 == true){
      Serial.print("statusBuz:");
      Serial.println(Data);
    }
  }
}

//limit switch checking without Debounce function
//Updated 2013.11.07 11:00
//tested OK!
void commDodgem(){

  //checking if crash
  for(int i = 0; i< rangeKey;i++){
    if(digitalRead(pinKey[i])==defineKeyON){
      dodgemCrashed = true;
      statusKeyBuf |= bit(i); //fill the status of key
      //  Serial.println(statusKeyBuf);
    }
  }

  //checking all release
  if(digitalRead(pinKey[0])!=defineKeyON&&digitalRead(pinKey[1])!=defineKeyON){
    dodgemCrashed = false;
    statusKeyBuf = 0; //clear the status of key
    statusKeyBufOld = 0;
  }

  if(statusKeyBuf > statusKeyBufOld){
    //HP -- 
    valueHP -= statusKeyBuf*valueHPratio;
    statusKeyBufOld = statusKeyBuf;

    if(testmode == 1){
      Serial.print("Dodgem Status:");
      Serial.print(dodgemCrashed);
      Serial.print(" - ");
      Serial.println(statusKeyBuf,BIN);
      Serial.print("valueHP: ");
      Serial.println(valueHP);
    }
  }
}



