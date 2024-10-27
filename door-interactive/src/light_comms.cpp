#include <Arduino.h>
#include <FastLED.h>
#include <CircularBuffer.hpp>
#include <main.h>
#include <light_comms.h>
#include <visuals.h>
#include <keys.h>

CircularBuffer<int,512> buffer;

const double ease = 0.001; //out of 255
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

void playPattern()
{
    
}

void listenForPattern()
{

}

void initLightComms(){
    Serial.print("initLightComms");

    playPatternKeyAttractor();
    pinMode(LIGHT_SENSOR_PIN,INPUT_PULLUP);
    setLoopFunc(lightCommsLoop);

    duration = 0;
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

    bool isHigh = val > (minVal+(maxVal-minVal)/2);
    if((maxVal-minVal)<50){
        isHigh = true;
    }
    if(isHigh){
        duration=0;
    }
    else {
        duration += deltaTime;
    }

    if(duration>1000){
        setLoopFunc(initWaitForKey);
    }
    
    // CRGB color = isHigh ? CRGB::Green : CRGB::Red;
    // FastLED.showColor(color);
    // Serial.print(isHigh?"HIGH ":"LOW  ");
    // Serial.print(minVal);
    // Serial.print(", ");
    // Serial.print(maxVal);
    // Serial.print(", ");
    // Serial.print(average);
    // Serial.print(", ");
    // Serial.print(val);
    // Serial.println();
}