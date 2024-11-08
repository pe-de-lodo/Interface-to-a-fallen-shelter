#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class GlitchPattern : public AbstractPattern
{
    float glitchVal;
    int glitchOffset = 0;
    float glitchNum;
    int numLED;

    public:
    GlitchPattern(int speed, int mmin, int mmax, int numled)
    {
        numLED = numled;
        m_timeline.add(glitchVal).init(0)
            // .then<Ease::Sine>(1,speed)
            .then<Ease::Sine>(0,speed, [this, mmin, mmax]() {
                glitchOffset = random(numLED);
                glitchNum = random(mmin, mmax);
            });
        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    GlitchPattern(int speed, int minn, int mmax, float inc, int numled)
    {
        numLED = numled;
        glitchNum = minn;
        m_timeline.add(glitchVal).init(0)
            // .then<Ease::Sine>(1,speed)
            .then<Ease::Sine>(0,speed, [this, inc, mmax]() {
                glitchOffset = random(numLED);
                glitchNum += inc;
                glitchNum = min(glitchNum, mmax);
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