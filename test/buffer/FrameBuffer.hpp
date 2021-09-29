//
//  FrameBuffer.hpp
//  test
//
//  Created by 谭勇 on 2021/9/28.
//

#ifndef FrameBuffer_hpp
#define FrameBuffer_hpp

#include <stdio.h>
#include <fstream>
#include "../math/Math.hpp"

class FrameBuffer
{
public:
    FrameBuffer(int w = 800, int h = 600)
    {
        m_With = w;
        m_Height = h;
        m_ColorBuffer.resize(w*h*4,0);
    }
    ~FrameBuffer() = default;
    
    void Clear(const Vector4f& color)
    {
        unsigned char * p = m_ColorBuffer.data();
        for (int i = 0; i < m_With*m_Height * 4; i += 4)
        {
            *(p + i) = color.GetX();
            *(p + i + 1) = color.GetY();
            *(p + i + 2) = color.GetZ();
            *(p + i + 3) = color.GetW();
        }
    }
    
    void WritePoint(const int &x,const int &y, const Vector4f& color)
    {
        if (x < 0 || x >= m_With || y < 0 || y >= m_Height)
        {
            return;
        }
        int pos = (y * m_With + x);
        m_ColorBuffer[pos * 4] = (int)color.GetX();
        m_ColorBuffer[pos * 4 + 1] = (int)color.GetY();
        m_ColorBuffer[pos * 4 + 2] = (int)color.GetZ();
        m_ColorBuffer[pos * 4 + 3] = (int)color.GetW();
    }
    
    unsigned char * GetDataPtr()
    {
//        FILE* file = fopen("/Users/yong.tan/Documents/my/MyRender/2.rgba", "wb+");
//        fwrite(m_ColorBuffer.data(), 1, m_ColorBuffer.size(), file);
//        fclose(file);
        
        return m_ColorBuffer.data();
    }
    
private:
    int m_With;
    int m_Height;
    std::vector<unsigned char> m_ColorBuffer;
    
};


#endif /* FrameBuffer_hpp */
