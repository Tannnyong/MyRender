//
//  Rasterization.hpp
//  test
//
//  Created by 谭勇 on 2021/9/28.
//

#ifndef Rasterization_hpp
#define Rasterization_hpp

#include <stdio.h>
#include "limits.h"
#include "../shader/ShaderData.hpp"
#include "../buffer/FrameBuffer.hpp"
#include "../shader/Shader.hpp"

static bool comp (const ShaderData& t1,const ShaderData& t2)
{
    float y1 = t1.m_PreviewPos.GetY();
    float y2 = t2.m_PreviewPos.GetY();
    return y1 > y2;
}


static bool equalFloat( float fa, float fb, float absfloat =  std::numeric_limits<float>::epsilon() )
{
    if ( fa == fb ) return true;
    if ( fabsf( fa - fb ) < absfloat )
    {
        return true;
    }
    else
    {
        return false;
    }
}


class Rasterization
{
public:
    Rasterization():m_Buffer(nullptr){}
    ~Rasterization() = default;
    
    void SetFrameBufferAndShader(FrameBuffer* buffer,Shader* shader)
    {
        m_Buffer = buffer;
        m_Shader = shader;
    }
    
    void ScanLineTriangle(const ShaderData& t1,const ShaderData& t2, const ShaderData& t3)
    {
        if(!m_Buffer || !m_Shader)
        {
            std::cout<<"Error : m_Buffer or m_Shader is null"<<std::endl;
            return;
        }
        std::vector<ShaderData> datavec{t1,t2,t3};
        std::sort(datavec.begin(), datavec.end(), comp);
        
        float y0 = datavec[0].m_PreviewPos.GetY();
        float y1 = datavec[1].m_PreviewPos.GetY();
        float y2 = datavec[2].m_PreviewPos.GetY();
        
        if(equalFloat(y0,y1))
        {
            _DownTriangle(datavec[0],datavec[1],datavec[2]);
        }
        else if(equalFloat(y2,y1))
        {
            _UpTriangle(datavec[2],datavec[1],datavec[0]);
        }
        else
        {
            float weight = (datavec[0].m_PreviewPos.GetY() - datavec[1].m_PreviewPos.GetY()) / (datavec[0].m_PreviewPos.GetY() - datavec[2].m_PreviewPos.GetY());
            ShaderData midPoint = ShaderData::Interpolation(datavec[0], datavec[2], weight);
            _UpTriangle(datavec[1], midPoint, datavec[0]);
            _DownTriangle(datavec[0], midPoint, datavec[2]);
        }
    }
private:
    
    void _UpTriangle(const ShaderData& t1,const ShaderData& t2, const ShaderData& t3)
    {
        ShaderData left,right,top;
        float x1 =  t1.m_PreviewPos.GetX();
        float x2 =  t2.m_PreviewPos.GetX();
        if(x1 > x2)
        {
            left = t2;
            right = t1;
        }else
        {
            left = t1;
            right = t2;
        }
        top = t3;
        left.IntegerizationPosX();
        int dy = top.m_PreviewPos.GetY() - right.m_PreviewPos.GetY();
        for(int i = dy;i >= 0; i--)
        {
            float weight = 0;
            if(dy != 0) weight = (float)i / dy;
            ShaderData newleft = ShaderData::Interpolation(left, top, weight);
            ShaderData newright = ShaderData::Interpolation(right, top, weight);
            newleft.IntegerizationPosX();
            newright.IntegerizationPosX();
            _DrawLine(newleft,newright);
        }
    }
    
    void _DownTriangle(const ShaderData& t1,const ShaderData& t2, const ShaderData& t3)
    {
        ShaderData left,right,bottom;
        float x1 =  t1.m_PreviewPos.GetX();
        float x2 =  t2.m_PreviewPos.GetX();
        if(x1 > x2)
        {
            left = t2;
            right = t1;
        }else
        {
            left = t1;
            right = t2;
        }
        bottom = t3;
        int dy = left.m_PreviewPos.GetY() - bottom.m_PreviewPos.GetY();
        for(int i = 0;i < dy; i++)
        {
            float weight = 0;
            if(dy != 0) weight = (float)i / dy;
            ShaderData newleft = ShaderData::Interpolation(left, bottom, weight);
            ShaderData newright = ShaderData::Interpolation(right, bottom, weight);
            newleft.IntegerizationPosX();
            newright.IntegerizationPosX();
            _DrawLine(newleft,newright);
        }
    }
    
    void _DrawLine(const ShaderData& left,const ShaderData& right)
    {
        int length = right.m_PreviewPos.GetX() - left.m_PreviewPos.GetX();
        ShaderData drawData;
        Vector4f color;
        for(int i = 0; i < length; i++)
        {
            drawData = ShaderData::Interpolation(left, right, (float)i/length);
            color = m_Shader->FragmentShaderProcess(drawData);
            m_Buffer->WritePoint(drawData.m_PreviewPos.GetX()+FLOAT_OFFSET, drawData.m_PreviewPos.GetY()+FLOAT_OFFSET, color);
        }
    }
    
private:
    FrameBuffer* m_Buffer;
    Shader*     m_Shader;
    
};

#endif /* Rasterization_hpp */
