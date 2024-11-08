#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class SuperBlinkPattern : public AbstractPattern
{
    float blinkVal;

    public:
    SuperBlinkPattern(int blinkSpeed, int numBlinks, int holdTime, int waitTime)
    {
        Tween::Sequence<float> seq = m_timeline.add(blinkVal).init(0).hold(holdTime).then(1,blinkSpeed).hold(holdTime).then(0,blinkSpeed).hold(waitTime);
        //for(int i = 0; i < numBlinks; i++)
            // seq;
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
        m_timeline.start();
    }

    CRGB Evaluate(int, ledData)
    {
        return CHSV(0,255,(int)(blinkVal*255));        
    }
};