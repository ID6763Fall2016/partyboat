/***************************************
 ** Blink and Buzz (bnb)              **
 ***************************************/

// include this library's description file
#include "BlinkAndBuzz.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

BlinkAndBuzz::BlinkAndBuzz()
{
  // initialize this instance's variables
  blinkDuration = maxDuration;
  buzzState = false;
  isBlinking = false;
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void BlinkAndBuzz::setup() {
  // Sets speaker timer and pins
  buzzButtonTimer = millis();
  blinkingTimer = millis();
  pinMode(buzzButtonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(touchSliderPin, HIGH); //enable pullup resistor
}

void BlinkAndBuzz::loop() {
  int buttonState = 0;
  unsigned long currentTimer = millis();

  int touchSliderReading = analogRead(touchSliderPin);
  if(touchSliderReading > 10) {
    blinkDuration = maxDuration * touchSliderReading / maxTouchSliderValue;
    // Some delay to improve touch slider reading
    // removed delay to avoid blocking processor
    //delay(100);
  }

  // Delay for reading speaker button activator
  if (currentTimer - buzzButtonTimer >= 500) {
    buzzButtonTimer = currentTimer;

    // read the state of the pushbutton value:
    buttonState = digitalRead(buzzButtonPin);
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      // turn sound on/off
      buzzState = !buzzState;
      
      if (!buzzState) {
        noTone(buzzPin);
      }
    }
  }

  // Blinking using timers
  evaluateState(blinkDuration);
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void BlinkAndBuzz::evaluateState(int duration) {
  // Non-blocking blinking based on duration and timers
  unsigned long currentTimer = millis();
  if (currentTimer - blinkingTimer >= duration / 2) {
    blinkingTimer = currentTimer;
    
    isBlinking = !isBlinking;
    if (isBlinking) {
      // turn the LED on (HIGH is the voltage level)
      digitalWrite(ledPin, HIGH);
      if (buzzState) {
        // set speaker note if active
        tone(buzzPin, NOTE_C4);
      }
    } else {
      // turn the LED off (LOW is the voltage level)
      digitalWrite(ledPin, LOW);
      if (buzzState) {
        // set speaker note off
        noTone(buzzPin);
      }
    }
  } 
}

