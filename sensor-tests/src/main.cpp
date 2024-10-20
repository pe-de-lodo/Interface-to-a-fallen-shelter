#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <SdFat.h>
#include <FastLED.h>
#include <main.h>
#include <light_comms.h>
#include <sleep.h>
#include <alarm.h>

CRGB leds[NUM_LEDS];

void (*loopFunc) () = NULL;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting");
  delay(1000);
  pinMode(LED_BLUE,OUTPUT);
  digitalWrite(LED_BLUE,LOW);
  //set battery charging to 100mA
  // pinMode(22,OUTPUT);
  // digitalWrite(22, LOW);
  // pinMode(23,INPUT);
  Serial.println("Waking");
  delay(2000);

  //enable mosfet 
  enablePeripherals();
  Serial.println("enablePeripherals");
  delay(2000);
  //init clock and set alarm
  bool m = initWakeAlarm();
  Serial.print("initWakeAlarm ");
  Serial.println(m);
  delay(2000);

  //config wake pins, disable Flash
  configSleep();
  Serial.println("configSleep");
  delay(2000);

  // while(true){
  //   printDateTime();
  //   Serial.println("meep");
  //   delay(5);
  // }

  FastLED.addLeds<NEOPIXEL, LED_PIN_1>(leds, 1);
  FastLED.addLeds<NEOPIXEL, LED_PIN_2>(leds, 1,1);


  pinMode(LIGHT_SENSOR_PIN,INPUT_PULLUP);
  pinMode(KNOCK_PIN,INPUT);
  
  pinMode(DOOR_KNOB,INPUT_PULLDOWN);
  pinMode(KEY_SENSOR,INPUT_PULLDOWN);

  loopFunc = allTests;
}

void manageCharge()
{
  
  
}

void loop() {
  //checkBtns();
  manageCharge();
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




void blankLoop(){
  Serial.println("meep");
}


void allTests()
{
  int knockSensorValue = analogRead(KNOCK_PIN);
  int lightSensorValue = analogRead(LIGHT_SENSOR_PIN);

  bool keyInserted = digitalRead(KEY_SENSOR);
  bool doorKnobHeld = digitalRead(DOOR_KNOB);

  bool flash = (millis()/1000L)%2 == 0;
  bool isCharging = digitalRead(23)==HIGH;

  Serial.print("knockSensor=");
  Serial.print(knockSensorValue);  
  Serial.print(" lightSensor=");
  Serial.print(lightSensorValue);  
  Serial.print(" key=");
  Serial.print(keyInserted);  
  Serial.print(" doorKnob=");
  Serial.print(doorKnobHeld);  
  Serial.print(" isCharging=");
  Serial.print(isCharging);
  Serial.print(" ");
  printDateTime();
  Serial.println(" ");
//  Serial.println(flash);

  
  //test leds
  FastLED.clear();
  FastLED.showColor( flash ? CRGB::Red : CRGB::Blue );
  //FastLED.showColor( !flash ? CRGB::Red : CRGB::Blue );
  if(millis()>20000){
    sleep();
  }
}


// void mmwaveLoop(){
//   leds[0] = digitalRead(DETECT_PIN)==HIGH ? CRGB::Green : CRGB::Red;
//   if(Serial1.available()){
//     Serial.write(Serial1.read());
//   }
//   if(Serial.available()){
//     Serial1.write(Serial.read());
//   }
  
//   delay(100);
// }

// void (*funcs[5]) ()= {
//     mmwaveLoop,
//     lightSensor,
//     lightCommsLoop,
//     blankLoop,
//     blankLoop
// };

// void checkBtns(){
//   int j=0;
  
//   for(int i=5;i<10;i++){
//     if( digitalRead(i) && loopFunc!=funcs[j]){
//       loopFunc=funcs[j];
//       Serial.print("Mode ");
//       Serial.println(j);
      
//     }
//     j++;
//   }
// }
