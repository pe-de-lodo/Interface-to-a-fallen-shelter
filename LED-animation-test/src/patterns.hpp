
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

    CRGB Evaluate(ledData)
    {
        return CHSV(64,128,(int)(blinkVal*128));        
    }
};

class Ripples : public AbstractPattern 
{
    CRGB Evaluate(ledData ledInfo)
    {
        uint16_t pulse=beatsin16(120,0,255,0,uint16_t(0xfffFFL*ledInfo.x));
        return CHSV(180,255,pulse);
    }
};