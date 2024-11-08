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
    Serial.println("play finale");
    setLoopFunc(loopFinale);
}


int finalePhase = 0;
void (*finaleFunctionArray[])() =   {playPatternFinale_1, playPatternFinale_2, sleep};
uint32_t finaleDuration[] =         {0, 20000, 20000};

void loopFinale()
{
    //playPatternFinale();
    timeElapsed+=deltaTime;

    if(timeElapsed > finaleDuration[finalePhase])
    {
        finaleFunctionArray[finalePhase]();

        timeElapsed -= finaleDuration[finalePhase];
        finalePhase++;
    }
}


