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
    // AbstractPattern()
    // {

    //     Serial.println("AbstractPattern Constructor");
    // }
    virtual void Start()
    {
        m_timeline.restart();
        Serial.println("Starting pattern");
        m_timeline.start();
    }
    virtual CRGB Evaluate(int, ledData) = 0;
    virtual void Update()
    {
        m_timeline.update();

        // Serial.print("Updating pattern ");
        // Serial.print(" ");
        // Serial.print( m_timeline.msec() );
        // Serial.print(" ");
        // Serial.println( m_timeline.isRunning() ? "Running" : "Paused" );
    }

    virtual void SetMask(int len) // I'm thinking we can store the masked LEDs in a bitmask. It has to be big though!
    {
        m_ledMask = new uint32_t[(len>>5)+1];
    }

    virtual void MaskSection(int section)
    {

    }
    
    protected:
    Tween::Timeline m_timeline;
    uint32_t* m_ledMask;

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

    CRGB Evaluate(int, ledData)
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
        SetColor(color);
        m_timeline.mode(Tween::Mode::REPEAT_SQ);
    }

    void SetColor(CRGB color)
    {
        m_color = color;
    }

    CRGB Evaluate(int, ledData)
    {
        return m_color;
    }
};

class MaskedPattern : public AbstractPattern
{
    CRGB m_color;
    int m_offsetLED;
    int m_numLED;

    public:
    MaskedPattern()
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

class CycleLeds : public AbstractPattern
{
    CRGB m_color;
    int m_index;
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
        m_index++;
    }


    CRGB Evaluate(int index, ledData data)
    {
        bool highlight = (data.index%m_cycleLength)==(m_index%m_cycleLength);
        return highlight ? m_color : CRGB::Black;
    }
};