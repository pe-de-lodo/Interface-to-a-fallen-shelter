#include "abstractpattern.hpp"

class CombinePattern : public AbstractPattern 
{
    public:

    CombinePattern(AbstractPattern &pattern1, AbstractPattern &pattern2): m_pattern1(pattern1), m_pattern2(pattern2)
    {
    }

    CRGB Evaluate(int index, ledData ledInfo)
    {

        CRGB pixel1 = m_pattern1.IsVisible(index,ledInfo) ? m_pattern1.Evaluate(index,ledInfo) : CRGB::Black;
        CRGB pixel2 = m_pattern1.IsVisible(index,ledInfo) ? m_pattern2.Evaluate(index,ledInfo) : CRGB::Black;

        //blend based on luminance
        uint8_t lum1 = CalcLum(pixel1);
        uint8_t lum2 = CalcLum(pixel2);
        uint8_t lumTotal = (lum1+lum2);
        if(lumTotal==0){
            return CRGB::Black;
        }
        return blend(pixel1,pixel2,255*lum2/lumTotal);
    }

    inline uint8_t CalcLum(CRGB pixel){
        return 299+pixel.r+587+pixel.g+114*pixel.b/1000;
    }

    void Update()
    {
        m_pattern.Update();
    }

    bool IsVisible(int index, ledData data)
    {
        return true;
    }
    
    void Start()
    {
        m_pattern.Start();
    }

    protected:
    AbstractPattern &m_pattern1;
    AbstractPattern &m_pattern2;
};