/***************************************
 ** Blink and Buzz (bnb)              **
 ***************************************/

// ensure this library description is only included once
#ifndef BlinkAndBuzz_h
#define BlinkAndBuzz_h

// include description files for other libraries used (if any)
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#ifndef DEBUG
  #define DEBUG false
#endif

#include <HardwareSerial.h>

// include types & constants of Wiring core API
#include "pitches.h"

// library interface description
class BlinkAndBuzz
{
  // user-accessible "public" interface
  public:
    BlinkAndBuzz();
    void loop();
    void setup();

  // library-accessible "private" interface
  private:
    // Pins
    const int touchSliderPin = 2; //analog pin 2
    const int ledPin = 13;
    const int buzzPin = 12;
    const int buzzButtonPin = 11;
    
    // Reference values
    const int maxTouchSliderValue = 1023;
    const unsigned long maxDuration = 1000;
    
    // Timers
    unsigned long blinkDuration;
    unsigned long buzzButtonTimer;
    unsigned long blinkingTimer;
    
    // State machines
    int buzzState;
    int isBlinking;

    void evaluateState(int duration);
};

#endif

