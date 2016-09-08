/***************************************
 ** Color Sensor and Strip            **
 ***************************************/

// ensure this library description is only included once
#ifndef ColorSensorStrip_h
#define ColorSensorStrip_h

// include description files for other libraries used (if any)
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

/* HC-SR04 Sensor
   https://www.dealextreme.com/p/hc-sr04-ultrasonic-sensor-distance-measuring-module-133696
  
   This sketch reads a HC-SR04 ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse 
   to return.  The length of the returning pulse is proportional to 
   the distance of the object from the sensor.
     
   The circuit:
  * VCC connection of the sensor attached to +5V
  * GND connection of the sensor attached to ground
  * TRIG connection of the sensor attached to digital pin 2
  * ECHO connection of the sensor attached to digital pin 4


   Original code for Ping))) example was created by David A. Mellis
   Adapted for HC-SR04 by Tautvidas Sipavicius

   This example code is in the public domain.
 */
#include <SPI.h>
#include <Wire.h>

// Requires installing this libraries on local Arduino
#include <GroveColorSensor.h>
#include <Adafruit_DotStar.h>

#ifndef DEBUG
  #define DEBUG false
#endif

#define NUMPIXELS 30
#define DATAPIN   4
#define CLOCKPIN  5

// library interface description
class ColorSensorStrip
{
  // user-accessible "public" interface
  public:
    ColorSensorStrip();
    void loop();
    void setup();

  // library-accessible "private" interface
  private:
    // Pins
    const int trigPin = 2;
    const int echoPin = 3;
    
    // Color Sensor
    GroveColorSensor colorSensor;

    // Color strip controller
    Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
    
    // Color Strip
    int stripHead = 0;
    int stripTail = -10; // Index of first 'on' and 'off' pixels
    int stripColorRed = 0xFF;
    int stripColorGreen = 0xFF;
    int stripColorBlue = 0xFF;

    // Methods
    void setupColorSensor();
    void setupStrip();
    void readColorSensor(int *red, int *green, int *blue);
    void writeLEDStripColor(int red, int green, int   blue);

    // Helpers to convert between distance and time
    long microsecondsToInches(long microseconds);
    long microsecondsToCentimeters(long microseconds);
};

#endif
