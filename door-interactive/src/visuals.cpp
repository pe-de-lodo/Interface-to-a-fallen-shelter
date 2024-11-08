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
#include "patterns/additiveglitch.hpp"
#include "patterns/noise.hpp"
#include "patterns/meteor.hpp"
#include "patterns/mask.hpp"
#include "patterns/range.hpp"
#include "patterns/desintegrate.hpp"
#include "patterns/whitenoise.hpp"
#include "patterns/combine.hpp"
#include "patterns/superblink.hpp"
#include "patterns/chase.hpp"

CRGB leds[NUM_LEDS];
extern uint32_t deltaTime;
PatternCanvas canvas(leds,ledLocationData,NUM_LEDS);

PulsePattern pulsePattern(2000, 0);
NoisePattern noisePattern(60000, NUM_LEDS, 0.5, 0.3, 50);

DesintegratePattern desintegratePattern(10000, NUM_LEDS);
WhiteNoisePattern whiteNoisePattern(10000, NUM_LEDS);

MeteorPattern meteorPattern(0.507,0.537);
MaskPattern meteorPatternMasked(SECTION_CRACK_L | SECTION_CRACK_R, meteorPattern );
RangePattern rangePattern;
MaskPattern sectionMask(0,rangePattern);
CycleLeds magentaCycle(CRGB(0xec,0x10,0xd4),24,2,1);
CycleLeds bluePattern(CRGB(0x55,0x8d,0xd4),100,20,-1); //blue
// MaskPattern rippleMask(SECTION_CRACK_L|SECTION_CRACK_R,ripplesForCombine);
// GlitchPattern glitchPattern(50, 10, 50, NUM_LEDS);
// CombinePattern glitchAndRipples(glitchPattern,ripplesForCombine);

/* ALARM ATTRACTOR */
PulsePattern pulseAttractorPattern(500, 255);
MaskPattern alarmAttractorPattern(SECTION_FLAG, pulseAttractorPattern);


/* WAIT FOR KNOCK & KNOCK PATTERN */
SuperBlinkPattern doorBlinkPattern(100, 2, 300, 2000);
MaskPattern waitForKnockPattern(SECTION_DOOR, doorBlinkPattern);

WhiteNoisePattern doorNoisePattern(100, NUM_LEDS, 127, 127);
MaskPattern knockNoisePatter(SECTION_DOOR,doorNoisePattern);


/* WAIT FOR KNOB & KNOB PATTERN */
int knobHue = random(255);
ChasePattern doorKnobPattern(CHSV(knobHue, 255, 255), 4000, 112, 0, 7);
MaskPattern tryDoorKnobPattern(SECTION_DOOR, doorKnobPattern);

FadePattern knobFadePatter(CHSV(knobHue, 255, 255), 2000, true);
MaskPattern doorKnobHeldPattern(SECTION_DOOR,knobFadePatter);


/* WAIT FOR TORCH & TORCH PATTERN */
PulsePattern torchAttPattern(2000, 0);
MaskPattern torchAttratorPattern(SECTION_DOOR, torchAttPattern);
GlitchPattern torchDetectedPattern(50, 10, NUM_LEDS, 2, NUM_LEDS);


/* FINALE 1 - DESINTEGRATE & RIPPLES */
DesintegratePattern finaleDesintegratePattern(10000, NUM_LEDS);
Ripples ripplesForCombine;
CombinePattern desintegrateIntoRipples(finaleDesintegratePattern,ripplesForCombine);
void playPatternFinale_1()
{
    desintegrateIntoRipples.Start();
    canvas.TransitionToPattern(&desintegrateIntoRipples,50);    // IDEA: Would be nice to have an offset to chain several pattern transitions.
}

/* FINALE 2  - RIPPLES */
// void playPatternFinale_2()
// {
//     canvas.TransitionToPattern(&ripplesForCombine,500);
// }

