#include <FastLED.h>
#include <ledpatterns.h>
#include <Tween.h>

#define LED_PIN A3
#define NUM_LEDS 16

CRGB leds[NUM_LEDS];
ledData *data;

PatternCanvas canvas(leds,data,NUM_LEDS);

long lastUpdateTime = 0; 

class BlinkPattern : public AbstractPattern
{
    float blinkVal;

    public:
    BlinkPattern()
    {
        m_timeline.add(blinkVal).init(0).hold(1000).then(1,0).hold(1000);
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
        m_timeline.start();
    }

    void Update()
    {
        m_timeline.update();
    }

    CRGB Evaluate(ledData)
    {
        
        return CRGB(128,0,0);
        //return CHSV(0,128,(int)(blinkVal*128));
    }
};

BlinkPattern blinkPattern;

void setup()
{
    FastLED.addLeds<WS2812, 25, GRB>(leds, 1);
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, 1, NUM_LEDS-1);
    FastLED.showColor(CRGB(255,0,255));

    Serial.begin(115200);

    canvas.TransitionToPattern(&blinkPattern,0);
}

void loop()
{
    //FastLED.showColor(CRGB(255,0,255));
    Serial.println("test");
    delay(500);
    long frameDuration = 33;
    long updateStartTime = millis();
    canvas.Update(lastUpdateTime - updateStartTime);
    FastLED.show();
    lastUpdateTime = updateStartTime;
    long elapsed = millis()-updateStartTime; 
    delay(frameDuration-elapsed);    
    Serial.println();
    // for(int i=0;i<NUM_LEDS;i++){
    //     Serial.print(leds[i].red);
    //     Serial.print(" ");  
    //     Serial.print(leds[i].green);
    //     Serial.print(" ");  
    //     Serial.println(leds[i].blue);
    // }
}