#include <FastLED.h>
#include <ledpatterns.h>
#include <Tween.h>
#include <structData.h>
#include <Ethernet_Generic.h>

#define LED_PIN A3
#define NUM_LEDS 450

EthernetUDP Udp;

CRGB leds[NUM_LEDS];
ledData *data = structData;

PatternCanvas canvas(leds,data,NUM_LEDS);

long lastUpdateTime = 0; 

class BlinkPattern : public AbstractPattern
{
    float blinkVal;

    public:
    BlinkPattern()
    {
        m_timeline.add(blinkVal).init(0).hold(500).then(1,100).hold(500).then(0,100);
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
        m_timeline.start();
    }

    CRGB Evaluate(ledData)
    {
        return CHSV(64,128,(int)(blinkVal*128));        
    }
};

class Ripples : public AbstractPattern 
{
    CRGB Evaluate(ledData ledInfo)
    {
        uint16_t pulse=beatsin16(120,0,255,0,uint16_t(0xfffFFL*ledInfo.x));
        return CHSV(180,255,pulse);
    }
};

Ripples ripplePattern;
BlinkPattern blinkPattern;
BlankPattern blankPattern;

void SendSimulatorData()
{
    //char ReplyBuffer[] = "ACK";      // a string to send back
    Udp.beginPacket("192.168.0.2", 6000);
    // Udp.write((char*)leds);
    Udp.write("ACK");
    Udp.endPacket();
}

void setup()
{
    FastLED.addLeds<WS2812, 25, GRB>(leds, 1);
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, 1, NUM_LEDS-1);
    FastLED.showColor(CRGB(255,0,255));

    Serial.begin(115200);
    while(!Serial){

    }
    delay(1000);
    Serial.println("A");
    Ethernet.init (21);
    Serial.println("B");
    Udp.begin(1234);
    Serial.println("C");
    delay(2000);
    SendSimulatorData();
    Serial.println("D");


    canvas.TransitionToPattern(&blinkPattern,0);
    canvas.TransitionToPattern(&blankPattern,4000);
    canvas.TransitionToPattern(&ripplePattern,4000);
    
}

void loop()
{
    long frameDuration = 33;
    long updateStartTime = millis();
    canvas.Update(updateStartTime-lastUpdateTime);
    FastLED.show();
    lastUpdateTime = updateStartTime;
    long elapsed = millis()-updateStartTime; 
    delay(frameDuration-elapsed); 
}

