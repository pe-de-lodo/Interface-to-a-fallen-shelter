#include <Arduino.h>
#include <nrf52840.h>
#include <main.h>
#include <Adafruit_SPIFlash.h>

Adafruit_FlashTransport_QSPI flashTransport;

void sleep()
{
    digitalWrite(MOSFET_PIN,LOW);
    pinMode(LED_PIN,INPUT);
    pinMode(KNOCK_PIN,INPUT);  
    // pinMode(SDA,INPUT);
    // pinMode(SCL,INPUT);
    NRF_POWER->SYSTEMOFF=1;
}


void QSPIF_sleep(void)
{
  flashTransport.begin();
  flashTransport.runCommand(0xB9);
  flashTransport.end();
}

void configSleep()
{
    pinMode(KNOCK_PIN,INPUT_PULLDOWN);   
    pinMode(MOSFET_PIN,OUTPUT);
    digitalWrite(MOSFET_PIN,HIGH);
    pinMode(WAKEUP_PIN, INPUT_SENSE_HIGH);
    pinMode(ALARM_PIN, INPUT_PULLUP_SENSE);

    QSPIF_sleep();
}