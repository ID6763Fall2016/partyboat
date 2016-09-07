# Party Bunny

This folder holds the source code of the Party Bunny. 

<a href="http://www.youtube.com/watch?feature=player_embedded&v=OaJIg0TYXc8
" target="_blank"><img src="http://img.youtube.com/vi/OaJIg0TYXc8/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="480" height="360" border="10" /></a>

## Sensors

* Input
  * Softpot Potentiometer
  * Photocell

* Output
  * Dotstar
  * Speaker

## Sensors hook up

1. Put __Wave Shield__ on the Arduino board
2. Connect __Softpot Potentiometer__ to the board
  * The middle pin should be connected to the analog pin __A0__
3. Connect __Photocell__ to the board
  * Connect one end of the photocell to 5V, the other end to Analog __A1__
  * Connect one end of a 10K resistor from Analog __A0__ to ground
4. Connect __Dotstar__ to the board
  * Data pin is connected to pin __6__
  * Clock pin is connected to pin __7__
5. Connect __speaker__ to the __Wave Shield__

## Libraries

To compile the code, you may need to install a coupld of libraries.

#### Wave Shield

[A tutorial on Adafruit](https://learn.adafruit.com/adafruit-wave-shield-audio-shield-for-arduino/wavehc-library)

Library: [WaveHC Library](https://github.com/adafruit/WaveHC)

#### Dotstar

[A tutorial on Adafruit](https://learn.adafruit.com/adafruit-dotstar-leds/overview)

Library: [Dotstar Library](https://github.com/adafruit/Adafruit_DotStar)





