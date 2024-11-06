#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

DEFINE_GRADIENT_PALETTE( icePalette ) {
     0,    0,   0,   0,  // index, r, g, b
    16, 0,   0,   32,  
    20, 0,   0,   128,  
    24, 0,   0,   40,
    64, 0,   0,   64,  
    68, 0,   0,   255,  
    72, 0,   0,   100,  
    128, 64, 64, 128,
    255, 255, 255, 255 
};

class Ripples : public AbstractPattern 
{
    const CRGBPalette16 m_colorPalette = icePalette;
    public: 
    
    Ripples() : AbstractPattern()
    {
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
    }

    CRGB Evaluate(int index, ledData ledInfo)
    {
        //uint16_t pulse=beatsin16(120,0,255,0,uint16_t(0xfffL*ledInfo.x));

        float cx = 0.5f;
        float cy = 0.5f;
        float dx = ledInfo.x-cx;
        float dy = ledInfo.y-cy;
        float d = sqrtf((dx*dx) + (dy*dy));
        uint32_t msec = m_timeline.usec64()/1000;
        long timeScale = 0x50;
        long posScale = 0x2fffff;
        int16_t pulse = sin16_C(timeScale*msec-uint32_t(posScale*d))/0xff;
        pulse = constrain(pulse,0,255); //,constrain(255-int16_t((5*(d-0.1))*255),0,255)
        //pulse = triwave8((msec>>4)+(uint32_t)(ledInfo.x*10000));
        return ColorFromPalette(m_colorPalette,(uint8_t)pulse);
        //return CHSV(5*ledInfo.index,255,pulse);
    }
};