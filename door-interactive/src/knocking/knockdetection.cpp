#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>
#include <CircularBuffer.hpp>
#include <visuals.h>

extern Adafruit_NeoPixel pixels;

const uint32_t shortInterval = 600;
const uint32_t longInterval = 1200;
const uint32_t timeout = 30000;

uint32_t intervalTypes[] = {
    shortInterval,
    longInterval,
    timeout
};

uint32_t intervalColors[3];

int numIntervalTypes = 3;

unsigned long lastKnock;

CircularBuffer<byte, 10> recordedIntervalTypes;
CircularBuffer<long, 10> recordedIntervals;

int knockThreshold = 200;


void initKnock()
{
    setLoopFunc(listenForKnock);

    intervalColors[0] = pixels.Color(0,255,0);
    intervalColors[1] = pixels.Color(150,0,0);
    intervalColors[2] = pixels.Color(0,0,0);
    
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(150,0,150));
    pixels.show();

    Serial.begin(115200);

    pinMode(A0,INPUT);
}

void listenForKnock()
{
    // pixels.clear();
    // pixels.setPixelColor(0, pixels.Color(255,0,255));
    // pixels.show();
    //delay(1000);
    uint32_t time = millis();
    uint32_t interval = time-lastKnock;
        
    long val =  analogRead(A0);
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

    pixels.setPixelColor(0, intervalColors[intervalType] );
    pixels.show();  
    delay(20);
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.show();

    if(intervalType==0){
        setLoopFunc(celebrate1);
    }
    
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
