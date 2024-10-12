#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_RGB + NEO_KHZ800);
uint32_t deltaTime;
uint32_t lastLoop;

void (*loopFunc) (void) = NULL;

void setup() 
{
  pixels.begin(); 
  configSleep();
  setLoopFunc(initKnock);
  lastLoop=millis();
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