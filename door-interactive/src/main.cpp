#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_I2CDevice.h>
#include <SdFat.h>
#include <Wire.h>
#include "main.h"
#include "sleep.h"
#include "knocking/knockdetection.h"
#include "light_comms.h"
#include "alarm.h"
#include "visuals.h"
#include "ledpatterns.h"

uint32_t deltaTime;
uint32_t lastLoop;

void (*loopFunc) (void) = NULL;

extern PatternCanvas canvas;

void setup() 
{
  Serial.begin(115200);
  while(!Serial) delay(5);
  Serial.println("INIT");
  enablePeripherals();
  delay(20);
  bool wokeFromAlarm = initWakeAlarm();
  configSleep();
  initVisuals();

  if(wokeFromAlarm){
    setLoopFunc(alarmAttractor);
  } {
    setLoopFunc(initKnock);
  }
  
  lastLoop=millis();
}

void loop() 
{
  calcDeltaTime();

  if(loopFunc!=NULL){
    loopFunc();
  }
  else {
    sleep();
  }
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