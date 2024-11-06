
#include <ledpatterns.h>

PatternCanvas::PatternCanvas(CRGB *leds, ledData *ledData, int length)
{
    m_length = length;
    m_leds = leds;
    m_ledData = ledData;
}

void PatternCanvas::TransitionToPattern(AbstractPattern *pattern,long duration)
{
    if(pattern==NULL){
        Serial.println("Couldn't transition to pattern. Was null");
        return;
    }
    m_transitionStack.push( patternTransition{pattern,duration} );
    pattern->Start();   
}

void PatternCanvas::Clear()
{
    m_transitionStack.clear();
    m_transitionElapsedTime = 0;
    FastLED.clear();
}

void PatternCanvas::Update(long deltaTime)
{
    CRGB pixel;
    float alpha = 0;
    bool isTransitioning = m_transitionStack.size()>=2;

    if(m_transitionStack.size()==0){
        return;
    }
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
        patternTransition currentPattern = m_transitionStack[0];
        bool currentPatternPixelVisible = currentPattern.targetPattern->IsVisible(i,m_ledData[i]);
        
        if(isTransitioning){
            patternTransition destinationPattern = m_transitionStack[1];
            bool destinationPatternPixelVisible = destinationPattern.targetPattern->IsVisible(i,m_ledData[i]);
            CRGB pixelA = currentPatternPixelVisible ? currentPattern.targetPattern->Evaluate(i, m_ledData[i]) : CRGB::Black;
            CRGB pixelB = destinationPatternPixelVisible ? destinationPattern.targetPattern->Evaluate(i, m_ledData[i]) : CRGB::Black;
            
            pixel = blend(pixelA,pixelB,(uint8_t)(0xff*alpha) );

            
        }
        else {
            bool visible = currentPattern.targetPattern->IsVisible(i,m_ledData[i]);
            
            pixel = visible ? currentPattern.targetPattern->Evaluate(i, m_ledData[i]) : CRGB::Black;
            // Serial.print(pixel.red);
            // Serial.print(" ");
            // Serial.print(pixel.green);
            // Serial.print(" ");
            // Serial.println(pixel.blue);
        }
        m_leds[i] = pixel;
    }

}

