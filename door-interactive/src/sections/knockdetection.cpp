#include <Arduino.h>
#include <FastLED.h>
#include <CircularBuffer.hpp>
#include "main.h"
#include "sections/knockdetection.h"
#include "sleep.h"
#include "visuals.h"
#include "light_comms.h"

const uint32_t shortInterval = 1000;
const uint32_t longInterval = 2000;
const uint32_t timeout = INTERACTION_TIMEOUT;

extern uint32_t deltaTime;
uint32_t timeElapsed = 0;

uint32_t intervalTypes[] = {
    shortInterval,
    longInterval,
    timeout
};

CRGB intervalColors[3];

int numIntervalTypes = 3;

unsigned long lastKnock;

CircularBuffer<byte, 10> recordedIntervalTypes;
CircularBuffer<long, 10> recordedIntervals;

float knockThreshVolts = 0.5f;
int knockThreshold = 80;//1024*knockThreshVolts/3.3f



void initKnock()
{
    Serial.println("initKnock");
    lastKnock = max((int32_t)millis()-(int32_t)longInterval,0);
    delay(5); // fix for mysterious voltage spike on MCU ADC min on power up
    setLoopFunc(listenForKnock);
    waitForKnockVisuals();
    
    // intervalColors[0] = CRGB::Green;
    // intervalColors[1] = CRGB::Red;
    // intervalColors[2] = CRGB::Black;
    
    //FastLED.showColor(CRGB(0,150,0));
    
    
    pinMode(KNOCK_PIN,INPUT);
}

void listenForKnock()
{
    // pixels.clear();
    // pixels.setPixelColor(0, pixels.Color(255,0,255));
    // pixels.show();
    //delay(1000);
    uint32_t time = millis();
    uint32_t interval = time-lastKnock;


    
    long val =  analogRead(KNOCK_PIN);
    if(interval>timeout){
        sleep();        
    }
    if(val>knockThreshold){
        float volts = val*3.3f/1024;
        Serial.print(volts);
        Serial.print("v ");
        Serial.print(val);
        Serial.print(" interval ");
        Serial.print(interval);
        Serial.println("ms ");
        recordedIntervals.push(interval);
        lastKnock = time;
        knockDetected(interval);
        
    }
}

void knockDetected(uint32_t interval)
{
    recordedIntervals.push(interval);
    int intervalType = numIntervalTypes-1;
    for(int i=0;i<numIntervalTypes;i++){
        if(interval<intervalTypes[i]){
            intervalType = i;
            break;
        }
    }
    recordedIntervalTypes.push(intervalType);
    recordedIntervals.push(interval);

    Serial.println(intervalType);

    FastLED.showColor(intervalColors[intervalType]);
    delay(50);
    FastLED.showColor(CRGB::Black);

    if(intervalType==0){
        playPatternTryDoorKnobPattern();
        timeElapsed = 0;
        Serial.println("waitForDoorKnobTouch");
        setLoopFunc(waitForDoorKnobTouch);
    }
    
}


long doorKnobHeldElapsed = 0;
void waitForDoorKnobTouch()
{

    bool isHeld = digitalRead(DOOR_KNOB);
    
    if(isHeld){
        doorKnobHeldElapsed += deltaTime;
    }
    else {
        doorKnobHeldElapsed = 0;
    }

    if(doorKnobHeldElapsed > 200){
        //setLoopFunc();
        setLoopFunc(initLightComms);
    }

    if( timeElapsed > timeout ){
        sleep();
    }
    timeElapsed+=deltaTime;

}

// pixels.clear();
//   for(int i=0; i<numCols; i++) { // For each pixel...
//     pixels.setPixelColor(0, colors[i]);
//     pixels.show();  
//     delay(1000); // Pause before next pass through loop
//     pixels.setPixelColor(0, pixels.Color(0,0,0));
//     pixels.show();   // Send the updated pixel colors to the hardware.
//     delay(500); // Pause before next pass through loop
//   }
