#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class NoisePattern : public AbstractPattern
{
    uint16_t noiseVal;

    uint8_t xScale;
    int yOffset;
    float yOffsetScale;
    //uint8_t m_hue = random(255);
    // float noiseY = 0;

    public:
    NoisePattern(int speed, int numled, float xscale, float yscale, int yoffset)
    {
        xScale = (USHRT_MAX / numled) * xscale;
        //yOffset = yoffset;
        //yOffsetScale = (yoffset / 255) + 1;
        m_timeline.add(noiseVal).init(0)
            .then(65535 * yscale,speed)
            .then(0,speed);
        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    CRGB Evaluate(int indx, ledData)
    {
        uint8_t brightness = inoise8(indx*xScale, noiseVal);
        //brightness = max(0, brightness - yOffset) * yOffsetScale;
        return CHSV(0,0,brightness);        
    }
};