#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"
#include "patterns.hpp"

DEFINE_GRADIENT_PALETTE( asteroidPalette ) {
    0,   0, 0,  0,
  8,  255, 168, 0,
  12,  255, 220, 64,
  20,  255, 168, 0,
  64,  64, 32, 0,
  128,  32, 8, 0,
  255, 0,0, 0};


class MeteorPattern : public AbstractPattern 
{
    float pos = 0;
    const CRGBPalette16 m_colorPalette = asteroidPalette;
    float centerX;
    float centerY;
    public:
    MeteorPattern(float posX, float posY)
    {
        centerX=posX;
        centerY=posY;
    }

    void Start()
    {
        AbstractPattern::Start();
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
        m_timeline.add(pos).init(0).then(0.5,4000);
    }

    CRGB Evaluate(int index, ledData ledInfo)
    {        
        
        float d = this->ledDistanceFrom(centerX,centerY,ledInfo);
        float k = constrain(20*(pos-d),0,1);

        return ColorFromPalette(m_colorPalette,uint8_t(k*0xff));
    }
};