# Introduction
The Calculator-Microcontroller is a system with goals are:

  - creats a HTTP network
  - calculating simple mathematical problems
  - logs the calculated calculations

# config-file 
The configuration for setting up both the mDNS and the calculator can be found in the file "Arduino_IDE_ESP-mDNS_Config.ino".

## Requirements 
# server:
 ESP8266

  - The Arduino code works only on an ESP8266.
  - This project requires a separate power supply.
  - You will need three LEDs.
  - A breadboard is also necessary.

# client:

- windows 64 bit
- Visual Studio Version 17.12.4
- curl version
WLAN-Konfiguration

## Einrichtung
# server:

 - copy Adruino_IDE_ESP-mDNS_Config.ino via micro-USB to the ESP8266
 - Server starts when power is available

# client:

 - copy Visual_Studio_Function.cpp to your local drive
 - open VSC and start the application
