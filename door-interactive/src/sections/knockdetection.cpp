#include <Arduino.h>
#include <FastLED.h>
#include <CircularBuffer.hpp>
#include "main.h"
#include "sections/knockdetection.h"
#include "sleep.h"
#include "visuals.h"
#include "sections/light_comms.h"

const uint32_t shortInterval = 2000;
const uint32_t longInterval = 4000;
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
int knockThreshold = 95;//1024*knockThreshVolts/3.3f

int knobHeldTime = 1500;

long knockInteractionTimeout=0;


void initKnock()
{
    Serial.println("initKnock");
    lastKnock = max((int32_t)millis()-(int32_t)longInterval,0);
    delay(5); // fix for mysterious voltage spike on MCU ADC min on power up
    setLoopFunc(listenForKnock);
    waitForKnockVisuals();
    pinMode(KNOCK_PIN,INPUT);
}

int samplePiezo(){
    int val=0;
    //TODO implement a sub 1khz low pass filter here to eliminate power draw noise from ws2812 strips.
    for(int i=100;i>0;i--){
        val=max(val,max(0,(int32_t)analogRead(KNOCK_PIN)));
    }
    return val;
}

void listenForKnock()
{
    uint32_t time = millis();
    uint32_t interval = time-lastKnock;
    

    long val = samplePiezo();
    
    if(interval>timeout){
        sleep();        
    }
    if(knockInteractionTimeout>0){
        knockInteractionTimeout-=deltaTime;
        if(knockInteractionTimeout<0){
            knockInteractionTimeout=0;
        }
        return;
    }
    if(val>knockThreshold){
        knockPattern();
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
        knockInteractionTimeout = 50;
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
    
    FastLED.showColor(CRGB::Black);

    if(intervalType==0){
        playPatternTryDoorKnob();
        timeElapsed = 0;
        Serial.println("waitForDoorKnobTouch");
        setLoopFunc(waitForDoorKnobTouch);
    }
    
}


long doorKnobHeldElapsed = 0;
void waitForDoorKnobTouch()
{

    bool isHeld = digitalRead(DOOR_KNOB);


    //rising edge
    if(isHeld && doorKnobHeldElapsed==0 ){
        playPatternDoorKnobHeld();
    }

    //falling edge
    if(!isHeld && doorKnobHeldElapsed>0){
        playPatternTryDoorKnob();
    }

    if(isHeld){
        doorKnobHeldElapsed += deltaTime;
    }
    else {
        doorKnobHeldElapsed = 0;
    }


    if(doorKnobHeldElapsed > knobHeldTime){
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
