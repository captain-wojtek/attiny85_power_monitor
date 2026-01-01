# attiny85_power_monitor
I wanted to create a portable battery module in which I would be able to see the battery voltage, the current and power draw whenever I connect something to it. So, I made this one. Currently, the connections and basic code is done and would update this once the casing and other parts are completed.

## Components

1. 2x 3.7V 18650 Lipo battery (I had 8.14wh cells) - Link
2. 2S Lipo battery holder (I bought the one with a barrel DC jack 5.5x2.1 mm output) - [Link](https://robu.in/product/2-x-18650-black-battery-holder-with-dc-power-plug/)
3. 2S 5A 8.4V 18650 BMS - [Link](https://robu.in/product/2s-3a-8-4v-18650-lithium-battery-charger-board-protection-module/)
4. SSD1306 128x64 I2C module (I had the 0.96 inch module)
5. INA219 I2C module - [Link](https://robu.in/product/cjmcu-219-ina219-i2c-interface-no-drift-bi-directional-current-power-monitoring-sensor-module/)
6. Attiny85 Digispark module - [Link](https://robu.in/product/digispark-attiny85-mini-usb-development-board/)
7. 3.3V 5V and input - multi voltage output module - [Link](https://robu.in/product/dc-dc-voltage-converter-multi-output-power-supply-module-12v-3-3v5v12v/)
8. Some jumper wires and normal wires for soldering requirements

I bought all these components from robu.in and I couldn't get the links for few components that I had with me before but you can easily find alternatives for that from robu.

## Connection

Images for the connections is present in docs folder. Few things to look for when trying to make the connections:
- For I2C, use PB0 for SDA and PB2 for SCL as PB1 has some interference with the USB port in this Digispark module
- The INA219 module and Attiny85 should have a common ground. Since it is portable and I can power the module with the battery, it works fine but if you keep different grounds for these, the readings would be rubbish
- Since this has a USB interface and you can flash through this, I would not recommend powering the board from USB as well as the battery at the same time. I didn't face any issues with this but I won't recommend it for safety purposes.

## Code

Since I didn't have an Arduino Uno or any other bootloader tool for this, I wanted somethign which can be used as a standalone for this board without other Arduino library dependencies. Since this has limited memory, for the display, [SSD1306_minimal](https://github.com/kirknorthrop/SSD1306_minimal/tree/master?tab=License-1-ov-file) library is used. For the INA219, I have written a custom library following the datasheet for it and made modifications so that the Attiny85 directly reads from the registers of the INA219 and report it. 


All the calibrations are done in the code and you can change it based on the current limit you want to set. For the I2C, [TinyWireM](https://github.com/adafruit/TinyWireM) is used for the less space requirement. Only the TinyWireM and USI_TWI_Master files are used as they are more than enough for this. All the Arduino.h and other Aruino related libraries have been replaced with avr libraries for direct building and flashing for Attiny85. This SSD1306 minimal library can display only characters and strings so the floating point values have been converted to string(ChatGPT helped me with this).

## Flashing code

This Digispark module has the micronucleus bootloader present in it by default. This takes up around 2kB of space on the 8kB flash but gives the advantage of not needing an Arduino Uno or an USBASP or other flashing tool to be used. In case yo have a module that doesn't have this, for the first flashing of the [micronucleus](https://github.com/micronucleus/micronucleus/releases), you would need an external bootloader. 

You would have to install the micronucleus tool in your pc for flashing code to the Attiny85. Commands for building and flashing the code is present in the [Makefile](Makefile). Since I have done this part in Linux, I'm not exactly sure of the processes to install the micronucleus and other dependencies in Mac or Windows, so the below part is specifically for linux. 
