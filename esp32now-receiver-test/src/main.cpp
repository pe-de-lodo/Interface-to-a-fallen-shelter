#include <Arduino.h>
#include <esp_now.h>
#include "WiFi.h"

#define LEDPIN GPIO_NUM_2

// void setup(){

//   Serial.begin(115200);

//   WiFi.mode(WIFI_MODE_STA);

//   Serial.println(WiFi.macAddress());

// }

 typedef struct struct_message {

    char a[32];

    int b;

    float c;

    bool d;

} struct_message;


// Create a struct_message called myData

struct_message myData;


// callback function that will be executed when data is received

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&myData, incomingData, sizeof(myData));

  Serial.print("Bytes received: ");

  Serial.println(len);

  Serial.print("Char: ");

  Serial.println(myData.a);

  Serial.print("Int: ");

  Serial.println(myData.b);

  Serial.print("Float: ");

  Serial.println(myData.c);

  Serial.print("Bool: ");

  Serial.println(myData.d);

  Serial.println();

  digitalWrite(LEDPIN,HIGH);
  delay(100);
  digitalWrite(LEDPIN,LOW);

}

 

void setup() {

  // Initialize Serial Monitor
  

  pinMode(LEDPIN,OUTPUT);
  digitalWrite(LEDPIN,HIGH);
  delay(500);
  digitalWrite(LEDPIN,LOW);
  delay(500);

  Serial.begin(115200);
  Serial.println(WiFi.macAddress());
  


  // Set device as a Wi-Fi Station

  WiFi.mode(WIFI_STA);


  // Init ESP-NOW

  if (esp_now_init() != ESP_OK) {

    Serial.println("Error initializing ESP-NOW");

    return;

  }

  

  // Once ESPNow is successfully Init, we will register for recv CB to

  // get recv packer info

  esp_now_register_recv_cb(OnDataRecv);

}

 

void loop() {


}