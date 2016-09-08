Safe Party Boat

## DESCRIPTION

This is the code file for the "Safe Party Boat" by John Kimionis and Colin Freeman.

The code is written for the Arduino Mega2560 or any other compatible board with the Atmel ATMega2560 microcontroller (Genuino, Sainsmart, etc).

The board outputs messages and/or debugging information to the serial console (9600 baud rate) and hosts the following sensors and actuators:

**SENSORS**

1. Force sensing resistor (FSR) - detects dance floor activity (dancing)
2. Photoresistor - detects sunlight to determine daytime/nighttime
3. 50K Potentiometer - detects wind speed
4. Tilt Sensor - detects boat tilting
5. DHT22 Sensor - detects temperature and humidity
6. BMP085 Barometer - detects atmospheric pressure
7. Ultrasound transducer - detects obstacle distance
8. 16-key Keypad - code entry and user input

**ACTUATORS**

1. Buzzer - beeps for alarm
2. 16x2 LCD screen - output messages
3. ES08A servo - raises “Party On” and “Party Off” flags
4. 16 pixel Neopixel strip - party lights
5. 3-color LED - output RGB color sequences


## CONNECTIONS

| Sensor Pins  | Mega2560 Pins |
| ------------- | ------------- |
| Tilt Sensor Pin 1  | 5 V  |
| Tilt Sensor Pin 2  | A12  |
| 3-color LED blue | D6  |
| 3-color LED green | D7|
| 3-color LED red | D8 |
| 3-color LED GND | GND |
| Buzzer Pin + | D28 |
| Buzzer Pin -| GND|
|BMP085 Pin 3V3|3.3 V|
|BMP085 Pin SCL|D21 (SCL)|
|BMP085 Pin SDA|D20 (SDA)|
|BMP085 Pin GND| GND|
|DHT22 Pin VDD| 5 V|
|DHT22 Pin Output|D22|
|DHT22 Pin GND|GND|
|Ultrasound Pin VDD| 5 V|
|Ultrasound Pin Trig|D26|
|Ultrasound Pin Echo|D24|
|Ultrasound Pin GND|GND|
|Servo Pin VDD| 5 V (external)|
|Servo Pin CTRL| D9|
|Servo Pin GND| GND & GND (external)|
|Potentiometer Pin 1|5 V|
|Potentiometer Pin 2|A0|
|Potentiometer Pin 3|GND|
|LCD Pin 1 (VSS)|GND|
|LCD Pin 2 (VDD)| 5 V|
|LCD Pin 3 (Vo)|through 2KOhm → GND|
|LCD Pin 4 (RS)|D12 (MISO)|
|LCD Pin 5 (R/W)|GND|
|LCD Pin 6 (E)|D11 (MOSI)|
|LCD Pin 7-10| Not Connected|
|LCD Pin 11 (DB4)| D5|
|LCD Pin 12 (DB5)| D4|
|LCD Pin 13 (DB6)| D3|
|LCD Pin 14 (DB7)| D2|
|LCD Pin 15 (LED+) | 5 V|
|LCD Pin 16 (LED-) | GND |
| Keypad Pin 1 (leftmost) | D52 |
| Keypad Pin 2   | D50 |
| Keypad Pin 3   | D48 |
| Keypad Pin 4   | D46 |
| Keypad Pin 5   | D44 |
| Keypad Pin 6   | D42 |
| Keypad Pin 7   | D40 |
| Keypad Pin 8 (rightmost) | D38 |
|FSR Pin 1|5 V|
|FSR Pin 2|A4 & through 1KOhm → GND|
|Photoresistor Pin 1|A6 & through 10KOhm → 5 V|
|Photoresistor Pin 2| GND|
|NeoPixel Pin 5 V|5 V|
|NeoPixel Pin CTRL|D30|
|NeoPixel Pin GND| GND|
