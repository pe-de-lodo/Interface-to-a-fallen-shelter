#include <FastLED>
#include <ledpatterns.h>


#define NUM_LEDS = 16

CRGB *leds;
ledData *data;

PatternCanvas canvas = PatternCanvas(leds,data,NUM_LEDS);

tweenData tweenData;

CRGB blinkPatter(ledData,tweenData){
    return CHSV(0,0,0);
}

void setup()
{
    canvas.TransitionToPattern(,0,tweenData)
}

void loop()
{

}