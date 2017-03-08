Main Idea of the project : Replace the RF remote controls 
by an Android app that uses Bluetooth ...

we need to hack the hardware and produce an Android app that control the car

full description of the project can be found
https://sites.google.com/site/nabilchouba2/home/nikko-rc-car-bluetooth-remake


2 - hardware Bluetooth connection :

I will  use the started Ti kit EZ430-RF2560, that use LowPower Bleutoth chip : CC2560 and the MSP340 micro-controller.

fig4-EZ430-RF2560

We can use the connector uEVM P1 (see fig4) but have specific connector & knowledge & material to solder it :(
Yeap ... There is GND, VDD and 4 signal (P7.4, P7.5, P7.6, P7.7) available on connector  MSP430BT5190 J3 (exactly what we need).
P7.7 : Down  //  P7.6 : Left  // P7.5 : Up  // P7.4 : Right


The Bluetooth Android App send to the car one byte every 200ms coding those move :
(up('c'), down('z'), left('b'), right('d'), leftUP('2'), rightUP('3'), leftDown('0'), rightDown('3'),UPTurbo('z'), leftUPTurbo('y'), rightDownTurbo('x'))
The msp430 receive the byte and generate the PWM signal by loop that generate 10 pulse and a period of 4ms.
When the Bluetooth connection is lost ... the car stay safe and don't move as he does not receive any byte ;)

3 - Software Java Android App :

The main idea of The Bluetooth Android App is :

fig5 : Android joystick

   1- We get the position of the cursor on the Joystick.
   2- We check the needed action to do, we stored in one variable.
   3- We program one Timer that run every 200ms, depending on the needed movement we send a specific byte to CC2560.


In the datasheet of msp430 & CC2560 it's clearly says that it's can run with a power supply of 1.7 to 3.3.
We need to generate controls signal under the level of ~2V => power supply for the kit need be 2V.
Unfortunately with 2V nothing work ... I have use 3V to bypass the issue (this problem need further investigation ).
the four connected msp controls signals can only reach 2V. it should be 3V (need to understand the Motor driver circuit).

A simple circuit based on LM317T can generate the 3V power supply from the 9V battery (see fig. 5)

By using the same power supply source for the motor & EZ430-RF2560, the system isn't stable.
I lost the connection with the Bluetooth chip every 30sec ... this lead to break my EZ430-RF2560 kit too

The solution is to use different battery for micro-controller, this solve the problem, and the system is stable now ;)
I lost the charger and the battery :( after forgotten them charging for more that 24h or the maximum is 4h :(
now I m using an external power supply 9.7V 2600mA ... planning to buy an LiFe battery ;)
