#include <Arduino.h>
#include <FastLED.h>
#include <main.h>
#include <light_comms.h>

#define DATA_PIN 25
#define DETECT_PIN 3

#define NUM_LEDS 1
CRGB leds[NUM_LEDS];

void (*loopFunc) () = NULL;

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // put your setup code here, to run once:
  Serial.begin(115200,SERIAL);
  Serial.write("serial1");
  leds[0] = CRGB::Red;
  FastLED.show();
  Serial1.begin(115200);

  pinMode(DETECT_PIN,INPUT);
  setupBtns();
  setupLightSensor();
}

void loop() {
  checkBtns();
  if(loopFunc!=NULL){
    loopFunc();
    FastLED.show();
  }
}

void setupBtns(){
  for(int i=5;i<10;i++){
    pinMode(i,INPUT);
  }
}

void setupLightSensor(){
  pinMode(A2,INPUT);
  pinMode(A3,OUTPUT);
  //analogReference(DEFAULT);
}



void blankLoop(){
  Serial.println("meep");
}

void lightSensor(){
  int val = analogRead(A2);
  leds[0] = blend( CRGB::Red, CRGB::Green, 256L*val/1024);
  
  Serial.println(val);
  delay(100);
}

void mmwaveLoop(){
  leds[0] = digitalRead(DETECT_PIN)==HIGH ? CRGB::Green : CRGB::Red;
  if(Serial1.available()){
    Serial.write(Serial1.read());
  }
  if(Serial.available()){
    Serial1.write(Serial.read());
  }
  
  delay(100);
}

void (*funcs[5]) ()= {
    mmwaveLoop,
    lightSensor,
    lightCommsLoop,
    blankLoop,
    blankLoop
};

void checkBtns(){
  int j=0;
  
  for(int i=5;i<10;i++){
    if( digitalRead(i) && loopFunc!=funcs[j]){
      loopFunc=funcs[j];
      Serial.print("Mode ");
      Serial.println(j);
      
    }
    j++;
  }
}
