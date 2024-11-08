#include <Arduino.h>
#include "main.h"
#include "sleep.h"
#include "visuals.h"
#include "keys.h"

extern uint32_t deltaTime;
extern uint32_t timeElapsed;
uint32_t keyDetectElapsed = 0;

void initWaitForKey()
{
    Serial.print("initWaitForKey");
    keyDetectElapsed = 0;
    timeElapsed = 0;
    setLoopFunc(loopWaitForKey);
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
        timeElapsed = 0;
        setLoopFunc(loopFinale);
    }

    if(timeElapsed>INTERACTION_TIMEOUT){
        sleep();        
    }
    timeElapsed+=deltaTime;
    
}

void initFinale()
{
    timeElapsed = 0;
    Serial.println("play finale");
    setLoopFunc(loopFinale);
}


int finalePhase = 0;
void (*finaleFunctionArray[])() =   {playPatternFinale_1, playPatternFinale_3, playPatternFinale_4, playPatternFinale_5, playPatternFinale_6, playPatternFinale_7, sleep};
uint32_t finaleDuration[] = {0, 40000, 8000, 20000, 10000, 3000, 20000};

void loopFinale()
{
    //playPatternFinale();
    timeElapsed+=deltaTime;

    if(timeElapsed > finaleDuration[finalePhase])
    {
        finaleFunctionArray[finalePhase]();

        timeElapsed = 0;
        finalePhase++;
    }
}


