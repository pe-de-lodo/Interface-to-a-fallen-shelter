#include "abstractpattern.hpp"

class MaskPattern : public AbstractPattern 
{
    public:

    MaskPattern(int sectionMask, AbstractPattern& pattern): m_pattern(pattern)
    {
        m_sectionMask = sectionMask;
    }

    void SetSection(int section)
    {
        m_sectionMask = section;
    }

    CRGB Evaluate(int index, ledData ledInfo)
    {
        return m_pattern.Evaluate(index,ledInfo);
    }

    void Update()
    {
        m_pattern.Update();
    }

    bool IsVisible(int index, ledData data)
    {
        return HasSection(data.section) && m_pattern.IsVisible(index, data);
    }
    
    void Start()
    {
        m_pattern.Start();
    }

    protected:
    int m_sectionMask;
    AbstractPattern& m_pattern;

    inline bool HasSection(int section)
    {

        return (1<<section & m_sectionMask)!=0;
    }

};