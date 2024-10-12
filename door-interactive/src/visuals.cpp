#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>
#include <visuals.h>

extern Adafruit_NeoPixel pixels;
extern uint32_t deltaTime;

int32_t elapsed;

void celebrate1()
{
    elapsed=0;
    setLoopFunc(celebrate1Loop);
}

void celebrate1Loop()
{
    elapsed+=deltaTime;    
    pixels.setPixelColor(0,pixels.Color(128,0,128));
    pixels.show();
    delay(100);
    pixels.setPixelColor(0,pixels.Color(0,255,0));
    pixels.show();
    delay(100);
    if(elapsed>=5000L){
        sleep();
    }
}