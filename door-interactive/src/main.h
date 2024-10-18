#define LED_PIN 2 
#define MOSFET_PIN 7
#define WAKEUP_PIN 8
#define ALARM_PIN 9
#define KNOCK_PIN A0
#define LIGHT_SENSOR_PIN A3
#define NUM_LEDS 16 
#define DOOR_KNOB 10
#define KEY_SENSOR 1

#pragma once

void setLoopFunc(void loopFunc());
inline void calcDeltaTime();