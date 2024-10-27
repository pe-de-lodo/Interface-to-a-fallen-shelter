#include <Arduino.h>
#include <FastLED.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>
#include <visuals.h>
#include <ledpatterns.h>
#include <ledLocationData.h>
#include <patterns.hpp>

CRGB leds[NUM_LEDS];
extern uint32_t deltaTime;
PatternCanvas canvas(leds,ledLocationData,NUM_LEDS);



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
    FastLED.addLeds<WS2812, LED_PIN_1,GRB>(leds, 200);  
    FastLED.addLeds<WS2812, LED_PIN_2,GRB>(leds, 200, 200);  
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


CycleLeds tryDoorKnobPattern(CRGB(0x28,0x8c,0x13));
CycleLeds torchAttratorPattern(CRGB(0x84,0x6d,0x12));
CycleLeds keyAttractorPattern(CRGB(0x55,0x8d,0xd4));

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


