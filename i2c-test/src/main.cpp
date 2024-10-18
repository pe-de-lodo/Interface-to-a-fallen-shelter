#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <SdFat.h>
#include <Wire.h>

void setup()
{
  Wire.begin();        // join i2c bus (address optional for main)
  Serial.begin(9600);  // start serial for output
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
}

void loop()
{
  Wire.requestFrom(0x5A, 6);    // request 6 bytes from secondary device #2

  while(Wire.available())    // secondary may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);
}
