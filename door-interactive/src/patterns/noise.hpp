#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class NoisePattern : public AbstractPattern
{
    uint16_t noiseVal;
    uint16_t hueNoiseVal;
    float dummy;

    uint8_t hueXScale;
    uint8_t xScale;
    int yOffset;
    float yOffsetUpscale;
    uint8_t m_hue = random(255);
    // float noiseY = 0;

    public:
    NoisePattern(int speed, int numled, float xscale, float yscale, int yoffset)
    {
        xScale = (USHRT_MAX / numled) * xscale;
        yOffset = yoffset;
        yOffsetUpscale = 255 / (255-yoffset);
        m_timeline.add(noiseVal).init(0)
            .then(65535 * yscale,speed)
            .then(0,speed);
        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }
    NoisePattern(int speed, int numled, float xscale, float yscale, int yoffset, uint8_t hue, int hueSpeed, float huexscale, float hueyscale, int hueAddTime)
    {
        xScale = (USHRT_MAX / numled) * xscale;
        hueXScale = (USHRT_MAX / numled) * huexscale;
        yOffset = yoffset;
        yOffsetUpscale = 255 / (255-yoffset);
        m_hue = hue;

        m_timeline.add(noiseVal).init(0)
            .then(65535 * yscale,speed)
            .then(0,speed);

        m_timeline.add(hueNoiseVal).init(0)
            .then(65535 * hueyscale,hueSpeed)
            .then(0,hueSpeed);
            
        m_timeline.add(dummy).init(0)
            .then(1,hueAddTime).then(0, 0);

        m_timeline.mode(Tween::Mode::REPEAT_SQ);
        m_timeline.start();
    }

    CRGB Evaluate(int indx, ledData)
    {
        uint8_t brightness = inoise8(indx*xScale, noiseVal);
        brightness = max(0, brightness - yOffset) * yOffsetUpscale;

        uint8_t hhue = ((m_hue + (dummy*255)) + (inoise8(indx*hueXScale, hueNoiseVal)) * 0.2) ;
        return CHSV(hhue,255,brightness);        
    }
};