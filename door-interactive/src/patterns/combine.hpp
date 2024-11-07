#include "abstractpattern.hpp"

class CombinePattern : public AbstractPattern 
{
    public:

    CombinePattern(AbstractPattern &pattern1, AbstractPattern &pattern2): m_pattern1(pattern1), m_pattern2(pattern2)
    {
    }

    void Start()
    {
        m_pattern1.Start();
        m_pattern2.Start();
    }

    void Update()
    {
        m_pattern1.Update();
        m_pattern2.Update();
    }

    bool IsVisible(int index, ledData data)
    {
        return m_pattern1.IsVisible(index,data)||m_pattern2.IsVisible(index,data);
    }

    CRGB Evaluate(int index, ledData ledInfo)
    {

        CRGB pixel1 = m_pattern1.IsVisible(index,ledInfo) ? m_pattern1.Evaluate(index,ledInfo) : CRGB::Black;
        CRGB pixel2 = m_pattern2.IsVisible(index,ledInfo) ? m_pattern2.Evaluate(index,ledInfo) : CRGB::Black;

        //blend based on luminance
        int lum1 = CalcLum(pixel1);
        int lum2 = CalcLum(pixel2);
        int lumTotal = (lum1+lum2);
        if(lumTotal==0){
            return CRGB::Black;
        }
        return blend(pixel1,pixel2,255*lum2/lumTotal);
    }

    inline uint8_t CalcLum(CRGB pixel){
        return min(299+pixel.r+587+pixel.g+114*pixel.b/1000,255);
    }

    
    protected:
    AbstractPattern &m_pattern1;
    AbstractPattern &m_pattern2;
};