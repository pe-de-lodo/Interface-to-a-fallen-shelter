#pragma once
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

    virtual void Update()
    {
        m_timeline.update();

        // Serial.print("Updating pattern ");
        // Serial.print(" ");
        // Serial.print( m_timeline.msec() );
        // Serial.print(" ");
        // Serial.println( m_timeline.isRunning() ? "Running" : "Paused" );
    }


    virtual bool IsVisible(int index, ledData data)
    {
        return true;
    }

    virtual CRGB Evaluate(int, ledData)
    {
        return CRGB::Black;
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


