# "Base" Arduino sketch

This sketch is intended to be uploaded to an Arduino Uno in the base of a CloudLamp.  Its primary purpose is to physically interact with hardware in the base of the CloudLamp (buttons, potentiometer, water pump, water level sensor) based on serial communications using the main `Serial` port.

Before connecting a USB cable to the Arduino Uno, the CloudLamp primary 5V supply should be disconnected and no USB cable should be plugged into the ESP32.  Required libraries are noted as comments in [base.ino](base.ino).

