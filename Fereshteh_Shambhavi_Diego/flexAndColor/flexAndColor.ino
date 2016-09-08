#define DEBUG false

// Requires installing this libraries on local Arduino
#include <GroveColorSensor.h>
#include <Adafruit_DotStar.h>

// Custom classes
#include "ColorSensorStrip.h"
#include "FlexAndServo.h"

FlexAndServo flex = FlexAndServo();
ColorSensorStrip colorSensorAndStrip = ColorSensorStrip();

void setup(){
  // initialize serial communication:
  if (DEBUG) {
    Serial.begin(9600);
  }

  colorSensorAndStrip.setup();
  flex.setup();
}

void loop(){
  colorSensorAndStrip.loop();
  flex.loop();
}

