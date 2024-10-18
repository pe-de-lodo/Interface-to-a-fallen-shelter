#include <Arduino.h>
#include <main.h>
#include <knocking/knockdetection.h>
#include <sleep.h>
#include <light_comms.h>
#include <FastLED.h>
#include <alarm.h>
#include <Wire.h>
//#include <ledpatterns.h>

CRGB leds[NUM_LEDS];
uint32_t deltaTime;
uint32_t lastLoop;

void (*loopFunc) (void) = NULL;

// void setup()
// {
//   Wire.begin();        // join i2c bus (address optional for main)
//   Serial.begin(9600);  // start serial for output
//   while ( !Serial ) delay(10);   // for nrf52840 with native usb

//   Wire.requestFrom(0x5A, 6);    // request 6 bytes from secondary device #2

//   while(Wire.available())    // secondary may send less than requested
//   { 
//     char c = Wire.read(); // receive a byte as character
//     Serial.print(c);         // print the character
//   }

//   delay(500);
//   initWakeAtTime();
// }

void setup() 
{
  Serial.begin(115200);
  //while(!Serial) delay(5);
  Serial.println("INIT");
  initWakeAtTime();
  FastLED.addLeds<WS2812, LED_PIN>(leds, NUM_LEDS, GRB);  
  FastLED.clear();
  configSleep();  
  setLoopFunc(initKnock);
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