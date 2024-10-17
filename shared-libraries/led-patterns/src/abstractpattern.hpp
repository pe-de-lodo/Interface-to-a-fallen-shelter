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