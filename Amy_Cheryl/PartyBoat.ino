/*
 * This sketch illustrates opening files by index
 * which can significantly reduce latency.
 *
 * How to prepare a test SD:
 * Start with a clean newly formatted SD.
 * First copy the 400 files in the 'fill' folder to the SD.
 * Next copy the 16 files in the 'DTMF' folder to the SD.
 * There should be 416 files in the SD root directory.
 *
 * You must copy the files in the above order so the 'fill'
 * files occur in the directory before the DTMF files.
 *
 * Run this sketch using the prepared SD.  Notice the
 * difference in latency between play by name and
 * play by index.
 */

#include <WaveHC.h>
#include <WaveUtil.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>        
#include <Wire.h>

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file 
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

/*
 * Init helper variables
 */
boolean isActive = false;
int mode = HIGH; //LOW - slow music & dim light; HIGH - quick music & bright light
int buttonState = HIGH;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

/*
 * Init helper constants
 */
// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can't be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int potThreshold = 800;
int lightThreshold = 600;

/*
 * Dotstar declarations
 */
#define NUMPIXELS 11 // Number of LEDs in strip
// Here's how to control the LEDs from any two pins:
#define DATAPIN    6
#define CLOCKPIN   7
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
int      head  = 0, tail = -7; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)

// More Pins declarations
int softpotPin = A0; // Pin for potentiometer softpot
int lightPin = A1; // Pin for photocell

/*
 * Define macro to put error messages in flash memory
 */
#define error(msg) error_P(PSTR(msg))

//////////////////////////////////// SETUP
void setup() {

  // Set pin mode
  digitalWrite(softpotPin, HIGH);
  digitalWrite(lightPin, HIGH);

  // Start serial
  Serial.begin(9600);

  // Set up dotstar
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  // Set up the connection with SD card
  if (!card.init()) error("card.init");

  // enable optimized read - some cards may timeout
  card.partialBlockRead(true);

  if (!vol.init(card)) error("vol.init");

  if (!root.openRoot(vol)) error("openRoot");

}


//////////////////////////////////// LOOP
void loop() {

  // Reading values from input pins
  int softPotReading = analogRead(softpotPin);
  int lightReading = analogRead(lightPin);

  // Log potentiometer value
  Serial.print("Potentiometer: ");
  Serial.println(softPotReading);
  Serial.println("=================");

  // Everytime the value is smaller than the threshold, 
  // toggle the state of the bunny
  // TODO debounce
  if (softPotReading < potThreshold) {
    isActive = !isActive;
    togglePlay();
  }

  // Log photocell reading value
  Serial.print("Light: ");
  Serial.println(lightReading);
  Serial.println("=================");

  // Everytime the value is smaller than the threshold, 
  // toggle the mode of the bunny
  int curMode = mode;
  if (lightReading < lightThreshold) {
    curMode = LOW;
  } else {
    curMode = HIGH;
  }
  if (curMode != mode) {
    mode = curMode;
    togglePlay();
  } else {
    mode = curMode;
  }

  // Update all sensors based on the state and the mode
  // isActive: 
  //    TRUE  --> all sensors on
  //    FALSE --> all sensors off
  // mode:
  //    HIGH --> fast music and fast-animated&bright light
  //    LOW  --> soft music and static&dim light
  if (isActive) {
    if (mode) {
      strip.setBrightness(200);
      strip.setPixelColor(head, color); // 'On' pixel at head
      strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
      strip.show();                     // Refresh strip
    
      if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
        head = 0;                       //  Yes, reset head index to start
        if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
          color = 0xFF0000;             //   Yes, reset to red
      }
      if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
    } else {
      for (int i = 0; i < NUMPIXELS; i++) {
        strip.setBrightness(50);
        strip.setPixelColor(i, 0x421212);
        strip.show();
      }
    }
    
  } else {
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, 0);
      strip.show();
    }
  }  
 
}

/////////////////////////////////// HELPERS
/*
 * print error message and halt
 */
void error_P(const char *str) {
  PgmPrint("Error: ");
  SerialPrint_P(str);
  sdErrorCheck();
  while(1);
}
/*
 * print error message and halt if SD I/O error, great for debugging!
 */
void sdErrorCheck(void) {
  if (!card.errorCode()) return;
  PgmPrint("\r\nSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  PgmPrint(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

// Number of files.
#define FILE_COUNT 16

/*
 * Helper function
 * access SD card and switch music files 
 */
void togglePlay() {
  char name[10];

  if (wave.isplaying) {
    wave.stop();
  }
  if (isActive) {
    if (mode) {
      strcpy_P(name, PSTR("RED.WAV"));
    } else {
      strcpy_P(name, PSTR("BLUE.WAV"));
    }
    Serial.println(name);
    // open file by name
    if (!file.open(root, name)) error("open by name"); 
    // create wave and start play
    if (!wave.create(file))error("wave.create");
    wave.play();
  }
}

