#include <Arduino.h>
#include "main.h"
#include "sleep.h"
#include "visuals.h"

extern uint32_t deltaTime;
extern uint32_t timeElapsed;
uint32_t keyDetectElapsed = 0;

void loopFinale();

void initWaitForKey()
{
    Serial.print("initWaitForKey");
    keyDetectElapsed = 0;
    timeElapsed = 0;
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

    if(keyDetectElapsed>500){
        playPatternFinale();
        timeElapsed = 0;
        setLoopFunc(loopFinale);
    }

    if(timeElapsed>INTERACTION_TIMEOUT){
        sleep();        
    }
    timeElapsed+=deltaTime;
    
}

void loopFinale()
{
    timeElapsed+=deltaTime;
    if(timeElapsed>FINALE_DURATION){
        sleep();        
    }
}


