#include <Arduino.h>
#include <CircularBuffer.h>
#include <FastLED.h>

CircularBuffer<int,512> buffer;
extern CRGB leds[1];

void lightCommsLoop(){
    buffer.push( analogRead(A2) );
    int min=INT16_MAX;
    int max=0;
    for(int i=0;i<buffer.size();i++){
        int val=buffer[i];
        
        if(val>max){
            max=val;
        }
        if(val<min){
            min=val;
        }

    }
    bool isHigh = buffer.last() > (min+(max-min)/2);
    if((max-min)<50){
        isHigh = true;
    }
    leds[0] = isHigh ? CRGB::Red : CRGB::Green;
    Serial.print(isHigh?"HIGH ":"LOW  ");
    Serial.print(min);
    Serial.print(",");
    Serial.print(max);
    Serial.print(",");
    Serial.print(buffer.last());
    Serial.println();
    delay(100);
}