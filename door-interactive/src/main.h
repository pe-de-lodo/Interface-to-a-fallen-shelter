#define LED_PIN 2 
#define NUMPIXELS 16 
#define MOSFET_PIN 5
#define WAKEUP_PIN 8
#define ALARM_PIN 9
#pragma once

void setLoopFunc(void loopFunc());
inline void calcDeltaTime();