#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class GlitchPattern : public AbstractPattern
{
    float glitchVal;
    int glitchOffset = 0;
    int glitchNum;
    int numLED;

    public:
    GlitchPattern(int speed, int min, int max, int numled)
    {
        numLED = numled;
        m_timeline.add(glitchVal).init(0)
            // .then<Ease::Sine>(1,speed)
            .then<Ease::Sine>(0,speed, [this, min, max]() {
                glitchOffset = random(numLED);
                glitchNum = random(min, max);
            });
        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    CRGB Evaluate(int indx, ledData)
    {
        bool masked = true;
        int endLED = glitchOffset + glitchNum;
        if(endLED > numLED)
        {
            if(indx >= glitchOffset || indx < endLED % numLED)
                masked = false;
        }
        else if(indx >= glitchOffset && indx < (glitchOffset + glitchNum))
            masked = false;

        if(!masked)
            return CHSV(0,0,255);
        else
            return CRGB::Black;
    }
};