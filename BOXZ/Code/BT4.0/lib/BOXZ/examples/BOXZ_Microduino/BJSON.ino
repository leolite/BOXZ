
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
//deal with Serial data input, include watch dog function
void serialDataInput() 
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
    //motorCom(valueK1); //20140830 change place to here.
    //motorComSP(valueK1,valueV1,valueV2,testmode);
    aJson.deleteItem(msg);
    watchDogCount ++;
    serialDataDone= true;
  }
}

//deal with unknown format data
void watchDogJSON()
{
  if(watchDogCount > watchDogCountLimit){
    //Serial.println(F("{\"PT\":{\"ME\":3}}")); //Watch Dog Enable! //2014.09.02 delete by Leo
    valueME = 3;        //2014.09.02 add by Leo
    bitSet(valueVB,7);  //2014.09.02 add by Leo
    watchDogEn = true;
  }

  if(watchDogEn == true){
    while(mySerial.read() >= 0){
    }
    serial_stream.flush();
    watchDogCount = 0;
    watchDogEn = false;
  }
}

//deal with JSON data input
void ComExecution(aJsonObject *msg)
{
  //Example
  //{"CF":{"ME:1}}           IOS          
  //{"CF":{"HP":100}}        IOS 
  aJsonObject *config = aJson.getObjectItem(msg, "CF"); //property
  if (config) {
    aJsonObject *config_ME = aJson.getObjectItem(config, "ME");  //Message
    aJsonObject *config_HP = aJson.getObjectItem(config, "HP");  //HP
    aJsonObject *config_MP = aJson.getObjectItem(config, "MP");  //MP
    //Sample: config_HP->valueint;//HPtoINT
    //Sample: config_HP->valuestring);//toString
    int valueHPBuf = config_HP->valueint;
    int valueMPBuf = config_MP->valueint;
    if(valueHPBuf>=1&&valueHPBuf<=255) valueHP=valueHPBuf;
    if(valueMPBuf>=1&&valueMPBuf<=255) valueMP=valueMPBuf; 
    //valueHP = valueByteChecking(config_HP->valueint);
    //valueMP = valueByteChecking(config_MP->valueint);

    //setHealth(config_HP->valueint);//2014.09.02 add by orge_c
    //setMagic(config_MP->valueint);//2014.09.22 add by Leo
    processME(config_ME->valueint); //2014.09.02 add by orge_c
    watchDogCount = 0;
  }
  //{"AT":{"K1":"w"}}
  //{"AT":{"K2":"u"}}
  aJsonObject *action = aJson.getObjectItem(msg, "AT");
  if(action) { 
    aJsonObject *action_V1 = aJson.getObjectItem(action, "V1"); 
    aJsonObject *action_V2 = aJson.getObjectItem(action, "V2"); 
    aJsonObject *action_K1 = aJson.getObjectItem(action, "K1"); //key1: Direction control
    aJsonObject *action_K2 = aJson.getObjectItem(action, "K2"); //key2: button and skill

    //just for current version start
    String key1s = action_K1->valuestring;
    String key2s = action_K2->valuestring;
    int valueK1Buf = key1s.charAt(0);
    int valueK2Buf = key2s.charAt(0);

    //valueK1=valueByteChecking(key1Buf);
    //valueK2=valueByteChecking(key2Buf); 
    int valueV1Buf = action_V1->valueint;
    int valueV2Buf = action_V2->valueint;
    if(valueV1Buf>=50&&valueV1Buf<=255) valueV1=valueV1Buf; //here is a bug if > 1, maybe get unknown value ?3/?7
    if(valueV2Buf>=50&&valueV2Buf<=255) valueV2=valueV2Buf; //also the speed could not reach too low.
    if(valueK1Buf>=1&&valueK1Buf<=255) valueK1=valueK1Buf;
    if(valueK2Buf>=1&&valueK2Buf<=255) valueK2=valueK2Buf; 
    //valueV1 = valueByteChecking(action_V1->valueint);  
    //valueV2 = valueByteChecking(action_V2->valueint); 
    watchDogCount = 0;
  }
}


