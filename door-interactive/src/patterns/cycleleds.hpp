#include "abstractpattern.hpp"

extern uint32_t deltaTime;
class CycleLeds : public AbstractPattern
{
    CRGB m_color;
    uint32_t m_index;
    int m_cycleLength;
    int m_colorLength;
    int m_direction;
    uint32_t m_durationSinceLastIncrement;
    public:

    CycleLeds(CRGB color, int cycleLength, int colorLength=1, int direction=1)
    {
        m_colorLength = colorLength;
        m_cycleLength = cycleLength;
        m_color = color;
        m_direction = direction;
    }

    void Start()
    {        
        AbstractPattern::Start(); 
        m_durationSinceLastIncrement = 0;
        m_index=0;       
    }

    void Update()
    {
        m_durationSinceLastIncrement += deltaTime;
        AbstractPattern::Update();
        if(m_durationSinceLastIncrement>100){
            m_index+=m_direction;
            m_durationSinceLastIncrement=0;
        }
    }


    CRGB Evaluate(int index, ledData data)
    {
        bool highlight = (index%m_cycleLength)>=(m_index%m_cycleLength) && (index%m_cycleLength)<((m_index+m_colorLength)%m_cycleLength);
        return highlight ? m_color : CRGB::Black;
    }
};