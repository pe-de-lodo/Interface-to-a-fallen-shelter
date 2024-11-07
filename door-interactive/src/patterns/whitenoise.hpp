#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class WhiteNoisePattern : public AbstractPattern
{
    uint16_t* desintegrateValues;
    uint16_t desintegrateVal;
    // int glitchNum;
    // int numLED;

    public:
    WhiteNoisePattern(int speed, int numled)
    {
        // numLED = numled;
        desintegrateValues = new uint16_t[numled];
        for(int i = 0; i < numled; i++)
            desintegrateValues[i] = random(USHRT_MAX);

        m_timeline.add(desintegrateVal).init(0).then(USHRT_MAX, speed);
        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    CRGB Evaluate(int indx, ledData)
    {
        return CHSV(0,0,random(2)*255);
    }
};