#define DEBUG false

#include "BlinkAndBuzz.h"

BlinkAndBuzz bnb = BlinkAndBuzz();

void setup(){
  // initialize serial communication:
  if (DEBUG) {
    Serial.begin(9600);
  }

  bnb.setup();
}

void loop(){
  bnb.loop();
}
