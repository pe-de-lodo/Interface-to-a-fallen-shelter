#include <Arduino.h>
#include <FastLED.h>
#include <CircularBuffer.hpp>
#include "main.h"
#include "sections/light_comms.h"
#include "visuals.h"
#include "keys.h"
#include "sleep.h"

CircularBuffer<int,512> buffer;

const double ease = 0.0001; //out of 255
double minVal=INT16_MAX;
double maxVal=0;
double average = 0;

const uint32_t shortPulse = 200;
const uint32_t longPulse = 600;

const uint32_t pattern[] = {
    shortPulse, shortPulse, shortPulse, longPulse, longPulse
};

uint32_t duration = 0;
extern uint32_t deltaTime;
extern uint32_t timeElapsed;

void playPattern()
{
    
}

void listenForPattern()
{

}

void initLightComms(){
    Serial.println("initLightComms");

    playPatternTorchAttractor(); 
    setLoopFunc(lightCommsLoop);

    duration = 0;
    timeElapsed = 0;
}

void lightCommsLoop(){
    uint32_t val = analogRead(LIGHT_SENSOR_PIN);
    buffer.push( val );
    
    average=average + ease*(val-average);
    if(val>maxVal){
        maxVal=val;
    }
    if(val<minVal){
        minVal=val;
    }
    
    
    minVal = minVal+ease*(average-minVal);
    maxVal = maxVal+ease*(average-maxVal);

    bool dark = val > (minVal+(maxVal-minVal)/2);
    if((maxVal-minVal)<200){
        dark = true;
    }

    //rising edge
    if(!dark && duration==0){
        playPatternTorchDetected();        
    }

    //falling edge
    if(dark && duration>0){
        playPatternTorchAttractor();
    }

    if(dark){
        duration=0;
    }
    else {
        duration += deltaTime;
    }

    
    timeElapsed += deltaTime;

    if(timeElapsed>INTERACTION_TIMEOUT){
        sleep();
    }

    if(duration>1000){
        
        setLoopFunc(initFinale);
    }
    

}