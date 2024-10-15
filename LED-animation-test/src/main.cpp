#include <FastLED.h>
#include <ledpatterns.h>
#include <Tween.h>
#include <structData.h>

#define LED_PIN A3
#define NUM_LEDS 16

CRGB *leds;
ledData *data = structData;

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

    CRGB Evaluate(ledData)
    {
        m_timeline.update();
        
        return CHSV(0,128,(int)(blinkVal*128));
    }
};

BlinkPattern blinkPattern;

void setup()
{
    canvas.TransitionToPattern(&blinkPattern,0);
}

void loop()
{
    long frameDuration = 33;
    long updateStartTime = millis();
    canvas.Update(lastUpdateTime - updateStartTime);
    FastLED.show();
    lastUpdateTime = updateStartTime;
    long elapsed = millis()-updateStartTime; 
    delay(frameDuration-elapsed);    
}