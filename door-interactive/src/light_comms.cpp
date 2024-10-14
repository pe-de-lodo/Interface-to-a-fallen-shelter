#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <CircularBuffer.hpp>
#include <main.h>
#include <light_comms.h>

CircularBuffer<int,512> buffer;
extern Adafruit_NeoPixel pixels;

const double ease = 0.001; //out of 255
double minVal=INT16_MAX;
double maxVal=0;
double average = 0;
#define LIGHT_SENSOR_PIN A3

const uint32_t shortPulse = 200;
const uint32_t longPulse = 600;

const uint32_t pattern[] = {
    shortPulse, shortPulse, shortPulse, longPulse, longPulse
};

void playPattern()
{
    
}

void listenForPattern()
{

}

void initLightComms(){
    pinMode(LIGHT_SENSOR_PIN,INPUT);
    setLoopFunc(lightCommsLoop);
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
    uint32_t color = isHigh ? pixels.Color(0,255,0) : pixels.Color(255,0,0);
    pixels.setPixelColor(0,color);
    pixels.show();
    Serial.print(isHigh?"HIGH ":"LOW  ");
    Serial.print(minVal);
    Serial.print(", ");
    Serial.print(maxVal);
    Serial.print(", ");
    Serial.print(average);
    Serial.print(", ");
    Serial.print(val);
    Serial.println();
    delay(10);
}