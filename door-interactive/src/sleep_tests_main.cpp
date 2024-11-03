#include <Arduino.h>
#include "main.h"

void setup()
{
    
    pinMode(LED_BLUE,OUTPUT);
    digitalWrite(LED_BLUE,LOW);
    delay(5000);
    digitalWrite(LED_BLUE,HIGH);
    NRF_POWER->SYSTEMOFF=1;
    for(;;){}
}

void loop()
{

}
