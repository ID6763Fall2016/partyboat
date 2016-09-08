/***************************************
 ** Flex and servo (flex)             **
 ***************************************/

// ensure this library description is only included once
#ifndef FlexAndServo_h
#define FlexAndServo_h

// include description files for other libraries used (if any)
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

/******************************************************************************
Flex_Sensor_Example.ino
Example sketch for SparkFun's flex sensors
  (https://www.sparkfun.com/products/10264)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining a flex sensor with a 47k resistor.
- The resistor should connect from A0 to GND.
- The flex sensor should connect from A0 to 3.3V
As the resistance of the flex sensor increases (meaning it's being bent), the
voltage at A0 should decrease.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/

#include <Servo.h>

#ifndef DEBUG
  #define DEBUG false
#endif

// library interface description
class FlexAndServo
{
  // user-accessible "public" interface
  public:
    FlexAndServo();
    void loop();
    void setup();

  // library-accessible "private" interface
  private:
    Servo flexServo;
    const int FLEX_PIN = A0; // Pin connected to voltage divider output
    
    // Measure the voltage at 5V and the actual resistance of your
    // 47k resistor, and enter them below:
    const float VCC = 4.98; // Measured voltage of Ardunio 5V line
    const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor
    
    // Upload the code, then try to adjust these values to more
    // accurately calculate bend degree.
    const float STRAIGHT_RESISTANCE = 37300.0; // resistance when straight
    const float BEND_RESISTANCE = 90000.0; // resistance at 90 deg
};

#endif
