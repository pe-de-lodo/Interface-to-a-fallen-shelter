#include <FastLED>
#include <ledpatterns.h>

#define LED_PIN = 2
#define NUM_LEDS = 16

CRGB *leds;
ledData *data;

PatternCanvas canvas = PatternCanvas(leds,data,NUM_LEDS);

tweenData tweenData;

long lastUpdateTime = 0; 

CRGB blinkPatter(ledData,tweenData){
    return CHSV(0,0,0);
}

void setup()
{
    canvas.TransitionToPattern(blinkPatter,0,tweenData);
}

void loop()
{
    long frameDuration = 33;
    long updateStartTime = millis();
    canvas.Update(lastUpdateTime - updateStartTime);
    FastLED.show();
    lastUpdateTime = updateStartTime;
    long elapsed = millis()-updateStartTime; 
    delay(frameDuration-elapsed);    
}