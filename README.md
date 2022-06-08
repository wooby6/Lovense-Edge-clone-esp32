DIY Hismith + Lovense Edge Dildo Project designed for ESP32

My goal of this Project is to make a Custom Hismith Compatible Smart Vibrating Dildo


==============WHAT’S DONE==============

A Not Final Prototype Circuit

	DFrobot Beetle ESP32 - C3
	
	DFrobot HR8833 Thumbnail Sized DC Motor Driver 2x1.5A
	
	SparkFun LiPo Fuel Gauge
	
	TP4056 lipo charger
	
	Pololu Adjustable Step-Up/Step-Down Voltage Regulator S7V8
	
Most of the Code

	All Vibrators Working, With Very Little Delay
	BLE functioning & Connects To the Xtoys.app
	MAX17043 code now Working


==============WHAT’S IN PROGRESS==============

Circuit

	Power Switch
	Power Connector
The Code 

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

1x Soft Power Switch by sparkfun https://www.sparkfun.com/products/17870
  
1x DFrobot Beetle ESP32 - C3 https://www.dfrobot.com/product-2566.html
  
	My Code Has been Optimised to work with this Low Frequency but also cheap & Tiny ESP32, 
	It's the Smallest & Cheapest I've found
  
1x DFrobot HR8833 Thumbnail Sized DC Motor Driver 2x1.5A   https://www.dfrobot.com/product-1492.html
  
1x  SparkFun LiPo Fuel Gauge 

	(you can get this particular one on eBay cheaper & in bulk)
  
1x  A lipo battery at least 500mAh  

	(A good Source for Small Batteries is a Li-ion rechargeable 9v you should be able to get these on eBay,  
	you get a bonus dc-dc 9v Step-up  Circuit/ 9v Boost Converter
	
	Notice these batteries often come without protection Circuits, 
	and the 9v boost converter can't be turned off without disconnecting the battery, 
	so it will constantly drain it) 

 A lipo charger, A common & cheap one is the TP4056, and it has a protection circuit.
 
 	Notice these come from the factory setup for charging 1000mah lipo if using a smaller battery use 
	https://www.best-microcontroller-projects.com/tp4056.html#TP4056_Current_Programming_Resistor as a guide
  
 A Adjustable boost Converter - Pololu Adjustable Step-Up/Step-Down Voltage Regulator S7V8A

 
 	This is to Ensure that the motors run correctly at a low PWM signal (or lovense Vibrate:1; 10% Power),
	this voltage is dependent on your motors
	
A 5v step-up Regulator to power the DFrobot Beetle ESP32 - C3 via the VIN + GND pins.

	Using the BAT + GND pins is possible but its not stable when the motors draw a high Current 
  
  2 DC ERM vibration motors 
	
	Easiest sources for these is cheap Vibrating egg Sex toys



