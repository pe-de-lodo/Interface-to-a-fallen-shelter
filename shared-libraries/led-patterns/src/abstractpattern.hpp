#include <Tween.h>
#include <FastLED.h>
typedef struct{
    char section;
    int index;
    float x;
    float y;
} ledData;

class AbstractPattern 
{
    public :
    virtual void Start()
    {
        m_timeline.restart();
    }
    virtual CRGB Evaluate(ledData) = 0;
    virtual void Update()
    {
        m_timeline.update();
    }
    
    protected:
    Tween::Timeline m_timeline;

    
};

class BlankPattern : public AbstractPattern
{

    public:
    BlankPattern()
    {

    }

    CRGB Evaluate(ledData)
    {
        return CRGB(0,0,0);
    }
};