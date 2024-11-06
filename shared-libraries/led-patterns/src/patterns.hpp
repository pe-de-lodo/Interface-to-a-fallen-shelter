#pragma once

#include <Arduino.h>
#include <FastLED.h>





  DEFINE_GRADIENT_PALETTE( testPalette ) {
    0,   0, 0,  0,
    32,  255, 0, 0,
    64,  0, 255, 0,
    96,  128, 0, 0,
    128,  0, 255, 0,
    160,  64, 0, 0,
    192,  0, 255, 0,
    224,  255, 255, 255,
    255,  0, 0, 0};



class BlinkPattern : public AbstractPattern
{
    float blinkVal;

    public:
    BlinkPattern()
    {
        m_timeline.add(blinkVal).init(0).hold(500).then(1,100).hold(500).then(0,100);
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
        m_timeline.start();
    }

    CRGB Evaluate(int, ledData)
    {
        return CHSV(0,255,(int)(blinkVal*255));        
    }
};





class TestPattern : public AbstractPattern 
{
    float pos = 0;
    const CRGBPalette16 m_colorPalette = testPalette;

    public:
    void Start()
    {
        AbstractPattern::Start();
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
        m_timeline.add(pos).init(0).then(2,10000).then(0,0);

    }

    CRGB Evaluate(int index, ledData ledInfo)
    {        
        
        // uint8_t d = this->ledSection(ledInfo);
        float d = this->ledDistanceFrom(0.5,0.5,ledInfo);
        float k = constrain((pos-d),0,1);

        return ColorFromPalette(m_colorPalette,uint8_t(k*0xff));
    }
};

class PulsePattern : public AbstractPattern
{
    float pulseVal;
    uint8_t m_hue = random(255);

    public:
    PulsePattern(int speed)
    {
        m_timeline.add(pulseVal).init(0)
            .then<Ease::Sine>(1,speed)
            .then<Ease::Sine>(0,speed, [this]() {
                m_hue = random(255);
            });
        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    CRGB Evaluate(int, ledData)
    {
        return CHSV(m_hue,255,(int)(pulseVal*255));        
    }
};


BlankPattern blankPattern;
