#include <Tween.h>

typedef struct{
    char section;
    int index;
    float x;
    float y;
} ledData;

class AbstractPattern 
{
    public :

    virtual CRGB Evaluate(ledData) = 0;

    protected:
    Tween::Timeline m_timeline;
};
