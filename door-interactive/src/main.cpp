#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_I2CDevice.h>
#include <SdFat.h>
#include <Wire.h>
#include "main.h"
#include "sleep.h"
#include "sections/knockdetection.h"
#include "light_comms.h"
#include "alarm.h"
#include "visuals.h"
#include "ledpatterns.h"
#include <SimpleSerialShell.h>
#include "shell_commands.h"

uint32_t deltaTime;
uint32_t lastLoop;

void (*loopFunc) (void) = NULL;

extern PatternCanvas canvas;

void setup() 
{
  Serial.begin(115200);
  //while(!Serial && millis()<5000) delay(5);
  shell.attach(Serial);
  Serial.println("INIT");
  
  enablePeripherals();
  delay(20);
  bool wokeFromAlarm = initWakeAlarm();
  configSleep();
  initVisuals();

  if(wokeFromAlarm){
    setLoopFunc(initAlarmAttractor);
  } {
    setLoopFunc(initKnock);
  }
  
  addCommands();

  lastLoop=millis();
}

void loop() 
{
  calcDeltaTime();


  if(loopFunc!=NULL){
    loopFunc();
  }
  shell.executeIfInput();
  
  canvas.Update(deltaTime);
}

void setLoopFunc(void func ())
{
  loopFunc = func;
}

inline void calcDeltaTime()
{
  uint32_t time = millis();
  deltaTime = time-lastLoop;
  lastLoop = time;
}