#include <Arduino.h>
//#include <common.h>
#include <FastLED.h>

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

DEFINE_GRADIENT_PALETTE( asteroidPalette ) {
    0,   0, 0,  0,
  8,  255, 168, 0,
  12,  255, 220, 64,
  20,  255, 168, 0,
  64,  64, 32, 0,
  128,  32, 8, 0,
  255, 0,0, 0};

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

class MeteorPattern : public AbstractPattern 
{
    float pos = 0;
    const CRGBPalette16 m_colorPalette = asteroidPalette;

    public:
    void Start()
    {
        AbstractPattern::Start();
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
        m_timeline.add(pos).init(0).then(0.5,4000);

    }

    CRGB Evaluate(int index, ledData ledInfo)
    {        
        
        float d = this->ledDistanceFrom(0.5,0.5,ledInfo);
        float k = constrain(20*(pos-d),0,1);

        return ColorFromPalette(m_colorPalette,uint8_t(k*0xff));
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

Ripples ripplePattern;
BlinkPattern blinkPattern;
BlankPattern blankPattern;
MeteorPattern meteorPattern;
TestPattern testPattern;