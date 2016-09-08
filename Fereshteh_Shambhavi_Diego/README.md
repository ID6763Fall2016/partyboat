# Party Bunny

This folder holds the source code of the Party Bunny. 

## Authors

* Fereshteh
* Shambhavi
* Diego Osorio

[Blog](http://ipdl.gatech.edu/doie2016/projects/p0-party-boat-shambhavi-diego-fereshteh/)

<a href="http://www.youtube.com/watch?feature=player_embedded&v=dRDf6i9t2mY" target="_blank"><img src="http://img.youtube.com/vi/dRDf6i9t2mY/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="480" height="360" border="10" /></a>

## Components

Due to latency issues when integrating all the party boat in a single Arduino UNO, it was split into two different components running in separate Arduino UNO

* Flex and Color
* Blink and Buzz

## Flex and Color

### Sensors

* Input
  * I2C color sensor
  * Ultrasonic distance sensor

* Output
  * DotStar LED Strip

## Libraries

The following libraries are needed:

#### Grove I2C Color Sensor

Library: [Grove I2C Color Sensor](https://github.com/Seeed-Studio/Grove_I2C_Color_Sensor)

#### Dotstar

[A tutorial on Adafruit](https://learn.adafruit.com/adafruit-dotstar-leds/overview)

Library: [Dotstar Library](https://github.com/adafruit/Adafruit_DotStar)


## Blink and Buzz

### Sensors

* Input
  * Softpot Potentiometer
  * Push button

* Output
  * LED
  * Speaker






