#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class SuperBlinkPattern : public AbstractPattern
{
    float blinkVal;
    uint8_t m_hue = 0;

    public:
    SuperBlinkPattern(int blinkSpeed, int numBlinks, int holdTime, int waitTime, uint8_t hhue)
    {
        m_hue = hhue;
        Tween::Sequence<float>& seq = m_timeline.add(blinkVal).init(0);
        for(int i = 0; i < numBlinks; i++)
            seq.then(1,blinkSpeed).hold(holdTime).then(0,blinkSpeed).hold(holdTime);
        seq.hold(waitTime, [this]() {
            //m_hue = random(255);
        });

        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    CRGB Evaluate(int, ledData)
    {
        return CHSV(m_hue,255,(int)(blinkVal*255));        
    }
};