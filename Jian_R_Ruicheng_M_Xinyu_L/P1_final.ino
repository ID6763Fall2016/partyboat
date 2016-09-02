#include <Servo.h>

#include <Adafruit_DotStar.h>
#include <SPI.h>

#define NUMPIXELS 82
#define DATAPIN 4
#define CLOCKPIN 5
Adafruit_DotStar strip =
    Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

int sonar_Pin = A0;
int sonarValue = 0;
int vib_Pin = 6;
Servo servo0;  // create servo object to control a servo

int pos = 0;  // initial pos of servo

int flex1 = A1;  // select the input pin for the potentiometer
int flex2 = A2;

int servo0_Pin = 9;  // select the pin for the LED
int servo1_Pin = 10;

int flexValue1 = 0;  // init the value coming from the flex sensors
int flexValue2 = 0;

int temp0 = 180;
int temp1 = 180;
int angle0 = 180;
int angle1 = 180;
int angle = 0;

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(9600);
  strip.begin();
  strip.show();
  pinMode(vib_Pin, OUTPUT);
  servo0.attach(
      servo0_Pin);  // attaches the servo on pin 9 to the servo0 object
}

int head = 0, tail = -65;  // Index of first 'on' and 'off' pixels
int len = head - tail;
uint32_t color = 255;  // 'On' color (starts red)
int measOn = 0;

void ledCtrl() {
  strip.setBrightness(50);
  for (int i = 1; i <= len; i++) {
    float colori = color / (len) * (len - i);
    strip.setPixelColor(head + 1 - i, colori / 1.3, colori, colori / 6);
  }
  strip.show();
  delay(2);

  if (++head >= NUMPIXELS + len) {
    head = 0;
  }
  if (++tail >= NUMPIXELS - len) tail = -len;
}

void ledOff() {
  for (int i = NUMPIXELS; i >= 0; i--) {
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
    delay(20);
  }
}

int sonarCtrl() {
  int s;
  s = analogRead(sonar_Pin) * 5;
  Serial.print("distance: ");
  Serial.print(s);
  Serial.println(" mm");
  return s;
}

void servoCtrl() {
  // read the value from the sensors:
  flexValue1 = analogRead(flex1);
  flexValue2 = analogRead(flex2);

  // print sensor read for monitoring
  Serial.print("flex1 degree: ");
  Serial.print(flexValue1);
  Serial.print("  flex2 degree: ");
  Serial.println(flexValue2);

  // flex value to servo1 angle mapping
  if (flexValue1 <= 300) {
    angle0 = map(flexValue1, 115, 300, 0, 90);
  } else if (flexValue1 >= 310) {
    angle0 = map(flexValue1, 310, 340, 90, 180);
  } else {
    angle0 = 90;
  }

  if (flexValue2 <= 205) {
    angle1 = map(flexValue2, 50, 205, 0, 90);
  } else if (flexValue2 >= 210) {
    angle1 = map(flexValue2, 215, 300, 90, 180);
  } else {
    angle1 = 90;
  }

  if (angle0 < 0) {
    angle++;
  }
  if (angle0 > 50) {
    angle--;
  }

  if (angle > 180) angle = 180;
  if (angle < 0) angle = 0;
  Serial.println(angle);
  servo0.write(angle);
  delay(50);

  temp0 = angle0;
  temp1 = angle1;
}

void startCtrl() {
  int measValue = analogRead(A5);
  Serial.println("Game_Monitoring");
  Serial.println(measValue);

  if (measValue == 0) {
    Serial.println("Game_Start");
    analogWrite(vib_Pin, 255);
    delay(1000);
    analogWrite(vib_Pin, 0);
  }
}

void loop() {
  startCtrl();
  servoCtrl();
  sonarValue = sonarCtrl();

  if (sonarValue <= 300) {
    for (int i = 0; i <= NUMPIXELS * 5; i++) {
      ledCtrl();
    }
    ledOff();
    delay(100);
  }
}
