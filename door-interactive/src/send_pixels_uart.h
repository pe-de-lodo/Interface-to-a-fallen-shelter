#include <Arduino.h>



void initSendPixelsUart()
{
    // 🤷
}

void sendPixelsUart(char *pixels,uint16_t length)
{
    char *lengthBytes = (char*)(&length);
    Serial.print("\nHEADER >>\n");
    Serial.write(lengthBytes,2);
    Serial.write(pixels,length);
}