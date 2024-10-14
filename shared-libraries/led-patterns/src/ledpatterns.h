#include <CircularBuffer.hpp>
#include <FastLED.h>

typedef struct{
    char section;
    int index;
    float x;
    float y;
} ledData;

typedef struct{
    float a;
    float b;
    float c;
    float time;
} tweenData;

typedef CRGB (*pattern) (ledData,tweenData); 

typedef struct {
    pattern targetPattern;
    long duration;
    long elapsed;
    tweenData *tweenDataPtr;
} patternTransition;


class PatternCanvas {
    CircularBuffer<patternTransition,16> m_transitionStack;
    int m_length;
    CRGB *m_leds;
    ledData *m_ledData;

    public:
    PatternCanvas(CRGB *leds, ledData *ledData, int len);
    void TransitionToPattern(pattern ptrn,long duration,tweenData *dataPtr);
    void Update(long deltaTime);
};