//*******************************************************************
//Output data for serial data
void serialDataOutput()
{
  if (valueVB > 0) {
    aJsonObject *msg = createMessage();
    aJson.print(msg, &serial_stream);
    mySerial.println(); /* Add newline. */
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
  if(bitRead(valueVB,0) == 1) 
  {
    aJson.addNumberToObject(propty ,"V2", valueV2);
    bitClear(valueVB,0); //2014.09.22 add by Leo
  }
  if(bitRead(valueVB,1) == 1) 
  {
    aJson.addNumberToObject(propty ,"V1", valueV1);
    bitClear(valueVB,1); //2014.09.22 add by Leo
  }
  if(bitRead(valueVB,2) == 1) 
  {
    aJson.addNumberToObject(propty ,"MP", valueMP);
    bitClear(valueVB,2); //2014.09.22 add by Leo
  }
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
void testFunction(int _mode)//2014.09.02 add by orge_c
{
  //0xC1 TestMode 1 - HP/MP Test Mode(if TestMode >1 bypass heatbeat)
  if (_mode == 1)
  {
    switch (valueK2)
    { 
    case 'u':
      {
        valueHP -= 15; 
        bitSet(valueVB,3); 
        break;
      } 
    case 'i':
      {   
        valueHP += 15;
        bitSet(valueVB,3); 
        break;
      } 
    case 'j':
      {   
        valueMP -= 15; 
        bitSet(valueVB,2); 
        break;
      } 
    case 'k':
      {   
        valueMP += 15;
        bitSet(valueVB,2); 
        break;
      } 
    default: 
      break;
    }
    valueK2 = ' ';
  }

  //0xC2 TestMode 2 - Speed Test Mode(if TestMode >1 bypass heatbeat)
  //Speed is not output yet. but replace with output HP/MP
  if (_mode == 2)
  {
    switch (valueK2)
    { 
    case 'u':
      {
        valueHP -= 15; 
        valueV1 = valueHP;
        bitSet(valueVB,3); 
        break;
      } 
    case 'i':
      {   
        valueHP += 15;
        valueV1 = valueHP;
        bitSet(valueVB,3); 
        break;
      } 
    case 'j':
      {   
        valueMP -= 15; 
        valueV2 = valueMP;
        bitSet(valueVB,2); 
        break;
      } 
    case 'k':
      {   
        valueMP += 15;
        valueV2 = valueMP;
        bitSet(valueVB,2); 
        break;
      } 
    default: 
      break;
    }
    valueK2 = ' ';
  }

  //if (_mode == 2) this function running in drive function
}

//*******************************************************************
//2014.09.02 add by orge_c
//Message process
void processME(int reqNo)
{ 
  switch (reqNo)
  { 
  case 0x00://spare
    {
      break;
    } 
  case 0x01://ID
    {
      //address of EEPROM 
      int addressID = 0x09;
      int boxzIDValue = 0xB0;
      int boxzIDReq = 1;//ID
      boolean boxzIDChecked = false;

      EEPROM.write(addressID, boxzIDValue);
      valueME = boxzIDValue; //add 2014.08.30
      boxzIDChecked = true;  
      bitSet(valueVB,7); 

      break;
    }
  case 0x02://HeartBeat
    {
      watchDogTimerEn = true; //set 1 when receive heartbeat
      valueME = 2; 
      bitSet(valueVB,7); 
      //serialDataDone = false; //disable test output
      break;
    } 
  case 0x03://Out of watchDog
    {
      //deal in function watchDogJSON()
      break;
    } 
  case 0x04://RAM
    {   
      //valueME = freeMem(&biggest);
      bitSet(valueVB,7); 
      break;
    }  


  case 0x09://Spare of out of watchDog
    {   
      break;
    } 

  case 0xC1://Test Mode 1
    {   
      testmode = 1;
      valueME = 0xC1;
      bitSet(valueVB,7); 
      break;
    } 

  case 0xC2://Test Mode 2
    {   
      testmode = 2; 
      valueME = 0xC2;
      bitSet(valueVB,7); 
      break;
    } 

  case 0xC3://Test Mode 3 spare yet
    {   
      testmode = 3; 
      valueME = 0xC3;
      bitSet(valueVB,7); 
      break;
    } 
  default: 
    break;
  }
}



//*******************************************************************
//2014.09.02 add by orge_c
//2014.09.02 updated by Leo
//2014.09.22 updated by Leo add testMode == 0, if not deactive this function
void heartbeat()
{
  if(testmode == 0){
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
}

//*******************************************************************
void valueCheck()
{   
  //value limit, fix the value out of range
  if(valueHP<=1) valueHP = 1;
  if(valueMP<=1) valueMP = 1;
  if(valueHP>=255) valueHP = 255;
  if(valueMP>=255) valueMP = 255;

  //Update output VB when HP and MP value changed
  if(valueHPtemp != valueHP){
    bitSet(valueVB,3);
    valueHPtemp = valueHP;
  }
  if(valueMPtemp != valueMP){
    bitSet(valueVB,2);
    valueMPtemp = valueMP;
  }
}


//*******************************************************************
//Reset serial data done
void serialDataReset()
{
  if(serialDataDone == true) 
    serialDataDone = false; //this is the shift bit for print.
}


