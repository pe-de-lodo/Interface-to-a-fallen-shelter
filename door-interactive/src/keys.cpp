#include <Arduino.h>
#include "main.h"


extern uint32_t deltaTime;
uint32_t keyDetectElapsed = 0;

void initWaitForKey()
{
    Serial.print("initWaitForKey");
    keyDetectElapsed = 0;

}

void loopWaitForKey()
{
    bool detect=digitalRead(DOOR_KNOB)==HIGH;
    if(detect){
        keyDetectElapsed+=deltaTime;
    }
    else {
        keyDetectElapsed=0;
    }    
}


