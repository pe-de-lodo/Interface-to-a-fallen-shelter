#include <Arduino.h>
#include <Ethernet_Generic.h>

EthernetUDP Udp;

void initSendPixelsEthernet()
{
    Serial.println("A");

    Ethernet.init (21);



    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };
    arduino::IPAddress ip = arduino::IPAddress("192.168.0.3");
    Ethernet.begin(mac, ip);

    Serial.println("B");
    Udp.begin(1234);
    Serial.println("C");
    delay(2000);
    

}


void sendPixelsEthernet(char *pixels,int length)
{
    // Udp.
    //char ReplyBuffer[] = "ACK";      // a string to send back
    Udp.beginPacket("192.168.0.2", 6000);
    // Udp.write((char*)leds);
    Udp.write("ACK");
    Udp.endPacket();
}