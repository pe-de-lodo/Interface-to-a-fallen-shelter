#pragma once

#include <CircularBuffer.hpp>
#include <FastLED.h>
#include <abstractpattern.hpp>

typedef struct {
    AbstractPattern *targetPattern;
    long duration;
} patternTransition;


class PatternCanvas {
    CircularBuffer<patternTransition,16> m_transitionStack;
    int m_length;
    CRGB *m_leds;
    ledData *m_ledData;
    long m_transitionElapsedTime = 0;

    public:
    PatternCanvas(CRGB *leds, ledData *ledData, int len);
    void TransitionToPattern(AbstractPattern *ptrn,long duration);
    void Update(long deltaTime);
    void Clear();
};