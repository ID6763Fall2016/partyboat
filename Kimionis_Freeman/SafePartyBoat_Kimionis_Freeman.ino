// ID6763 - Design of Interactive Environments
// Project 1 - Party Boat
// John Kimionis
// Colin Freeman
// "Safe Party Boat"

#include <Arduino.h>

#define ENABLE_SERIAL 1
#define ENABLE_LCD    1
#define ENABLE_SERVO  1
#define ENABLE_BUZZER 1
#define ENABLE_BAROMETER  1

#define DEBUG_POT         0
#define DEBUG_SERVO       0
#define DEBUG_KEYPAD      1
#define DEBUG_DHT         1
#define DEBUG_ULTRASOUND  0
#define DEBUG_BAROMETER   1
#define DEBUG_TILT        0

int guestCounter = 0;

#define GUESTS_MIN    0
#define GUESTS_PARTY  3
#define GUESTS_MAX    99

/**************** TILT ***********************************/

int tiltSensePin = A12;
int tiltCounter = 0;

#define TILT_THRESHOLD           512
#define TILT_COUNTER_THRESHOLD   20


/**************** LED ***********************************/
int LED_red = 8;
int LED_green = 7;
int LED_blue = 6;

unsigned int rgbColor[3] = {255, 0, 0}; // Start off with red.
int decColor = 0;
int incColor;
int i_color;

/**************** BUZZER ***********************************/
int buzzPin = 28;        // Voltage/current source for buzzer

/**************** PHOTOCELL ***********************************/
int photocellPin = A6;  // Analog input pin for photoresistor
const int photocellThreshold = 768; // 3/4 of max A/D value

/**************** BAROMETER ***********************************/
#if ENABLE_BAROMETER
#include <Wire.h>
#include "Adafruit_BMP085.h"

Adafruit_BMP085 bmp;

#endif

/**************** HUMTEMP SENSOR ***********************************/
#include "DHT.h"

#define DHTPIN 22     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

/**************** ULTRASOUND ***********************************/
const int TrigPin = 26;
const int EchoPin = 24;
float cm;

#define ULTRASOUND_THRESHOLD_CM   5

/**************** SERVO ***********************************/
#if ENABLE_SERVO
#include <Servo.h>

Servo myServo;  // create a servo object
int servo_pin = 9;

int ServoAngle;   // variable to hold the angle for the servo motor

#define SERVO_MIN_ANGLE   2
#define SERVO_DEFAULT_ANGLE   90
#define SERVO_MAX_ANGLE   170

#endif

/**************** POTENTIOMETER ***********************************/
int const potPin = A0; // analog pin used to connect the potentiometer
int potVal;  // variable to read the value from the analog pin

/**************** LCD ***********************************/
#if ENABLE_LCD
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#endif

/**************** Keypad ***********************************/
#include <Password.h>
#include <Keypad.h>

char keycode[] = "1234";
Password password = Password( keycode );

const byte ROWS = 4; // Four rows
const byte COLS = 4; //  columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 52, 50, 48, 46 }; // Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 44, 42, 40, 38, }; // Connect keypad COL0, COL1 and COL2 to these Arduino pins.


// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/****************** Force Sensing Resistor *********************************/
int fsrAnalogPin = A4; // FSR is connected to analog 4
int fsrReading;      // the analog reading from the FSR resistor divider
int fsrReading_mapped;

/**************** NeoPixel *********************************/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            30
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16
// Setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int NeoPixel_delayval = 10; // delay (ms)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**************** Time Events *********************************/

#include <EventManager.h>
unsigned long lastToggled_NeoPixel;
unsigned long lastToggled_LED;

EventManager gEM;


