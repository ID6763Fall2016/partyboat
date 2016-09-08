/***************************************
 ** Flex and servo (flex)             **
 ***************************************/

// include this library's description file
#include "FlexAndServo.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

FlexAndServo::FlexAndServo()
{
}

void FlexAndServo::setup() 
{
  pinMode(FLEX_PIN, INPUT);
  flexServo.attach(9);
}

void FlexAndServo::loop() 
{
  // Read the ADC, and calculate voltage and resistance from it
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);
  if (DEBUG) {
    Serial.println("Resistance: " + String(flexR) + " ohms");
  }

  // Use the calculated resistance to estimate the sensor's
  // bend angle:
  float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);
  int val = analogRead(angle); 
  val = map(val, 0, 200, 0, 180);

  if (DEBUG) {
    Serial.println("Bend: " + String(angle) + " degrees");
    Serial.println();
  }

  // Rotates servo an specific angle based on flex sensor resistance value
  flexServo.write(val);  
  delay(150);
}

