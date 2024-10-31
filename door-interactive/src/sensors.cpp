#include <Arduino.h>
#include "alarm.h"
#include "main.h"


void printAllSensorValues()
{
  int knockSensorValue = analogRead(KNOCK_PIN);
  int lightSensorValue = analogRead(LIGHT_SENSOR_PIN);

  bool keyInserted = digitalRead(KEY_SENSOR);
  bool doorKnobHeld = digitalRead(DOOR_KNOB);
  bool handSensor = digitalRead(WAKEUP_PIN);

  
  bool isCharging = digitalRead(23)==HIGH;

  Serial.print("handSensor=");
  Serial.print(handSensor);
  Serial.print(" knockSensor=");
  Serial.print(knockSensorValue);
  Serial.print(" doorKnob=");
  Serial.print(doorKnobHeld);
  Serial.print(" lightSensor=");
  Serial.print(lightSensorValue);
  Serial.print(" key=");
  Serial.print(keyInserted);    
  Serial.print(" ");
  printDateTime();
  Serial.print(" ");

}

