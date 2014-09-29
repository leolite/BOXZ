#include <EEPROM.h>
#include "aJSON.h"
#include "BDrive.h"

//2014.09.03 add by Leo
//1. fix bug, stop() every comment

//2014.08.30 add by Leo
//watchDogLimit = 5000;
//tested

//2014.08.30 add by ogre_c
//crash use interrupt type
//command send only once, not loop

//2013.12.01
//tested with Limit switch!
//Test ok without BOXZ lib
//Fixed watchDog for ROMEO with Leonardo, not support serialEvent.


aJsonStream serial_stream(&Serial);

//Serial speed config
//unsigned long serialSpeed = 115200; //9600 for HC/BT2.0 and 115200 for BLE 4.0
unsigned long serialSpeed = 9600;

int testmode =1; // 0: Disable; 1: testMode

//hardware define
int pinIO[]={
  2,3,A0,A1,A2,A3,11,12}; //pin 14,15,16,17
int pinMotor[]={
  4,7,5,6};
int pinServo[]={
  9,10};
int pinSoftSerial = 8;


//value Data
// - CF:Config Byte 00 - 08
int valueME; //Message
int valueHP; //Health para
int valueVB; //Visu Byte


// - AT:Action V0.1 for current version
int valueK1; //Button key 1 for BOXZ direction control
int valueK2; //Button key 2 for function key A/B/C/D


//for JSON
boolean watchDogEn;
int watchDogLen = 2000;
int watchDogCount = 0;

//add for connection online timer by Leo
boolean watchDogTimerEn = false;
static unsigned long watchDogTimer = 0;
static unsigned long watchDogLimit = 5000;


void setup()
{
  Serial.begin(serialSpeed);
  initJSON();
  initDrive();
}


void loop()
{
  //value limit
  if(valueHP<=1) valueHP = 1;

  serialData();
  commJSON();
  heartbeat();//2014.09.02 add by orge_c
  testSys(); //2014.09.02 add by orge_c
}
//*******************************************************************
void initJSON()
{
  //default value
  //futher function get data from APP
  //valueHP = 100; //2014.09.02 del by Leo
  stop();
  valueK1 = 0;
  valueK2 = 0;
  watchDogEn = false;
  watchDogTimerEn = false;
  watchDogTimer = millis();
  //Serial.println("Init...."); 
}
//*******************************************************************
//deal with Serial data
void serialData() 
{
  watchDogJSON(); 

  if (serial_stream.available()) {
    /* First, skip any accidental whitespace like newlines. */
    serial_stream.skip();
  }

  if (serial_stream.available()) {
    /* Something real on input, let's take a look. */
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    motorCom(valueK1); //20140830 change place to here.
    aJson.deleteItem(msg);
    watchDogCount ++;
    //freeMem("RAM");
  }
}

//deal with unknown format data
void watchDogJSON()
{
  if(watchDogCount >watchDogLen){
    //Serial.println(F("{\"PT\":{\"ME\":3}}")); //Watch Dog Enable! //2014.09.02 del by Leo
    valueME = 3;        //2014.09.02 add by Leo
    bitSet(valueVB,7);  //2014.09.02 add by Leo
    watchDogEn = true;
  }

  if(watchDogEn == true){
    while(Serial.read() >= 0){
    }
    serial_stream.flush();
    watchDogCount = 0;
    watchDogEn = false;
  }
}

void ComExecution(aJsonObject *msg)
{
  //{"CF":{"ME:1}}           IOS          
  //{"CF":{"HP":100}}        IOS 
  aJsonObject *config = aJson.getObjectItem(msg, "CF"); //property
  if (config) {
    aJsonObject *config_ME = aJson.getObjectItem(config, "ME");  //Message
    aJsonObject *config_HP = aJson.getObjectItem(config, "HP");  //HP

    //Sample: config_HP->valueint;//HPtoINT
    //Sample: config_HP->valuestring);//toString
    setHealth(config_HP->valueint);//2014.09.02 add by orge_c
    processME(config_ME->valueint); //2014.09.02 add by orge_c
  }
  //{"AT":{"K1":"w"}}
  //{"AT":{"K2":"u"}}
  aJsonObject *action = aJson.getObjectItem(msg, "AT");
  if(action) { 
    aJsonObject *action_K1 = aJson.getObjectItem(action, "K1"); //key1: Direction control
    aJsonObject *action_K2 = aJson.getObjectItem(action, "K2"); //key2: button and skill

    //just for current version start
    String key1s = action_K1->valuestring;
    String key2s = action_K2->valuestring;
    int key1Buf = key1s.charAt(0);
    int key2Buf = key2s.charAt(0);

    valueK1=valueByteChecking(key1Buf);
    valueK2=valueByteChecking(key2Buf); 
  }  
}


int valueByteChecking(int valueTemp)
{
  int valueLimit = 255;
  if(valueTemp>=0 && valueTemp<=valueLimit) return valueTemp;
}