void listener_LED( int event, int param )
{

  /**************** TILT *********************************/

  int tiltSense = analogRead(tiltSensePin);
#if DEBUG_TILT
  Serial.println(tiltSense);
#endif

  if (tiltSense < TILT_THRESHOLD) {// at least 1/2 of Vcc: no tilt
    tiltCounter++;

    if (tiltCounter > TILT_COUNTER_THRESHOLD) {
      emergency();
      partyisover("Bad Sea!        ");
      tiltCounter = 0;
    }
  }

  /**************** POTENTIOMETER *********************************/
  // check wind

  potVal = analogRead(potPin); // read the value of the potentiometer

#if DEBUG_POT
  // print out the value to the serial monitor
  Serial.print("potVal: ");
  Serial.print(potVal);
#endif

  // if it is windy
  if (potVal > 512) {
    emergency();
    partyisover("Windy!          ");
  }

  /**************** PHOTOCELL *********************************/
  // check if sun has risen
  unsigned int val = analogRead(photocellPin);

  if (val > photocellThreshold) {     // check for high light condition
    partyisover("Good Morning!   ");
  }

  /**************** BAROMETER *********************************/
#if ENABLE_BAROMETER
#if DEBUG_BAROMETER

  //  Serial.print("Altitude = ");
  //  Serial.print(bmp.readAltitude(101500));
  //  Serial.println(" meters");

  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  Serial.println();
  
#endif

#endif
  /**************** HUMTEMP SENSOR *********************************/
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
#if DEBUG_DHT
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  else {
    Serial.print(h);
    Serial.print(" %RH, ");
    Serial.print(t);
    Serial.println(" C");
  }
#endif

  if (h > 80) {
    partyisover("Humid Weather   ");
  }

  /**************** ULTRASOUND *********************************/
  digitalWrite(TrigPin, LOW); //Low high and low level take a short time to TrigPin pulse
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58.0; //Echo time conversion into cm
  cm = (int(cm * 100.0)) / 100.0; //Keep two decimal places

#if DEBUG_ULTRASOUND
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
#endif


  if ((cm > 0) && (cm < ULTRASOUND_THRESHOLD_CM)) {
    emergency();
  }

  /******************* LED *****************************/
  // cycle through color spectrum

  if (decColor < 3) {
    incColor = (decColor == 2) ? 0 : decColor + 1;

    if (i_color < 255) {

      rgbColor[decColor]--;
      rgbColor[incColor]++;

      setColorRgb(rgbColor[0], rgbColor[1], rgbColor[2]);

      i_color++;
    }
    else {
      i_color = 0;
      decColor++;
    }

  }
  else {
    decColor = 0;
  }

  // keep current time
  lastToggled_LED = millis();
}


void listener_NeoPixel( int event, int param )
{
  /******************** NeoPixel *******************************/

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  for (int i = 0; i < NUMPIXELS; i++) {

    int NeoPixel_intensity = fsrReading_mapped;

    int NeoPixel_color_R = NeoPixel_intensity * random(0, 2);
    int NeoPixel_color_G = NeoPixel_intensity * random(0, 2);
    int NeoPixel_color_B = NeoPixel_intensity * random(0, 2);

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(NeoPixel_color_R, NeoPixel_color_G, NeoPixel_color_B)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

  }

  // keep current time
  lastToggled_NeoPixel = millis();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup() {

  /******************* RANDOM ****************************/
  randomSeed(analogRead(12));


  /******************* SERIAL ****************************/
#if ENABLE_SERIAL
  Serial.begin(9600);
  Serial.println("Hello.");
#endif

  /******************* BAROMETER****************************/
#if ENABLE_BAROMETER
#if DEBUG_BAROMETER
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) { // halt
      ;
    }
  }

#endif
#endif
  /******************* HUMTEMP SENSOR ****************************/
  dht.begin();

  /******************* ULTRASOUND ****************************/
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  /******************* BUZZER ****************************/
  pinMode(buzzPin, OUTPUT);

  /******************* LED ****************************/
  pinMode(LED_red, OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_blue, OUTPUT);

  /******************* SERVO ****************************/
  // move servo to default position
  myServo.attach(servo_pin);
  delay(100);
  myServo.write(SERVO_DEFAULT_ANGLE);
  delay(100);
  Serial.print(", actual angle: ");
  Serial.println(myServo.read());
  myServo.detach();

  /******************* LCD ****************************/
#if ENABLE_LCD
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Welcome to the");
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  lcd.print("party boat!");
#endif

  /******************* Keypad ****************************/
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad

  /******************* NeoPixel ********************************/
  pixels.begin(); // This initializes the NeoPixel library.

  /******************* Time Events ********************************/
  lastToggled_NeoPixel = millis();
  lastToggled_LED = millis();

  // Add our listener
  gEM.addListener( EventManager::kEventUser0, listener_LED );
  gEM.addListener( EventManager::kEventUser1, listener_NeoPixel );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop() {

  /******************* Keypad *****************************/
  char keystroke = keypad.getKey();

#if DEBUG_KEYPAD
  if (keystroke) {
    Serial.println(keystroke);
  }
#endif

  /********************* Force Sensing Resistor******************************/
  fsrReading = analogRead(fsrAnalogPin);

  fsrReading_mapped = map(fsrReading, 0, 1024, 0, 128); // force value mapped from [0, 1024] to [0, 255]

  /******************** Time Events *******************************/
  // Handle any events that are in the queue
  gEM.processEvent();

  // Add events into the queue
  addPinEvents();

}

