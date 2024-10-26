#include <Arduino.h>
#include <nrf52840.h>
#include <main.h>
#include <Adafruit_SPIFlash.h>

Adafruit_FlashTransport_QSPI flashTransport;

void sleep()
{
    digitalWrite(MOSFET_PIN,LOW);
    digitalWrite(LED_BLUE,HIGH);
    pinMode(LED_PIN_1,INPUT);
    pinMode(KNOCK_PIN,INPUT);  
    // pinMode(SDA,INPUT);
    // pinMode(SCL,INPUT);
    NRF_TWI1->ENABLE       = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos;
    NRF_POWER->SYSTEMOFF=1;
}


void QSPIF_sleep(void)
{
  flashTransport.begin();
  flashTransport.runCommand(0xB9);
  flashTransport.end();
}

void enablePeripherals()
{
  pinMode(LED_BLUE,OUTPUT);
  digitalWrite(LED_BLUE,LOW);
  pinMode(MOSFET_PIN,OUTPUT);
  digitalWrite(MOSFET_PIN,HIGH);
}

void configSleep()
{
    pinMode(WAKEUP_PIN, INPUT_PULLUP_SENSE);
    pinMode(ALARM_PIN, INPUT_PULLUP_SENSE); //INPUT_PULLUP_SENSE

    //QSPIF_sleep();
}