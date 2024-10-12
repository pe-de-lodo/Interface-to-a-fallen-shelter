#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_RGB + NEO_KHZ800);
uint32_t deltaTime;
uint32_t lastLoop;

void (*loopFunc) (void) = NULL;

uint32_t colors[] = {
    pixels.Color(255,0,0),
    pixels.Color(0,255,0),
    pixels.Color(0,0,255),
    pixels.Color(255,255,0),
    pixels.Color(0,255,255),
    pixels.Color(255,0,255),
    pixels.Color(255,255,255)
};


const int numCols = 7;

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

void loop() 
{
  uint32_t time = millis();
  deltaTime = time-lastLoop;
  lastLoop = time;
  if(loopFunc!=NULL){
    loopFunc();  
  }
  else {
    sleep();
  }
}