#define LED_PIN 2 
#define NUM_LEDS 16 
#define MOSFET_PIN 7
#define WAKEUP_PIN 8
#define ALARM_PIN 9
#define KNOCK_PIN A0
#pragma once

void setLoopFunc(void loopFunc());
inline void calcDeltaTime();