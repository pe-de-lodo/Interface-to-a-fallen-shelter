#include <FastLED.h>
#include <ledpatterns.h>
#include <Tween.h>
#include <structData.h>
#include <sendpixelsUart.h>
//#include <sendPixelsEthernet.h>
#include <patterns.hpp>

#define LED_PIN A3
#define NUM_LEDS 450
#define USE_W5100                           false
#define CUR_PIN_MISO              20
#define CUR_PIN_MOSI              23
#define CUR_PIN_SCK               22
#define CUR_PIN_SS                21
#define USE_THIS_SS_PIN           21

CRGB leds[NUM_LEDS];
ledData *data = structData;

PatternCanvas canvas(leds,data,NUM_LEDS);

long lastUpdateTime = 0; 


Ripples ripplePattern;
BlinkPattern blinkPattern;
BlankPattern blankPattern;


void setup()
{
    FastLED.addLeds<WS2812, 25, GRB>(leds, 1);
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, 1, NUM_LEDS-1);
    FastLED.showColor(CRGB(255,0,255));

    Serial.begin(921600);
    while(!Serial){

    }
    delay(1000);
    
    //initSendPixelsEthernet();
    initSendPixelsUart();

    canvas.TransitionToPattern(&blinkPattern,0);
    canvas.TransitionToPattern(&blankPattern,4000);
    canvas.TransitionToPattern(&ripplePattern,4000);
    
}

void loop()
{
    long frameDuration = 33;
    long updateStartTime = millis();
    canvas.Update(updateStartTime-lastUpdateTime);
    FastLED.show();
    lastUpdateTime = updateStartTime;
    long elapsed = millis()-updateStartTime; 
    delay(frameDuration-elapsed); 
    sendPixelsUart((char*)leds,NUM_LEDS*sizeof(CRGB));
    //sendPixelsEthernet((char*)leds,NUM_LEDS*sizeof(CRGB));
}

