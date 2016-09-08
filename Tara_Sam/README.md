This code was written in wiring for Arduino.

### Hookup ###
The serial usb input to the arduino is exposed.

Dotstar input:  9

Dotstar Jewel input:
	Datapin: 10
	clock pin: 11

Servo wheel: 9

Servo for disco ball: 8

Gyrometer input: A1(x), A2(y), A2(z)

Pressure sensor: A0

### Libraries needed ###

Dotstar library: Adafruit_DotStar.h
Neopixel library: Adafruit_NeoPixel.h
Wire: Wire.h

### Test for code ###

-> press pressure sensor
		activates jewel lights
		activates discoball servo
		activates servo for wheel
		
-> tilt boat left and right
		dotstars change direction of motion
		(dotstars switch on even with no tilting)