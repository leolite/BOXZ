1. Need to install AJSON lib and new BOXZ lib to to "x:\xxx\arduino-1.0.x\libraries\" , then unzip the package.
2. Open Arduino IDE and select File -> Example -> BOXZ -> BOXZ Mini
3. select Tools -> Board Select Arduino Nano and current Serial Port.
4. Upload to Arduino
5. Download IOS APP: https://itunes.apple.com/cn/app/boxz/id783688269?mt=8
6. Have fun!


BOXZ JSON Communication Protocol

/*
Define JSON
{"类型":{"属性1":参数1, "属性2":参数2}}
{"Type":{"Para1":Data1, "Para2":Data2}}

Type
CF: Config
- Para
-- ME: Message(0~255)
-- HP: Health para(0~100)


AT: Action
- Para
-- K1: Function Key1(w,a,s,d)
-- K2: Function Key2(u,i,j,k)

PT: Propty
- Para
-- ME: Message(ME = 1 is ID)
-- HP: Health para(0~100)


*/
1.  start/restart
    {"CF":{"HP":100}}                   IOS 
    {"PT":{"HP":100}}                   Arduino

2. Action K1 direction control
Go forward	{"AT":{"K1":"w"}}	
Go backward	{"AT":{"K1":"s"}}	
Turn Left	{"AT":{"K1":"a"}}
Turn Right	{"AT":{"K1":"d"}}	
Go Fwd Left 	{"AT":{"K1":"q"}}		
Go Fwd right	{"AT":{"K1":"e"}}		
Go Bwd Left	{"AT":{"K1":"z"}}		
Go Bwd Right	{"AT":{"K1":"x"}}			
Release key	{"AT":{"K1":" "}}		

3.  Action K2			
Button A   {"AT":{"K2":"u"}}	    
Button B   {"AT":{"K2":"i"}}
Button C   {"AT":{"K2":"j"}}
Button D   {"AT":{"K2":"k"}}
Release	   {"AT":{"K2":" "}}


