# Lovense-Edge-clone-esp32
Lovense Edge clone designed for ESP32

============Missing Features==============
 an input form the Soft Power Switch by sparkfun to play preset Pattens on each press
 or any Preset comands
 
====Circuit Advice to make this as close to an Edge 2 For a Compact insertable Build without making a custom PCB========
  Soft Power Switch by sparkfun https://www.sparkfun.com/products/17870
  any ESP32 based Microcontroller e.g. https://www.dfrobot.com/product-2195.html
  any PWM based Dual Motor Driver e.g. https://www.dfrobot.com/product-1492.html
  any MAX17043 Based I2C lipo Battery Fuel Guage e.g. https://www.sparkfun.com/products/10617 (you can get this particular one on ebay cheaper & in bulk)
  A lipo battery & charger  (A good Source for both of these with the added bonus of a built in 9v Boost Converter,
  to run the motors off is recharagble 9v on ebay, But most ESP32 can't Handle 9v so you will need a Buck converter to lower the voltage to 5v/3.3v to power the esp32) 
  Optional a boost/buck Converter - Boost the Voltage so the motors can run better, OR Buck/ dial back the voltage to run the ESP32 Correctly
  2 DC ERM vibration motors

==========Motor running functions=================
MA_F = Motor A Forward CW
MB_F = Motor B Forward CW
MA_B = Motor A Backward CCW - Not Used - Keept for testing purposes eg will it function better if the 2 motors run in opisite directions
MB_B = Motor B Backward CCW - Not Used - Keept for testing purposes
