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
        Serial.println("Starting pattern");
        m_timeline.start();
    }
    virtual CRGB Evaluate(ledData) = 0;
    virtual void Update()
    {
        m_timeline.update();

        // Serial.print("Updating pattern ");
        // Serial.print(" ");
        // Serial.print( m_timeline.msec() );
        // Serial.print(" ");
        // Serial.println( m_timeline.isRunning() ? "Running" : "Paused" );
    }
    
    protected:
    Tween::Timeline m_timeline;

    static float ledDistanceFrom(float x, float y, ledData led)
    {
        float dx = led.x - x;
        float dy = led.y - y;
        return sqrtf((dx*dx) + (dy*dy));
    }

    static uint8_t ledSection(ledData led)
    {
        return led.section;
    }
    
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

class SolidColorPattern : public AbstractPattern
{
    CRGB m_color;
    public:
    SolidColorPattern(CRGB color)
    {
        m_color = color;
    }

    CRGB Evaluate(ledData)
    {
        return m_color;
    }
};

class CycleLeds : public AbstractPattern
{
    CRGB m_color;
    int index;
    int m_cycleLength;
    public:

    CycleLeds(CRGB color, int cycleLength)
    {
        m_cycleLength = cycleLength;
        m_color = color;
    }

    void Start()
    {        
        AbstractPattern::Start();        
    }

    void Update()
    {
        AbstractPattern::Update();
        index++;
    }


    CRGB Evaluate(ledData data)
    {
        bool highlight = data.index==(index%m_cycleLength);
        return highlight ? m_color : CRGB::Black;
    }
};