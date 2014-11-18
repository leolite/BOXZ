Library for BOXZ - General Robot Control

Created by Leo.Zhu, 21 July, 2013
Updated by Leo.Zhu, 04 Dec., 2014
https://github.com/leolite/BOXZ

Notice: the sample of this lib just support the old BOXZ, not support JSON format yet.

License: Attribution-NonCommercial-ShareAlike 3.0 Unported (CC BY-NC-SA 3.0)
http://creativecommons.org/licenses/by-nc-sa/3.0/

Wiki Chinese
http://wiki.geek-workshop.com/doku.php?id=arduino:libraries:boxz

Include following library
  Servo.h - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2




Modified record:
Update: 20141104
1. cancel Hardware automatic ini()
2. delete test function(print message)
3. simplify function

Update: 20130816
1. goRaw() renamed with motorRaw()
2. goRaws() renamed with motorRaws()
3. init() renamed with initMotor()

Update: 20130814
1. add servoRaw()
2. add servoRaws()
3. Update servoCom()(Both servo action) and motorCom()(add Key Q and E)
4. Update goRaw() and go Raws()

Update: 20130812
1. add Servo function(SEEED drive board not support Servo, because of disable PWM of Pin 9 and 10)
2. add servoCom()
3. Update motorCom() (servo could action when motor is running)

Update: 20130810
1. Support for Adafruit Motor Drive(not support RAW)
2. add checkIO_AF()
3. add initAFMotor()

Update: 20130809
1. add motorCom()

Update: 20130730
1. add goRaws() for control by raw data(String HEX).

Update: 20130729
1. add goRaw() for control by raw data(Long int HEX).

Update: 20130728
1. add checkIO_ED() and checkIO_DF() for checking board type
2. add init() for Automatic choose board type.
