#include <Arduino.h>
#include <FastLED.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>
#include <visuals.h>

extern CRGB leds[NUM_LEDS];
extern uint32_t deltaTime;

int32_t elapsed;

CRGB colors[] = {
  CRGB::Red,
  CRGB::Green,
  CRGB::Blue,
  CRGB::Cyan,
  CRGB::Yellow,
  CRGB::Magenta
};

const int numCols = 7;


void celebrate1()
{
    elapsed=0;
    setLoopFunc(celebrate1Loop);
}

void celebrate1Loop()
{
    elapsed+=deltaTime;    
    FastLED.showColor(CRGB(255,0,255));
    delay(100);
    FastLED.showColor(CRGB::Black);
    delay(100);
    if(elapsed>=5000L){
        sleep();
    }
}