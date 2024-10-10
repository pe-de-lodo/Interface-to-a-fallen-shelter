#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address

uint8_t broadcastAddress[] = {0x30, 0xAE, 0xA4, 0x98, 0x14, 0xA8};

// Structure example to send data

// Must match the receiver structure

typedef struct struct_message {

  char a[32];

  int b;

  float c;

  bool d;

} struct_message;


// Create a struct_message called myData

struct_message message;


esp_now_peer_info_t peerInfo;


// callback when data is sent

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  Serial.print("\r\nLast Packet Send Status:\t");

  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

}

 

void setup() {

  // Init Serial Monitor

  Serial.begin(115200);

 

  // Set device as a Wi-Fi Station

  WiFi.mode(WIFI_STA);


  // Init ESP-NOW

  if (esp_now_init() != ESP_OK) {

    Serial.println("Error initializing ESP-NOW");

    return;

  }


  // Once ESPNow is successfully Init, we will register for Send CB to

  // get the status of Transmitted packet

  esp_now_register_send_cb(OnDataSent);

  

  // Register peer

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);

  peerInfo.channel = 0;  

  peerInfo.encrypt = false;

  

  // Add peer        

  if (esp_now_add_peer(&peerInfo) != ESP_OK){

    Serial.println("Failed to add peer");

    return;

  }

}

 

void loop() {

  // Set values to send

  strcpy(message.a, "THIS IS A CHAR");

  message.b = random(1,20);

  message.c = 1.2;

  message.d = false;

  

  // Send message via ESP-NOW

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));

   

  if (result == ESP_OK) {

    Serial.println("Sent with success");

  }

  else {

    Serial.println("Error sending the data");

  }

  delay(2000);

}