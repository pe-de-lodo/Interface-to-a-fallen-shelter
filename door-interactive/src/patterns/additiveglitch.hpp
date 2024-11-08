#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class AddGlitchPattern : public AbstractPattern
{
    float glitchVal;
    int glitchOffset = 0;
    float glitchNum;
    int numLED;
    int colorDecay;
    CHSV* ledColors;
    CHSV col = CHSV(random(255),255,255);
    CHSV initColor;

    public:
    AddGlitchPattern(CHSV initcolor, int speed, int mmin, int mmax, int decay, int numled)
    {
        numLED = numled;
        ledColors = new CHSV[numled];
        colorDecay = decay;
        initColor = initcolor;
        fill_solid(ledColors, numled, initColor);

        m_timeline.add(glitchVal).init(0)
            // .then<Ease::Sine>(1,speed)
            .then<Ease::Sine>(0,speed, [this, mmin, mmax]() {
                glitchOffset = random(numLED);
                glitchNum = random(mmin, mmax);
                col = CHSV(random(255),255,255);
            });
        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    void Start()
    {
        AbstractPattern::Start();
        fill_solid(ledColors, numLED, initColor);
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
            ledColors[indx] = col;
        else
            ledColors[indx] = blend(ledColors[indx], CHSV(0, 0, 0), colorDecay, TGradientDirectionCode::SHORTEST_HUES);
        
        return ledColors[indx];
    }
};