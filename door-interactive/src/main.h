#define LED_PIN        2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 16 // Popular NeoPixel ring size
#define MOSFET_PIN 5
#define WAKEUP_PIN 10
#define ALARM_PIN 9
#pragma once

void setLoopFunc(void loopFunc());
inline void calcDeltaTime();