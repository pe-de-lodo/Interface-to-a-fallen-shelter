#include <Arduino.h>
#include <nrf52840.h>
#include <Adafruit_SPIFlash.h>
#include "main.h"
#include "sleep.h"

Adafruit_FlashTransport_QSPI flashTransport;

void sleep()
{
    digitalWrite(MOSFET_PIN,LOW);
    digitalWrite(LED_BLUE,HIGH);
    // pinMode(LED_PIN_1,INPUT);
    // pinMode(LED_PIN_2,INPUT);
    disconnectPin(LED_PIN_1);
    disconnectPin(LED_PIN_2);
    pinMode(LIGHT_SENSOR_PIN,INPUT);
    pinMode(KNOCK_PIN,INPUT);
 
    // pinMode(SDA,INPUT);
    // pinMode(SCL,INPUT);
    
    //This was weirdly setting one of the LED pins to GND
    //NRF_TWI1->ENABLE       = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos;
    
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
  pinMode(LED_PIN_1,OUTPUT);
  pinMode(LED_PIN_2,OUTPUT);
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

void disconnectPin(uint32_t ulPin)
{
    ulPin = g_ADigitalPinMap[ulPin];

    NRF_GPIO_Type * port = nrf_gpio_pin_port_decode(&ulPin);

    port->PIN_CNF[ulPin] = ((uint32_t)GPIO_PIN_CNF_DIR_Input        << GPIO_PIN_CNF_DIR_Pos)
                           | ((uint32_t)GPIO_PIN_CNF_INPUT_Disconnect    << GPIO_PIN_CNF_INPUT_Pos)
                           | ((uint32_t)GPIO_PIN_CNF_PULL_Disabled    << GPIO_PIN_CNF_PULL_Pos)
                           | ((uint32_t)GPIO_PIN_CNF_DRIVE_S0S1       << GPIO_PIN_CNF_DRIVE_Pos)
                           | ((uint32_t)GPIO_PIN_CNF_SENSE_Disabled   << GPIO_PIN_CNF_SENSE_Pos);
  
}