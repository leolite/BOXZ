IOS APP link
https://itunes.apple.com/cn/app/boxz/id783688269?mt=8

We have submited new APP update, but is not public yet.
So please download the old version:
Json_Serial_ROMEO_20131201v4z.zip

Update 2014.09.05
In this update, we fixed 2 emergency bug.
1 Update communication protocol, streamlined format(JSON). 
Sometimes, the old version of the communication protocol data overload can cause data loss.
Because communication protocol has changed, if the player are using an older version of the APP, the operation is invalid.
New Format: {"AT": {"K1": "w"}} (17 characters) 
Old Format: {"action": {"K1": "w"}} (22 characters) 


2 Add heartbeat function 
In order to avoid the robot is out of control when communication range is exceeded. 
We added the heartbeat control, the robot would stop if it does not receive heartbeat.