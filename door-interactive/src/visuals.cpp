#include <Arduino.h>
#include <FastLED.h>
#include "main.h"
#include "sections/knockdetection.h"
#include "sleep.h"
#include "visuals.h"
#include "ledpatterns.h"
#include "led_location_data.h"
#include "patterns.hpp"
#include "send_pixels_uart.h"


CRGB leds[NUM_LEDS];
extern uint32_t deltaTime;
PatternCanvas canvas(leds,ledLocationData,NUM_LEDS);

CycleLeds alarmAttractorPattern(CRGB(0xd4,0x62,0x55),NUM_LEDS); //reddish
CycleLeds waitForKnockPattern(CRGB(0xcc,0x55,0xd4),NUM_LEDS); //magenta
CycleLeds tryDoorKnobPattern(CRGB(0x28,0x8c,0x13),NUM_LEDS); //greenish
CycleLeds torchAttratorPattern(CRGB(0x84,0x6d,0x12),NUM_LEDS); //dim yellow
CycleLeds keyAttractorPattern(CRGB(0x55,0x8d,0xd4),NUM_LEDS); //blue
Ripples finalePattern;
bool sendVisualsOverUart = false;

void initVisuals()
{
    FastLED.addLeds<WS2812, LED_PIN_1,GRB>(leds, 200);  
    FastLED.addLeds<WS2812, LED_PIN_2,GRB>(leds, 200, 200);  
}

void updateVisuals()
{
  canvas.Update(deltaTime);

  if( sendVisualsOverUart ){
    sendPixelsUart((char*)leds,NUM_LEDS*sizeof(CRGB));
  }
  FastLED.show();

}

void playPatternAlarmAttractorPattern()
{
    canvas.TransitionToPattern(&alarmAttractorPattern,500);
}

void waitForKnockVisuals()
{
    canvas.TransitionToPattern(&waitForKnockPattern,500);
}

void playPatternTryDoorKnobPattern()
{
    canvas.TransitionToPattern(&tryDoorKnobPattern,500);
}

void playPatternTryDoorKnob()
{
    canvas.TransitionToPattern(&tryDoorKnobPattern,500);
}

void playPatternTorchAttractor()
{
    canvas.TransitionToPattern(&torchAttratorPattern,500);
}

void playPatternKeyAttractor()
{
    canvas.TransitionToPattern(&keyAttractorPattern,500);
}

void playPatternFinale()
{
    canvas.TransitionToPattern(&finalePattern,500);    
}
