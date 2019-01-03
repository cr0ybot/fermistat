
Fermistat
=========

Arduino-based temp & humidity controller for my fermentation chamber. Currently only controls adding heat and adding humidity if below set ideal levels.

## Controls

The LCD keypad is used to set the desired temp & humidity. Temp is set with `UP` and `DOWN`, Humidity is set with `LEFT` and `RIGHT`. In order for these set values to persist after a power cycle, you must press `SELECT` after setting the desired values. There is currently no feedback on the screen for saved values. Connect to the Serial monitor to see the values loaded on startup and when the values are saved.

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

* Add support for temperature probe (instead of relying only on ambient temperature)
* Moisture probe is unnecessary, and I wouldn't recommend sticking it into anything you're fermenting
* Add support for cooling and dehumidifying
* Add support for an alarm/noisemaker if temperatures are outside safe ranges
* Bigger LCD to fit more information
* More buttons, or better UI
* WiFi connectivity, remote set temp/humidity, check levels, alarms
