
Fermistat
=========

Arduino-based temp & humidity controller for my fermentation chamber.

## Hardware

* [Arduino Deumilanova](https://www.arduino.cc/en/Main/arduinoBoardDuemilanove) with upgraded ATmega328P
* [MHUM-01 Humidity & Moisture Breakout](https://www.osepp.com/electronic-modules/breakout-boards/94-humidity-moisture-breakout) by OSEPP, which includes a DHT11 sensor
* [2 Channel 5v relay](http://wiki.sunfounder.cc/index.php?title=2_Channel_5V_Relay_Module) by inland (2c5vrelay) to control heating pad & humidifier
* [LCD & Keypad Shield](https://www.velleman.eu/products/view/?id=435510) by vellman (VMA203)
* Seedling heating mat for temperature control
* Homes Ultrasonic Cube Humidifier with the button taped down so it comes on with power (do not recommend, but it's what I had on hand)

## Libraries

* [CheapLCD](https://github.com/kdhooper/arduino-CheapLCD) v0.9.1 by Daniel Hooper, which uses the standard LiquidCrystal library under the hood but adds convenient keypad accessors
* [DHT sensor library](https://github.com/adafruit/DHT-sensor-library) v1.3.0 by Adafruit, requires [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
* [arduino-timer](https://github.com/contrem/arduino-timer) v1.0.0 by Michael Contreras

## To Do

* Currently DOES NOT SAVE set temp/humidity after reset/power cycle. Need to store in EEPROM.
