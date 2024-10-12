#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPIFlash.h>
#include <nrf52840.h>


#define LED_PIN        2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 16 // Popular NeoPixel ring size
#define MOSFET_PIN 5
#define WAKEUP_PIN 10

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_RGB + NEO_KHZ800);


uint32_t colors[] = {
    pixels.Color(255,0,0),
    pixels.Color(0,255,0),
    pixels.Color(0,0,255),
    pixels.Color(255,255,0),
    pixels.Color(0,255,255),
    pixels.Color(255,0,255),
    pixels.Color(255,255,255)
};

const int numCols = 7;

void setup() {
  pixels.begin(); 
  pinMode(MOSFET_PIN,OUTPUT);
  digitalWrite(MOSFET_PIN,HIGH);
  pinMode(WAKEUP_PIN, INPUT_PULLUP_SENSE);
}

void loop() {
  pixels.clear(); 

  for(int i=0; i<numCols; i++) { // For each pixel...

    pixels.setPixelColor(0, colors[i]);

    pixels.show();  

    delay(1000); // Pause before next pass through loop

    pixels.setPixelColor(0, pixels.Color(0,0,0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(500); // Pause before next pass through loop

  }
}