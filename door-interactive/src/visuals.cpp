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
#include "patterns/cycleleds.hpp"
#include "patterns/ripples.hpp"
#include "patterns/glitch.hpp"
#include "patterns/noise.hpp"
#include "patterns/meteor.hpp"
#include "patterns/mask.hpp"
#include "patterns/range.hpp"

CRGB leds[NUM_LEDS];
extern uint32_t deltaTime;
PatternCanvas canvas(leds,ledLocationData,NUM_LEDS);

CycleLeds alarmAttractorPattern(CRGB(0xd4,0x00,0x00),50,1,-1); //reddish
CycleLeds waitForKnockPattern(CRGB(0xcc,0x10,0xd4),50); //magenta
CycleLeds tryDoorKnobPattern(CRGB(0x28,0xac,0x13),100,5,-1); //greenish
CycleLeds torchAttratorPattern(CRGB(0x8d,0x80,0x12),100,10,1); //dim yellow
CycleLeds keyAttractorPattern(CRGB(0x55,0x8d,0xd4),100,20,-1); //blue
Ripples finalePattern;
PulsePattern pulsePattern(2000);
NoisePattern noisePattern(300000, NUM_LEDS, 0.5, 0.5, 100);
GlitchPattern glitchPattern(50, 10, 50, NUM_LEDS);
MeteorPattern meteorPattern;
MaskPattern meteorPatternMasked(SECTION_CRACK_L | SECTION_CRACK_R, meteorPattern );
RangePattern rangePattern;

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

void playRangePattern(int offset, int num)
{
    rangePattern.Set(CHSV(random(255), 255, 255), offset, num);
    canvas.TransitionToPattern(&rangePattern, 500);
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
