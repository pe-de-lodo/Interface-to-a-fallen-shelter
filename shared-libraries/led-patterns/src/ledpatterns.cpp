
#include <ledpatterns.h>

PatternCanvas::PatternCanvas(CRGB *leds, ledData *ledData, int length)
{
    m_length = length;
    m_leds = leds;
    m_ledData = ledData;
}

void PatternCanvas::TransitionToPattern(pattern ptrn,long duration,tweenData *dataPtr)
{
    m_transitionStack.push( patternTransition{ptrn,duration,0,dataPtr} );
}

void PatternCanvas::Update(long deltaTime)
{
    CRGB pixel;
    bool isTransitioning = m_transitionStack.size()>=2;
    for(int i=0;i<m_length;i++){
        
        if(isTransitioning){
            patternTransition destinationPattern = m_transitionStack[1];
            destinationPattern.elapsed += deltaTime;
            if(destinationPattern.elapsed>destinationPattern.duration){
                destinationPattern.elapsed = destinationPattern.duration;
            }
            CRGB pixelA = m_transitionStack[0].targetPattern(m_ledData[i],*m_transitionStack[0].tweenDataPtr);
            CRGB pixelB = destinationPattern.targetPattern(m_ledData[i],*destinationPattern.tweenDataPtr);
            
            float alpha = 1.0f*destinationPattern.elapsed/destinationPattern.duration;
            pixel = blend(pixelA,pixelB, alpha );

            if(destinationPattern.elapsed==destinationPattern.duration){
                m_transitionStack.shift();
            }
        }
        else {
            pixel = m_transitionStack[0].targetPattern(m_ledData[i],*m_transitionStack[0].tweenDataPtr);
        }
        m_leds[i] = pixel;
    }

}

