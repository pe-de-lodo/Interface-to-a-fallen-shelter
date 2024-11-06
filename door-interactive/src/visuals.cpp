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

CycleLeds alarmAttractorPattern(CRGB(0xd4,0x00,0x00),16); //reddish
CycleLeds waitForKnockPattern(CRGB(0xcc,0x10,0xd4),16); //magenta
CycleLeds tryDoorKnobPattern(CRGB(0x28,0x8c,0x13),16); //greenish
CycleLeds torchAttratorPattern(CRGB(0x84,0x6d,0x12),16); //dim yellow
CycleLeds keyAttractorPattern(CRGB(0x55,0x8d,0xd4),16); //blue
MaskedPattern maskedPattern;
Ripples finalePattern;
PulsePattern pulsePattern(2000);
NoisePattern noisePattern(300000, NUM_LEDS, 0.5, 0.5, 100);
GlitchPattern glitchPattern(50, 10, 50, NUM_LEDS);

bool sendVisualsOverUart = false;

AbstractPattern* patternArray[] = {&pulsePattern, &glitchPattern, &noisePattern, &torchAttratorPattern, &keyAttractorPattern, &finalePattern, &blankPattern, &meteorPattern};

void initVisuals()
{
    FastLED.addLeds<WS2812, LED_PIN_1,GRB>(leds, 270);  
    FastLED.addLeds<WS2812, LED_PIN_2,GRB>(leds, 270, 270);  
}

void updateVisuals()
{
  canvas.Update(deltaTime);

  if( sendVisualsOverUart ){
    sendPixelsUart((char*)leds,NUM_LEDS*sizeof(CRGB));
  }
  FastLED.show();

}


void playPattern(int ptrnIndex, long duration)
// void playPattern(int ptrnIndex, long duration = 500) //Why doesn't the default argument work?
{
    if(ptrnIndex <= 0)
        canvas.Clear();
    else {
        int patternBounds = sizeof(patternArray) / sizeof(AbstractPattern*);
        if(ptrnIndex <= patternBounds)
            canvas.TransitionToPattern(patternArray[ptrnIndex-1], duration);
        else
        {
            Serial.print("Pattern index out of bounds. Num of patterns is: ");
            Serial.println(patternBounds);
        }
    }
}

void playMaskedPattern(int offset, int num)
{
    maskedPattern.Set(CHSV(random(255), 255, 255), offset, num);
    canvas.TransitionToPattern(&maskedPattern, 500);
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
