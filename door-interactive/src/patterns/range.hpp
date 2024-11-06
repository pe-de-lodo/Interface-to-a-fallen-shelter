#include <FastLED.h>
#include "abstractpattern.hpp"

class RangePattern : public AbstractPattern
{
    CRGB m_color;
    int m_offsetLED;
    int m_numLED;

    public:
    RangePattern()
    {
        m_timeline.mode(Tween::Mode::REPEAT_SQ); 
    }
    
    void Start()
    {   
         
        AbstractPattern::Start();
    }

    void Update()
    {
        AbstractPattern::Update();
    }

    void Set(CRGB color, int offset, int num)
    {
        m_color = color;
        m_offsetLED = offset;
        m_numLED = num;
    }

    CRGB Evaluate(int indx, ledData data)
    {
        if(indx >= m_offsetLED && indx < m_offsetLED+m_numLED)
            return m_color;

        return CRGB::Black;
    }
};