/* FINALE 3 - RIPPLES & PULSE */
uint8_t pulsingFinaleHue = 160;
PulsePattern pulsingFinalePattern(2000, 255, pulsingFinaleHue);
CombinePattern ripplesPulsing(ripplesForCombine, pulsingFinalePattern);
void playPatternFinale_3()
{
    canvas.TransitionToPattern(&ripplesPulsing,500);
}

/* FINALE 4 - NOISE */
// FadePattern finaleFadePattern(CHSV(pulsingFinaleHue, 255, 255), 2000, true);
NoisePattern finaleNoisePattern(60000, NUM_LEDS, 0.5, 0.3, 50, 135, 50000, 0.2, 0.3, 20000);
void playPatternFinale_4()
{
    finaleNoisePattern.Start();
    canvas.TransitionToPattern(&finaleNoisePattern, 500);
}

/* FINALE 5 - METEOR */
MeteorPattern finaleMeteorPattern(0.507,0.537);
void playPatternFinale_5()
{
    canvas.TransitionToPattern(&finaleMeteorPattern, 500);
}

/* FINALE 6 - METEOR & ADDGLITCH */
// AddGlitchPattern addGlitch(CHSV(0, 0, 0), 100, 10, 50, 3, NUM_LEDS);
GlitchPattern finaleGlitchPattern(50, 10, NUM_LEDS, 2, NUM_LEDS);
CombinePattern finaleMetorGlitch(finaleMeteorPattern, finaleGlitchPattern);
void playPatternFinale_6()
{
    finaleGlitchPattern.Start();
    canvas.TransitionToPattern(&finaleMetorGlitch, 500);
}

/* FINALE 7 - ADDGLITCH */
void playPatternFinale_7()
{
    canvas.TransitionToPattern(&finaleGlitchPattern, 500);
}

// Ripples finalePattern;

// CycleLeds torchAttratorPattern(CRGB(0x8d,0x80,0x12),100,10,1); //dim yellow
// AbstractPattern& torchDetectedPattern = glitchAndRipples;

// CycleLeds keyAttractorPattern(CRGB(0x55,0x8d,0xd4),100,20,-1); //blue

bool sendVisualsOverUart = false;

AbstractPattern* patternArray[] = {&desintegrateIntoRipples, &ripplesPulsing, &finaleNoisePattern, &finaleMeteorPattern, &finaleMetorGlitch, &finaleGlitchPattern, &desintegrateIntoRipples, &ripplesForCombine};

void initVisuals()
{
    FastLED.addLeds<WS2812, LED_PIN_0,GRB>(leds, NUM_LEDS_0);  
    FastLED.addLeds<WS2812, LED_PIN_1,GRB>(leds, NUM_LEDS_0, NUM_LEDS_1);  
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

void highlightSection(int section)
{
    rangePattern.Set(CHSV(random(255), 255, 255), 0, NUM_LEDS);
    sectionMask.SetSection(1<<section);
    canvas.TransitionToPattern(&sectionMask,100);
}

void playPatternAlarmAttractorPattern()
{
    canvas.TransitionToPattern(&alarmAttractorPattern,500);
}

void waitForKnockVisuals()
{
    canvas.TransitionToPattern(&waitForKnockPattern,500);
}

void knockPattern()
{
    canvas.TransitionToPattern(&knockNoisePatter,50);
    canvas.TransitionToPattern(&waitForKnockPattern,50);
}

void playPatternTryDoorKnob()
{
    canvas.TransitionToPattern(&tryDoorKnobPattern,50);
}

void playPatternDoorKnobHeld()
{
    doorKnobHeldPattern.Start();
    canvas.TransitionToPattern(&doorKnobHeldPattern,50);
}

void playPatternTorchAttractor()
{
    canvas.TransitionToPattern(&torchAttratorPattern,50);
}

void playPatternTorchDetected()
{
    canvas.TransitionToPattern(&torchDetectedPattern,50);
}

// void playPatternKeyAttractor()
// {
//     canvas.TransitionToPattern(&keyAttractorPattern,500);
// }