/******************** Time Events *******************************/
// Add events to toggle NeoPixel every 100 ms
// NOTE:  doesn't handle millis() turnover
void addPinEvents()
{
  if ( ( millis() - lastToggled_LED ) > 1 )
  {
    gEM.queueEvent( EventManager::kEventUser0, 0 );
  }

  if ( ( millis() - lastToggled_NeoPixel ) > 100 )
  {
    gEM.queueEvent( EventManager::kEventUser1, 0 );
  }
}

/*********************** Keypad **************************/
//take care of some special events
void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState()) {
    case PRESSED:
#if DEBUG_KEYPAD
      Serial.print("Pressed: ");
      Serial.println(eKey);
#endif
      switch (eKey) {
        case 'A':
          access_granted();
          break;
        case 'D':
          guestCounter--;
          if (guestCounter < 0) {
            guestCounter = 0;
          }
#if ENABLE_LCD
          lcd.setCursor(14, 1);
          lcd.print(guestCounter);
#endif
          break;
        case '*': checkPassword(); password.reset(); break;
        case '#': password.reset(); break;
        default: password.append(eKey);
      }
  }
}

void checkPassword() {
  if (password.evaluate()) {

    access_granted();

  }
  else {
    access_denied();
  }
}

/******************* LED *****************************/
void setColorRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(LED_red, red);
  analogWrite(LED_green, green);
  analogWrite(LED_blue, blue);
}

/******************* OTHER ROUTINES *****************************/

void partyison() {
  /******************* LCD *****************************/
#if ENABLE_LCD
  lcd.setCursor(0, 0);
  lcd.print("It's a party!   ");
  lcd.setCursor(0, 1);
  lcd.print("              ");
#endif
  /******************* SERVO *****************************/
#if ENABLE_SERVO
  // enable "party on" flag servo
  myServo.attach(servo_pin);
  delay(100);
  myServo.write(SERVO_MIN_ANGLE);
  delay(100);
  myServo.detach();
#endif
}

void partyisover(char *message) {
  /******************* LCD *****************************/
#if ENABLE_LCD
  lcd.setCursor(0, 0);
  lcd.print("Party is over!  ");
  lcd.setCursor(0, 1);
  lcd.print(message);
#endif
  /******************* SERVO *****************************/
#if ENABLE_SERVO
  // enable "no party" flag servo
  myServo.attach(servo_pin);
  delay(100);
  myServo.write(SERVO_MAX_ANGLE);
  delay(100);
  myServo.detach();
#endif
}

void emergency() {
  /******************* BUZZER/LED *****************************/
  // static red LED and buzzer for 1 sec

  setColorRgb(255, 0, 0);
#if ENABLE_BUZZER
  digitalWrite(buzzPin, HIGH);    // sound buzzer
#endif
  delay(1000);
  setColorRgb(255, 0, 0);
#if ENABLE_BUZZER
  digitalWrite(buzzPin, LOW);     // stop buzzer
#endif

}

void access_granted() {
  if (guestCounter < GUESTS_MAX) {
    guestCounter++;
#if DEBUG_KEYPAD
    Serial.println("Success");
#endif
    /******************* LED *****************************/
    // blink green LED for 1 sec @ 10 Hz
    for (int i = 0; i < 10; ++i) {
      setColorRgb(0, 255, 0);
      delay(100);
      setColorRgb(0, 0, 0);
      delay(100);
    }
  }

  ////////// add counter!!!

  if (guestCounter >= GUESTS_PARTY && guestCounter < GUESTS_MAX) {
    partyison();
  }

  if (guestCounter == GUESTS_MAX) {
#if DEBUG_KEYPAD
    Serial.println("Success but Party Full");
#endif
    /******************* LED *****************************/
    // blink blue LED for 1 sec @ 10 Hz
    for (int i = 0; i < 10; ++i) {
      setColorRgb(0, 0, 255);
      delay(100);
      setColorRgb(0, 0, 255);
      delay(100);
    }
    /******************* LCD *****************************/
#if ENABLE_LCD
    lcd.setCursor(0, 0);
    lcd.print("Party is full!  ");
#endif
  }

  /******************* LCD *****************************/
#if ENABLE_LCD
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(14, 1);
  // print the number of seconds since reset:
  lcd.print(guestCounter);
#endif

}

void access_denied() {
#if DEBUG_KEYPAD
  Serial.println("Wrong");
#endif

  /******************* BUZZER/LED *****************************/
  // blink red LED and buzzer for 1 sec @ 10 Hz
  for (int i = 0; i < 10; ++i) {
    setColorRgb(255, 0, 0);
#if ENABLE_BUZZER
    digitalWrite(buzzPin, HIGH);    // sound buzzer
#endif
    delay(100);
    setColorRgb(0, 0, 0);
#if ENABLE_BUZZER
    digitalWrite(buzzPin, LOW);     // stop buzzer
#endif
    delay(100);
  }

}
