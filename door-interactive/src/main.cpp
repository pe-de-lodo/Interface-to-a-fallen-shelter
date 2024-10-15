#include <Arduino.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>
#include <light_comms.h>
#include <FastLED.h>

CRGB leds[NUM_LEDS];
uint32_t deltaTime;
uint32_t lastLoop;

void (*loopFunc) (void) = NULL;

void setup() 
{
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);  
  FastLED.clear();
  configSleep();
  setLoopFunc(initKnock);
  lastLoop=millis();
}

void loop() 
{
  calcDeltaTime();
  if(loopFunc!=NULL){
    loopFunc();  
  }
  else {
    sleep();
  }
}

void setLoopFunc(void func ())
{
  loopFunc = func;
}

inline void calcDeltaTime()
{
  uint32_t time = millis();
  deltaTime = time-lastLoop;
  lastLoop = time;
}