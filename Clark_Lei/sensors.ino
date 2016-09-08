/**
 * PIR Motion Sensor (JST) - SEN-13285 
 *
 *
 **/
 #include <SPI.h>

#include <Adafruit_DotStar.h>

#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <Servo.h>

#define NUMPIXELS 7

#define DATAPIN  4
#define CLOCKPIN 5

#define HUE_MAX  6.0

Adafruit_DotStar strip = Adafruit_DotStar(7, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
int succ_pir_pin = 2; //digital 2
int flexSensorPin = A3; //analog pin 3, 0 is reserved for random seed
int softpotPin = A1;
int led_pin_out = 12;
int vib_in = A2;
int switch_pin = 7;
int servo_pin = 9;

int last_sw = LOW;
bool game_on = false;
int last_angle = -1;
int last_hue = -1;
int hue_cnt = 0;
bool judged = false;

Servo pointer;

void setup(){
  Serial.begin(9600); 
  randomSeed(analogRead(0));
  pointer.write(0);

  pointer.attach(servo_pin);
  pinMode(succ_pir_pin, INPUT);
  pinMode(led_pin_out, OUTPUT);
  pinMode(switch_pin, INPUT);
  last_sw = digitalRead(switch_pin);
  digitalWrite(led_pin_out, LOW);
  digitalWrite(softpotPin, HIGH); //enable pullup resistor
  
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}


void loop(){
  
//  int flexSensorReading = analogRead(flexSensorPin);
//  Serial.println(flexSensorReading);
//  int flex0to100 = map(flexSensorReading, 90, 230, 0, 100);
//  Serial.println(flex0to100);

//  int vib_val= analogRead(vib_in);
//  if(vib_val > 50) {
//    Serial.println(vib_val);
//  }
  int sw = digitalRead(switch_pin);
  if(sw != last_sw) {
    last_sw = sw;
    if(sw) {
      game_on = !game_on;
      if(game_on) {
        // rotate pointer
        int rand_angle = random(45, 300);
        while(-1 != last_angle && last_angle == rand_angle) {
          rand_angle = random(45, 300);
        }
        pointer.write(rand_angle);
        last_angle == rand_angle;
      }
      judged = false; // clear it
      Serial.println(game_on? "Switched on!" : "Switched off!");
    }
  }

  long color = 0;
  if(game_on) {
    int softpotReading = analogRead(softpotPin);
//    Serial.println(softpotReading);
    if(15 <= softpotReading && softpotReading <= 900) {
      float hue = mapfloat(softpotReading, 40, 900, 0, HUE_MAX);
    //  Serial.println(hue);
      if(hue < 0) hue = 0;
      if(hue > 6) hue = 6;
      last_hue = hue;
      // TODO count hue slider hold
      color = HSV_to_RGB(hue, 1.0, 1.0);
    } else if(40 > softpotReading) {
      float gr = mapfloat(softpotReading, 0, 40, 0, 1);
      color = HSV_to_RGB(0, gr, gr);
//      Serial.print("gr = ");Serial.print(gr);Serial.print("sr = ");Serial.println(softpotReading);
    } else {
      float gr = mapfloat(softpotReading, 1024, 900, 0, 1);
      color = HSV_to_RGB(0, gr, gr);
//      Serial.print("gr = ");Serial.print(gr);Serial.print("sr = ");Serial.println(softpotReading);
    }
  }
  for(int i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, color);
      strip.show();
      delay(15);
  }
  int flex_val = analogRead(flexSensorPin);
//  Serial.print("flex_val = ");Serial.println(flex_val);
  if(game_on && !judged) {
      if(flex_val < 70) {
        Serial.println("Success Motion Detected");
        judged = true;
        digitalWrite(led_pin_out, HIGH);
        delay(800); 
        digitalWrite(led_pin_out, LOW);
        delay(800); 
        digitalWrite(led_pin_out, HIGH);
        delay(800); 
        digitalWrite(led_pin_out, LOW);
      } else {
        digitalWrite(led_pin_out, LOW);
      }
  }
}

long HSV_to_RGB( float h, float s, float v ) {
  /* modified from Alvy Ray Smith's site: http://www.alvyray.com/Papers/hsv2rgb.htm */
  // H is given on [0, 6]. S and V are given on [0, 1].
  // RGB is returned as a 24-bit long #rrggbb
  int i;
  float m, n, f;
 
  // not very elegant way of dealing with out of range: return black
  if ((s<0.0) || (s>1.0) || (v<0.0) || (v>1.0)) {
    return 0L;
  }
 
  if ((h < 0.0) || (h > 6.0)) {
    return long( v * 255 ) + long( v * 255 ) * 256 + long( v * 255 ) * 65536;
  }
  i = floor(h);
  f = h - i;
  if ( !(i&1) ) {
    f = 1 - f; // if i is even
  }
  m = v * (1 - s);
  n = v * (1 - s * f);
  switch (i) {
  case 6:
  case 0:
    return long(v * 255 ) * 65536 + long( n * 255 ) * 256 + long( m * 255);
  case 1:
    return long(n * 255 ) * 65536 + long( v * 255 ) * 256 + long( m * 255);
  case 2:
    return long(m * 255 ) * 65536 + long( v * 255 ) * 256 + long( n * 255);
  case 3:
    return long(m * 255 ) * 65536 + long( n * 255 ) * 256 + long( v * 255);
  case 4:
    return long(n * 255 ) * 65536 + long( m * 255 ) * 256 + long( v * 255);
  case 5:
    return long(v * 255 ) * 65536 + long( m * 255 ) * 256 + long( n * 255);
  }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
