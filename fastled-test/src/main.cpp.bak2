// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

#define PIN        2 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 16 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);


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
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<numCols; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(0, colors[i]);

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(1000); // Pause before next pass through loop

    pixels.setPixelColor(0, pixels.Color(0,0,0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(500); // Pause before next pass through loop

  }
}