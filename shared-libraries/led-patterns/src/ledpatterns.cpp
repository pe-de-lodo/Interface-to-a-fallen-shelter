
#include <ledpatterns.h>

PatternCanvas::PatternCanvas(CRGB *leds, ledData *ledData, int length)
{
    m_length = length;
    m_leds = leds;
    m_ledData = ledData;
}

void PatternCanvas::TransitionToPattern(AbstractPattern *pattern,long duration)
{
    m_transitionStack.push( patternTransition{pattern,duration} );
    pattern->Start();   
}

void PatternCanvas::Update(long deltaTime)
{
    CRGB pixel;
    float alpha = 0;
            
    bool isTransitioning = m_transitionStack.size()>=2;
    for(int i=m_transitionStack.size()-1;i>=0;i--){
        m_transitionStack[i].targetPattern->Update();
    }
    if(isTransitioning){
        patternTransition destinationPattern = m_transitionStack[1];

        m_transitionElapsedTime += deltaTime;
        if(m_transitionElapsedTime>destinationPattern.duration){
            m_transitionElapsedTime = destinationPattern.duration;
        }
        alpha = 1.0f*m_transitionElapsedTime/destinationPattern.duration;

        Serial.print(m_transitionElapsedTime);
        Serial.print(" ");
        Serial.print(destinationPattern.duration);
        Serial.print(" ");
        Serial.println(alpha);
        
        if(m_transitionElapsedTime>=destinationPattern.duration){
            m_transitionStack.shift();
            isTransitioning = false;
            m_transitionElapsedTime = 0;
        }
    }

    for(int i=0;i<m_length;i++){
        
        if(isTransitioning){
            patternTransition destinationPattern = m_transitionStack[1];
            
            CRGB pixelA = m_transitionStack[0].targetPattern->Evaluate(m_ledData[i]);
            CRGB pixelB = destinationPattern.targetPattern->Evaluate(m_ledData[i]);
            
            pixel = blend(pixelA,pixelB,(uint8_t)(0xff*alpha) );

            
        }
        else {
            pixel = m_transitionStack[0].targetPattern->Evaluate(m_ledData[i]);
            // Serial.print(pixel.red);
            // Serial.print(" ");
            // Serial.print(pixel.green);
            // Serial.print(" ");
            // Serial.println(pixel.blue);
        }
        m_leds[i] = pixel;
    }

}

