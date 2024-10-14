//#define ARDUINO_Seeed_XIAO_nRF52840_Sense

#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 2
#define MOSFET_PIN 5

CRGB leds[NUM_LEDS];

CRGB colors[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Cyan,
  CRGB::Yellow,
  CRGB::Magenta
};

const int numCols = 6;

void setup() {
  pinMode(MOSFET_PIN,OUTPUT);
  // pinMode(10,OUTPUT);
  // digitalWrite(10,LOW);
  digitalWrite(MOSFET_PIN,LOW);
  delay(200);
  digitalWrite(MOSFET_PIN,HIGH);
  delay(100);
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  
  FastLED.clear();
  Serial.begin(115200);

  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  Serial.println("test");
  delay(500);
  for(int i=0;i<numCols;i++){
    FastLED.showColor(colors[i]);
    delay(1000);
    FastLED.showColor(CRGB::Black);
    delay(500);    
  }
}

