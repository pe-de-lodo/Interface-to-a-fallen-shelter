#include <Arduino.h>
#include <FastLED.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>
#include <visuals.h>
#include <ledpatterns.h>
#include <ledLocationData.h>
#include <patterns.hpp>

extern CRGB leds[NUM_LEDS];
extern uint32_t deltaTime;
PatternCanvas canvas(leds,ledLocationData,NUM_LEDS);


CRGB leds[NUM_LEDS];
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
DEFINE_GRADIENT_PALETTE( alarmAttractorGradient_p ) {
     0,    0,   0,   0,  /* at index 0,   black(0,0,0) */
    192, 255,   0,   0,  /* at index 192, red(255,0,0) */
    255, 0, 0, 0   /* at index 255, white(255,255,255) */
};
CRGBPalette16 alarmAttractorGradient = alarmAttractorGradient_p;

void initVisuals()
{
    FastLED.addLeds<WS2812, LED_PIN_1,GRB>(leds, 250);
      
    //FastLED.addLeds<WS2812, LED_PIN_2,GRB>(leds, 250, 250);  
    FastLED.clear();
}

Ripples waitForKnockPattern;
void waitForKnockVisuals()
{
    canvas.TransitionToPattern(&waitForKnockPattern,500);
}

void alarmAttractor()
{
    Serial.println("playing alarm attractor");

    elapsed = 0;
    setLoopFunc(loopAlarmAttractor);
    canvas.Clear();
    FastLED.clear();
}

void loopAlarmAttractor()
{
    leds[0] = ColorFromPalette(alarmAttractorGradient,(uint8_t)(millis()*3*255/1000));
    FastLED.show();
    elapsed+=deltaTime; 
    if(digitalRead(WAKEUP_PIN)==HIGH){
        setLoopFunc(initKnock);
        return;
    }
    if(elapsed>30000){
        sleep();
    }
}


SolidColorPattern tryDoorKnobPattern(CRGB(0x28,0x8c,0x13));
SolidColorPattern torchAttratorPattern(CRGB(0x84,0x6d,0x12));

void playPatternTryDoorKnobPattern()
{
    canvas.TransitionToPattern(&tryDoorKnobPattern,500);
}

void playPatternTryDoorKnob()
{
    
}

