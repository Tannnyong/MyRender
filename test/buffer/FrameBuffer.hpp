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
    FrameBuffer(int w = WINDOW_WIDTH, int h = WINDOW_HEIGHT)
    {
        m_With = w;
        m_Height = h;
        m_ColorBuffer.resize(w*h*4,0);
        m_DepthBuffer.resize(w*h, 1.0f);
    }
    ~FrameBuffer() = default;
    
    void ClearColorBuffer(const Vector4f& color)
    {
        m_ColorBuffer.assign(m_ColorBuffer.size(), 0.0f);
    }
    
    void ClearDepthBuffer()
    {
        m_DepthBuffer.assign(m_DepthBuffer.size(), 1.0f);
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
        //保存图片
//        FILE* file = fopen("/Users/yong.tan/Documents/my/MyRender/2.rgba", "wb+");
//        fwrite(m_ColorBuffer.data(), 1, m_ColorBuffer.size(), file);
//        fclose(file);
        
        return m_ColorBuffer.data();
    }
    
    float GetDepth(const int &x, const int  &y)
    {
        float ret = 1.0f;
        if (x < 0 || x >= m_With || y < 0 || y >= m_Height)
        {
            return ret;
        }
        int pos = (y * m_With + x);
        ret = m_DepthBuffer[pos];
        return ret;
    }
        
    void WriteDepth(const int &x,const int &y, const float &depth)
    {
        if (x < 0 || x >= m_With || y < 0 || y >= m_Height)
        {
            return;
        }
        int pos = (y * m_With + x);
        m_DepthBuffer[pos] = depth;
    }
    
private:
    int m_With;
    int m_Height;
    std::vector<unsigned char> m_ColorBuffer;
    std::vector<float> m_DepthBuffer;
};


#endif /* FrameBuffer_hpp */