//*******************************************************************
void commJSON()
{
  if (valueVB > 0) {
    aJsonObject *msg = createMessage();
    aJson.print(msg, &serial_stream);
    Serial.println(); /* Add newline. */
    aJson.deleteItem(msg);
    valueME = 0; //2014.09.02 add by Leo
  }
}

/* Generate message like: {"PT":{"HP":100, "MP":100}} */
//not public version, include information of next version
//1. valueVB
//2. PT -> ME -> ID
aJsonObject *createMessage()
{
  //{"PT":{"HP":100}}        Arduino
  //{"PT":{"ME":1264}}       Arduino
  aJsonObject *msg,*propty;
  msg = aJson.createObject(); 
  aJson.addItemToObject(msg, "PT", propty = aJson.createObject());
  if(bitRead(valueVB,3) == 1) 
  {
    aJson.addNumberToObject(propty ,"HP", valueHP);
    bitClear(valueVB,3); //2014.09.02 add by orge_c
  }
  if(bitRead(valueVB,7) == 1) 
  {
    aJson.addNumberToObject(propty ,"ME", valueME);
    bitClear(valueVB,7); //2014.09.02 add by orge_c
  }

  return msg;
}

//*******************************************************************
// given a PROGMEM string, use Serial.print() to send it out
// this is needed to save precious memory
//htanks to todbot for this http://todbot.com/blog/category/programming/
void printProgStr(const prog_char* str) {
  char c;
  if (!str) {
    return;
  }
  while ((c = pgm_read_byte(str))) {
    Serial.write(c);
    str++;
  }
}

//Code to print out the free memory

struct __freelist {
  size_t sz;
  struct __freelist *nx;
};

extern char * const __brkval;
extern struct __freelist *__flp;

uint16_t freeMem(uint16_t *biggest)
{
  char *brkval;
  char *cp;
  unsigned freeSpace;
  struct __freelist *fp1, *fp2;

  brkval = __brkval;
  if (brkval == 0) {
    brkval = __malloc_heap_start;
  }
  cp = __malloc_heap_end;
  if (cp == 0) {
    cp = ((char *)AVR_STACK_POINTER_REG) - __malloc_margin;
  }
  if (cp <= brkval) return 0;

  freeSpace = cp - brkval;

  for (*biggest = 0, fp1 = __flp, fp2 = 0;
     fp1;
     fp2 = fp1, fp1 = fp1->nx) {
      if (fp1->sz > *biggest) *biggest = fp1->sz;
    freeSpace += fp1->sz;
  }

  return freeSpace;
}

uint16_t biggest;


//*******************************************************************
void testSys()//2014.09.02 add by orge_c
{
  if (testmode == 1)
  {
    //test function for Button C and D
    if(valueK2 == 'j') {
      valueHP --; 
      bitSet(valueVB,3); 
    }
    if(valueK2 == 'k') {
      valueHP ++;
      bitSet(valueVB,3); 
    }
    valueK2 = ' ';

    //test function over
  }
}

void processME(int reqNo)//2014.09.02 add by orge_c
{ 
  switch (reqNo)
  { 
  case 0://spare
    {
      break;
    } 
  case 1://ID
    {
      //address of EEPROM 
      int addressID = 0x09;
      int boxzIDValue = 0xB2;
      int boxzIDReq = 1;//ID
      boolean boxzIDChecked = false;

      EEPROM.write(addressID, boxzIDValue);
      valueME = boxzIDValue; //add 2014.08.30
      boxzIDChecked = true;  
      bitSet(valueVB,7); 

      break;
    }
  case 2://HeartBeat
    {
      watchDogTimerEn = true; //set 1 when receive heartbeat
      valueME = 2; 
      bitSet(valueVB,7); 
      break;
    } 
  case 3://Out of watchDog
    {
      //deal in function watchDogJSON()
      break;
    } 
  case 4://RAM
    {   
      valueME = freeMem(&biggest);
      bitSet(valueVB,7); 
      break;
    }  

  }
}

void setHealth(int healthValue)//2014.09.02 add by orge_c
{
  int valueHPBuf;
  valueHPBuf = valueByteChecking(healthValue);
  //Process crash sensor to change valueHP 
  valueHP = valueHPBuf;
  //bitSet(valueVB,3); 
}

//2014.09.02 add by orge_c
//2014.09.02 updated by Leo
void heartbeat()
{
  if (millis() >= watchDogTimer + watchDogLimit){
    initJSON();
    //watchDogTimerEn = false;
    //watchDogTimer = millis();
    //stop();
    valueME = 9; 
    bitSet(valueVB,7); 
  } 

  //||(valueK1 + valueK2 > 0x60) bug
  if((watchDogTimerEn == true)){
    watchDogTimer = millis();
    watchDogTimerEn = false;
  }
}













