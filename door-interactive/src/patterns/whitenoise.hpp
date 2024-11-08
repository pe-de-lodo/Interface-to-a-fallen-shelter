#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class WhiteNoisePattern : public AbstractPattern
{
    uint8_t* noiseValues;
    uint8_t timelineVal;

    uint8_t lowTresh;
    uint8_t highTresh;
    int numLED;

    public:
    WhiteNoisePattern(int speed, int numled, int low = 0, int high = 255)
    {
        numLED = numled;
        lowTresh = low;
        highTresh = high;
        noiseValues = new uint8_t[numled];
        NoiseValues();

        m_timeline.add(timelineVal).init(0).then(1, speed, [this]() { NoiseValues(); });
        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    void NoiseValues()
    {
        for(int i = 0; i < numLED; i++)
            noiseValues[i] = random(255);
    }

    CRGB Evaluate(int indx, ledData)
    {
        uint8_t b = noiseValues[indx];
        b = b <= lowTresh || lowTresh == 255 ? 0 : b;
        b = b > highTresh || highTresh == 0 ? 255 : b;
        return CHSV(0, 0, b);
    }
};