#include <Arduino.h>
#include <nrf52840.h>
#include <Adafruit_SPIFlash.h>
#include "main.h"
#include "sleep.h"
#include "FastLED.h"

Adafruit_FlashTransport_QSPI flashTransport;

void enablePeripherals()
{
  pinMode(LED_BLUE,OUTPUT);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  digitalWrite(LED_BLUE,LOW);
  pinMode(LED_PIN_0,OUTPUT);
  pinMode(LED_PIN_1,OUTPUT);
  pinMode(DOOR_KNOB,INPUT);
  pinMode(KEY_SENSOR,KEY_SENSOR);
  pinMode(LIGHT_SENSOR_PIN,INPUT_PULLUP);
  pinMode(MOSFET_PIN,OUTPUT);
  pinMode(KNOCK_PIN,INPUT);
  digitalWrite(MOSFET_PIN,HIGH);
}

void sleep()
{
    digitalWrite(MOSFET_PIN,LOW);
    digitalWrite(LED_BLUE,HIGH);
    // disconnectPin(LED_BLUE);
    // disconnectPin(LED_RED);
    // disconnectPin(LED_GREEN);
    // disconnectPin(LED_PIN_0);
    // disconnectPin(LED_PIN_1);
    pinMode(LED_BLUE,INPUT);
    pinMode(LED_RED,INPUT);
    pinMode(LED_GREEN,INPUT);
    pinMode(LED_PIN_0,INPUT);
    pinMode(LED_PIN_1,INPUT);
    pinMode(LIGHT_SENSOR_PIN,INPUT);
    pinMode(KNOCK_PIN,INPUT);
 
    // pinMode(SDA,INPUT);
    // pinMode(SCL,INPUT);
    
    //disable i2c interface
    NRF_TWI1->ENABLE       = TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos;
    FastLED.clear();
    delay(1000);
    
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

    pinMode(WAKEUP_PIN, INPUT_SENSE_HIGH);
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