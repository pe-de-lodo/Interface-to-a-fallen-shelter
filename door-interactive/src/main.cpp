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
uint32_t lastRedraw;
uint32_t redrawDeltaTime;

void (*loopFunc) (void) = NULL;

extern PatternCanvas canvas;

int drawRate = 30;

void setup() 
{
  Serial.begin(115200);
  shell.attach(Serial);
  
  enablePeripherals();
  Serial.println("enablePeripherals");
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
  bool redraw = calcDeltaTime();

  if(Serial){
    shell.executeIfInput();
  }
  
  if(loopFunc!=NULL){
    loopFunc();
  }

  if(redraw){
    deltaTime = redrawDeltaTime;
    updateVisuals();    
  }
}

void setLoopFunc(void func ())
{
  loopFunc = func;
}

inline bool calcDeltaTime()
{
  uint32_t minRedrawDeltaTime = 1000/drawRate;
  
  uint32_t time = millis();
  deltaTime = time-lastLoop;
  lastLoop = time;
  
  bool redraw = (time-lastRedraw)>minRedrawDeltaTime;  
  if(redraw){
    redrawDeltaTime = time-lastRedraw;
    lastRedraw = time;
    digitalWrite(LED_RED,LOW);
    digitalWrite(LED_BLUE,HIGH);
  }
  else {
    digitalWrite(LED_RED,HIGH);
    digitalWrite(LED_BLUE,LOW);
  }

  return redraw;
}