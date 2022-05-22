DIY Hismith + Lovense Edge Dildo Project designed for ESP32

My goal of this Project is to make a Custom Hismith Compatible Smart Vibrating Dildo


==============WHAT’S DONE==============
A Not Final Prototype Circuit
	DFrobot Beetle ESP32 - C3
	DFrobot HR8833 Thumbnail Sized DC Motor Driver 2x1.5A
	SparkFun LiPo Fuel Gauge
	TP4056 lipo charger

Most of the Code

==============WHAT’S IN PROGRESS==============
Circuit
Power Switch
Power Connector

The Code 
MAX17043 code isn’t Working
Button Pressed Presets/ Patterns


==============TO DO==============
Dildo Mould
Electronics Housing Design to be 3d Printed
 

===========Currently Missing Code Features==============

 an input from the Soft Power Switch circuit to play preset Patterns on each press
 or any Preset commands
 
====NOT FINALISED Recommended Circuit Parts========
This is to make a compact Insertable device that is as close to an Edge 2 as possible without 
making a custom PCB

  Soft Power Switch by sparkfun https://www.sparkfun.com/products/17870
  
  My code is currently optimised to work with the DFrobot Beetle ESP32 - C3 https://www.dfrobot.com/product-2566.html
  
  this is the cheapest & smallest Production ESP32 I've found
  
  any PWM based Dual Motor Driver I recommend  https://www.dfrobot.com/product-1492.html
  
  SparkFun LiPo Fuel Gauge (you can get this particular one on eBay cheaper & in bulk)
  
  A lipo battery  (A good Source for Small Batteries is a Li-ion rechargeable 9v you should be able to get these on eBay,  you get a bonus 9v Boost Converter 
Notice these batteries often come without protection Circuits, and the 9v boost converter can't be turned off without disconnecting the battery, so it will constantly drain it) 

 A lipo charger, A common & cheap one is the TP4056, and it has a protection circuit. 
 Notice these come from the factory setup for charging 1000mah lipo if using a smaller battery use https://www.best-microcontroller-projects.com/tp4056.html#TP4056_Current_Programming_Resistor as a guide
  
 A boost Converter - Boost the Voltage of your battery, so the motors run correctly at low PWM (or lovense 10%, Vibrate:1;), this voltage is dependent on your motors
  
  2 DC ERM vibration motors

