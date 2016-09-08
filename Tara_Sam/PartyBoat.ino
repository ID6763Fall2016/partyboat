#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_TCS34725.h>

#include <Adafruit_DotStar.h>
#include <SPI.h>

#define PIN      9
#define N_LEDS_DISCO 7
#define NUMPIXELS 32

#define DATAPIN    10
#define CLOCKPIN   11
Adafruit_DotStar dstrip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

#include <Servo.h> 
int servoPin = 8;
Servo servo;
Servo wheel; 
int pos = 0; 
int angle = 0;   // servo position in degrees 
int increasing = true;

int PressureSensorPin = A0;
bool showDiscoBall = false;
bool rotateServo = false;

//gyro pins
const int xpin = 1;                  // x-axis of the accelerometer
const int ypin = 2;                  // y-axis
const int zpin = 3;                  // z-axis (only on 3-axis models)

    
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS_DISCO, PIN, NEO_GRB + NEO_KHZ800);

//You can have fun here changing the colors for the code
uint32_t color1 = strip.Color(236, 79, 100); //Salmon Pink
uint32_t color2 = strip.Color(0, 216, 180); //Cream
uint32_t color3 = strip.Color(274, 0, 208); //Lavendar
uint32_t color4 = strip.Color(182, 131, 0); //Red
uint32_t color5 = strip.Color(0, 244, 0); //Purple
uint32_t color0 = strip.Color(0, 0, 0); //none

void setup() 
{ 
  Serial.begin(9600);
  servo.attach(servoPin); 

  //wheel.attach(9);

  //light sensor setup
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    //while (1);
  }

  //neopixel setup
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //dot star setup
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  dstrip.begin(); // Initialize pins for output
  dstrip.show();  // Turn all LEDs off ASAP
} 

int      head  = 0, tail = 0, dotstarLength = 6; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)
 
void loop() 
{ 

//force sensor
  int force = analogRead(PressureSensorPin);
  //Serial.print("\nforce\t");
  //Serial.println(force);
  if(force>100)
  {
    showDiscoBall = true;
    rotateServo = true;
  }
  else
  {
    showDiscoBall = false;
    rotateServo = false;
  }

//disco ball rotate
  if(rotateServo == true)
  {
      if(angle==180)
        increasing = 0;
    
      if(angle==0)
        increasing = 1;
    
      if (increasing)
        angle+=5;
      else
        angle-=5;
      
      servo.write(angle);
  }

  if(showDiscoBall == true)
  {
    strip.setPixelColor(1, color3); 
    strip.setPixelColor(2, color2); 
    strip.setPixelColor(3, color3); 
    strip.setPixelColor(4, color4); 
    strip.setPixelColor(5, color5); 
    strip.setPixelColor(6, color2); 
    strip.setPixelColor(0, color1); 
    strip.show();
  }
  else
  {
    strip.setPixelColor(1, color0); 
    strip.setPixelColor(2, color0); 
    strip.setPixelColor(3, color0); 
    strip.setPixelColor(4, color0); 
    strip.setPixelColor(5, color0); 
    strip.setPixelColor(6, color0); 
    strip.setPixelColor(0, color0); 
    strip.show();
  }

  //gyro sensor
  int x = analogRead(xpin);  //read from xpin
 
  int y = analogRead(ypin);  //read from ypin
 
  int z = analogRead(zpin);  //read from zpin
 
  float zero_G = 512.0; //ADC is 0~1023  the zero g output equal to Vs/2
                        //ADXL335 power supply by Vs 3.3V
  float scale = 102.3;  //ADXL335330 Sensitivity is 330mv/g
                         //330 * 1024/3.3/1000  

  float x1 = ((float)x - 331.5)/65*9.8;
  float y1 = ((float)y - 329.5)/68.5*9.8;
  float z1 = ((float)z - 340)/68*9.8;

  Serial.print(((float)x - 331.5)/65*9.8);  //print x value on serial monitor
  Serial.print("\t");
  Serial.print(((float)y - 329.5)/68.5*9.8);  //print y value on serial monitor
  Serial.print("\t");
  Serial.print(((float)z - 340)/68*9.8);  //print z value on serial monitor
  Serial.print("\n");

  //dotstar code
  dstrip.setPixelColor(head, color); // 'On' pixel at head
  dstrip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  dstrip.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)
  //////////

  if(z1 > 0)
  {
      if(--head <= 0) {
        head = NUMPIXELS;
        if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
          color = 0xFF0000;             //   Yes, reset to red
      }
      tail = head + dotstarLength;
      if(tail <=0) tail = NUMPIXELS;
  }
  ////////////
  else
  {
      if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
        head = 0;                       //  Yes, reset head index to start
        if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
          color = 0xFF0000;             //   Yes, reset to red
      }
      tail = head - dotstarLength;
      if(tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
  }

  //random wheel movement servo
   /* if(pos ==180)
  {
      increasing = false;
      pos+=10
      wheel.write(pos);              // tell servo to go to position in variable 'pos'                      
  }
  else (pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
    pos-=10
    wheel.write(pos);              // tell servo to go to position in variable 'pos'
  }*/

  delay(30);
} 
