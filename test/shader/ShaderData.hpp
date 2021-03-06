//
//  ShaderData.hpp
//  test
//
//  Created by 谭勇 on 2021/9/28.
//

#ifndef ShaderData_hpp
#define ShaderData_hpp

#include <stdio.h>
#include "../math/Math.hpp"
#include "../buffer/Texture.hpp"

#define FLOAT_OFFSET (0.5f)


class LinearSampling
{
public:
    LinearSampling() = default;
    ~LinearSampling() = default;
    
    static Vector4f Vector4fLerp(const Vector4f& v1,const Vector4f& v2,float factor)
    {
        return (v1 * (1.0f - factor)) + (v2 * factor);
    }
    
    static Vector2f Vector2fLerp(const Vector2f& v1,const Vector2f& v2,float factor)
    {
        return (v1 * (1.0f - factor)) + (v2 * factor);
    }
};

class ShaderData
{
public:
    //输出到片段着色器的坐标
    Vector4f m_PreviewPos;
    //颜色
    Vector4f m_Color;
    //纹理坐标
    Vector2f m_TextureCoord;
    // 1/z
    float m_Factor;
    
public:
    ShaderData() = default;
    ~ShaderData() = default;
    
    ShaderData(const Vector4f& pos,const Vector4f& color):m_PreviewPos(pos),m_Color(color){};
    
public:
    static ShaderData Interpolation(const ShaderData& fdata,const ShaderData& sdata,const float& factor)
    {
        ShaderData ret;
        ret.m_PreviewPos = LinearSampling::Vector4fLerp(fdata.m_PreviewPos,sdata.m_PreviewPos,factor);
        ret.m_Color = LinearSampling::Vector4fLerp(fdata.m_Color,sdata.m_Color,factor);
        ret.m_TextureCoord = LinearSampling::Vector2fLerp(fdata.m_TextureCoord,sdata.m_TextureCoord,factor);
        ret.m_Factor = (1.0f - factor)*fdata.m_Factor + factor * sdata.m_Factor;
        return ret;
    }
    
    static Matrix4x4f GetViewPortMatrix(int ox, int oy, int width, int height)
    {
        std::vector<float> vec(16,0.0f);
        vec[0] = width / 2.0f;
        vec[3] = ox + (width / 2.0f);
        vec[5] = - height / 2.0f;
        vec[7] = oy + (height / 2.0f);
        vec[10] = 1.0f;
        vec[15] = 1.0f;
        Matrix4x4f ret(vec);
        return ret;
    }
    
    //透视除法
    static void PerspectiveDivision(ShaderData& data)
    {
        data.m_PreviewPos =  data.m_PreviewPos / data.m_PreviewPos.GetW();
        data.m_PreviewPos.SetW(1.0f);
        data.m_PreviewPos.SetZ((data.m_PreviewPos.GetZ() + 1.0) * 0.5f);
    }
    
    void IntegerizationXY()
    {
        int temp = m_PreviewPos.GetX()+FLOAT_OFFSET;
        m_PreviewPos.SetX(temp);
        temp = m_PreviewPos.GetY()+FLOAT_OFFSET;
        m_PreviewPos.SetY(temp);
    }
    
    void IntegerizationPosX()
    {
        int temp = m_PreviewPos.GetX()+FLOAT_OFFSET;
        m_PreviewPos.SetX(temp);
    }
    
};



#endif /* ShaderData_hpp */
