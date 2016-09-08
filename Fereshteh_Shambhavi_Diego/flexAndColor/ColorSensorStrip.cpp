/***************************************
 ** Color Sensor and Strip            **
 ***************************************/

// include this library's description file
#include "ColorSensorStrip.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

ColorSensorStrip::ColorSensorStrip()
{
}
  
void ColorSensorStrip::setupColorSensor() {
  Wire.begin();

  // When turn on the color sensor LED, ledStatus = 1; When turn off the color sensor LED, ledStatus = 0.
  colorSensor.ledStatus = 1;
}

void ColorSensorStrip::setupStrip() {
  // initialize this instance's variables
  strip.begin();
  strip.show(); 
}

void ColorSensorStrip::readColorSensor(int *red, int *green, int *blue)
{
  colorSensor.readRGB(red, green, blue);   //Read RGB values to variables.
  delay(300);
  colorSensor.clearInterrupt();
}

void ColorSensorStrip::writeLEDStripColor(int red, int green, int   blue) {
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  if (DEBUG) {
    Serial.print(inches);
    Serial.print("in, ");
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
  
    Serial.print("The RGB value are: RGB( ");
    Serial.print(red,DEC);
    Serial.print(", ");
    Serial.print(green,DEC);
    Serial.print(", ");
    Serial.print(blue,DEC);
    Serial.println(" )");
  }

  strip.setPixelColor(stripHead, green, red, blue); // 'On' pixel at head
  strip.setPixelColor(stripTail, 0);     // 'Off' pixel at tail
  strip.show();                         // Refresh strip
  delay(cm * 2);                        // Pause 20 milliseconds (~50 FPS)

  if(++stripHead >= NUMPIXELS) {         // Increment head index.  Off end of strip
      stripHead = 0;
  }
  if(++stripTail >= NUMPIXELS)
    stripTail = 0; // Increment, reset tail index
}//end of loop

long ColorSensorStrip::microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long ColorSensorStrip::microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void ColorSensorStrip::setup() {
  setupColorSensor();
  setupStrip();
}

void ColorSensorStrip::loop() {
  // Reads color sensor and store value on strip color variables
  readColorSensor(&stripColorRed, &stripColorGreen, &stripColorBlue);

  // Writes color to led strip
  writeLEDStripColor(stripColorRed, stripColorGreen, stripColorBlue);
}

