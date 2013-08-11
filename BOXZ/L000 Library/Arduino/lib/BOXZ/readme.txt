Library for general robot control

Created by Leo.Zhu, 21 July, 2013.
https://github.com/leolite/BOXZ

License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
http://creativecommons.org/licenses/by-nc-sa/3.0/

Wiki Chinese
http://wiki.geek-workshop.com/doku.php?id=arduino:libraries:boxz

Hardware support list: 
1. DFRobot L298 Shield 2A
2. DFRobot L293 Shield 1A
3. Seeed Motor Shield V2.0
4. Adafruit Motor Drive

Known issues:
1. AFMotor not support RAW yet.


Modified record:
Updata: 20130810
1. Support for Adafruit Motor Drive
2. add checkIO_AF()
3. add initAFMotor()

Updata: 20130809
1. add motorCom()

Update: 20130730
1. add goRaws() for control by raw data(String HEX).

Update: 20130729
1. add goRaw() for control by raw data(Long int HEX).

Update: 20130728
1. add checkIO_ED() and checkIO_DF() for checking board type
2. add init() for Automatic choose board type.
