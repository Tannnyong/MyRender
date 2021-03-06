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
            _DownTriangle(datavec[1], midPoint, datavec[2]);
        }
    }
    
    void ScanAxis(const ShaderData& x,const ShaderData& y,const ShaderData& z,const ShaderData& o)
    {
        if(!m_Buffer || !m_Shader)
        {
            std::cout<<"Error : m_Buffer or m_Shader is null"<<std::endl;
            return;
        }
        ShaderData drawData;
        int length = x.m_PreviewPos.GetX() - o.m_PreviewPos.GetX();
        for(int i = 0; i < length; i++)
        {
            drawData = ShaderData::Interpolation(x, o, (float)i/length);
            m_Buffer->WritePoint(drawData.m_PreviewPos.GetX()+FLOAT_OFFSET, drawData.m_PreviewPos.GetY()+FLOAT_OFFSET, x.m_Color);
        }
        
        length = y.m_PreviewPos.GetX() - o.m_PreviewPos.GetX();
        for(int i = 0; i < length; i++)
        {
            drawData = ShaderData::Interpolation(y, o, (float)i/length);
            m_Buffer->WritePoint(drawData.m_PreviewPos.GetX()+FLOAT_OFFSET, drawData.m_PreviewPos.GetY()+FLOAT_OFFSET, y.m_Color);
        }
        
        length = z.m_PreviewPos.GetX() - o.m_PreviewPos.GetX();
        for(int i = 0; i < length; i++)
        {
            drawData = ShaderData::Interpolation(z, o, (float)i/length);
            m_Buffer->WritePoint(drawData.m_PreviewPos.GetX()+FLOAT_OFFSET, drawData.m_PreviewPos.GetY()+FLOAT_OFFSET, z.m_Color);
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
        //fix2：整数化，防止出现黑色条纹
        left.IntegerizationXY();
        top.IntegerizationXY();
        right.IntegerizationXY();
        int dy = top.m_PreviewPos.GetY() - right.m_PreviewPos.GetY();
        
        ShaderData newleft;
        ShaderData newright;
        float weight = 0;
        for(int i = dy;i >= 0; i--)
        {
            if(dy != 0) weight = (float)i / dy;
            newleft = ShaderData::Interpolation(left, top, weight);
            newright = ShaderData::Interpolation(right, top, weight);
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
        left.IntegerizationXY();
        bottom.IntegerizationXY();
        right.IntegerizationXY();
        int dy = left.m_PreviewPos.GetY() - bottom.m_PreviewPos.GetY();
        
        ShaderData newleft;
        ShaderData newright;
        float weight = 0;
        for(int i = 0;i < dy; i++)
        {
            if(dy != 0) weight = (float)i / dy;
            newleft = ShaderData::Interpolation(left, bottom, weight);
            newright = ShaderData::Interpolation(right, bottom, weight);
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
        int x = 0;
        int y = 0;
        float z = 0;
        for(int i = 0; i < length; i++)
        {
            drawData = ShaderData::Interpolation(left, right, (float)i/length);
            //fix1: 这里xy加偏移 能够解决精度问题导致有黑色躁点
            float zFactor = drawData.m_Factor;
            drawData.m_TextureCoord =  drawData.m_TextureCoord / zFactor;
            x = drawData.m_PreviewPos.GetX()+FLOAT_OFFSET;
            y = drawData.m_PreviewPos.GetY()+FLOAT_OFFSET;
            z = drawData.m_PreviewPos.GetZ();
            color = m_Shader->FragmentShaderProcess(drawData);
            
            float depth = m_Buffer->GetDepth(x, y);
            if(depth > z)
            {
                m_Buffer->WritePoint(x, y, color);
                m_Buffer->WriteDepth(x, y, z);
            }
        }
    }
    
private:
    FrameBuffer* m_Buffer;
    Shader*     m_Shader;
    
};

#endif /* Rasterization_hpp */
