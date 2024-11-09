#include <Arduino.h>
#include "alarm.h"
#include "main.h"
#include "sections/knockdetection.h"

void printAllSensorValues()
{

  int knockSensorValue = samplePiezo();
  int lightSensorValue = analogRead(LIGHT_SENSOR_PIN);

  bool doorKnobHeld = digitalRead(DOOR_KNOB);
  bool handSensor = digitalRead(WAKEUP_PIN);
  bool keyInserted = digitalRead(KEY_SENSOR);

  bool alarmWake = digitalRead(ALARM_PIN);

  //int batteryLevel = analogRead(PIN_VBAT);
  //bool isCharging = digitalRead(23)==HIGH;

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
  Serial.print(" alarmWakePin=");
  Serial.print(alarmWake);
  Serial.print(" ");
  printDateTime();
  

}

