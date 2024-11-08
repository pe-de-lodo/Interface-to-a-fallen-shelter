#include <Arduino.h>
#include <FastLED.h>
#include "abstractpattern.hpp"

class ChasePattern : public AbstractPattern
{
    float chaseVal;
    int chaseLength;
    int colorLength;
    int chaseSpeed;
    int chaseOffset;
    CHSV m_color;

    public:
    ChasePattern(CHSV color,int speed, int length, int offset, int cLength)
    : m_color(color), chaseSpeed(speed), chaseLength(length), chaseOffset(offset), colorLength(cLength) 
    {

        m_timeline.add(chaseVal).init(0).then(1, chaseSpeed).then(0, 0);

        m_timeline.mode(Tween::Mode::REPEAT_TL);
        m_timeline.start();
    }

    CRGB Evaluate(int indx, ledData)
    {
        int ledIndex = (indx + chaseOffset) % chaseLength;
        int chaseIndex = chaseVal * chaseLength;

        bool highlight = false;
        
        if(chaseIndex + colorLength > chaseLength)
        {
            if(ledIndex >= chaseIndex || ledIndex < (chaseIndex + colorLength) - chaseLength)
                highlight = true;
        }
        else if(ledIndex >= chaseIndex && ledIndex < chaseIndex + colorLength)
            highlight = true;

        //bool highlight = ledIndex >= chaseIndex && ledIndex < chaseIndex + colorLength;
        return highlight ? m_color : CRGB(0, 0, 0);      
    }
};