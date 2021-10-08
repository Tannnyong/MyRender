//
//  Shader.hpp
//  test
//
//  Created by 谭勇 on 2021/9/28.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include "../math/Math.hpp"
#include "ShaderData.hpp"
#include "../vertex/Vertex.hpp"

class Shader
{
public:
    Shader() = default;
    ~Shader() = default;
    
public:
    void SetModelMatrix(const Matrix4x4f& model)
    {
        m_ModelMatrix = model;
    }
    void SetViewMatrix(const Matrix4x4f& view)
    {
        m_ViewMatrix = view;
    }
    void SetProjectMatrix(const Matrix4x4f& projection)
    {
        m_ProjectMatrix = projection;
    }
    
    void LoadTexture(const std::string& path)
    {
        m_Sample2D.Load(path);
    }
    
    ShaderData VertexShaderProcess(const Vertex& vertex)
    {
        ShaderData dataout;
    
        Vector4f test = m_ModelMatrix * vertex.m_Position;
        test.SetW(1.0f);
        Matrix4x4f temp = m_ProjectMatrix * m_ViewMatrix * m_ModelMatrix;
        dataout.m_PreviewPos = temp * vertex.m_Position;
        dataout.m_Factor = 1.0f / dataout.m_PreviewPos.GetW();
        dataout.m_Color = vertex.m_Color;
        
        //投影后的x'和y'，与z不是线性关系，与1/z才是线性关系
        dataout.m_TextureCoord = vertex.m_TextureCoord * dataout.m_Factor;
        return dataout;
    }
    
    Vector4f FragmentShaderProcess(ShaderData& datain)
    {
        Vector4f color = m_Sample2D.Sample2D(datain.m_TextureCoord.GetX(), datain.m_TextureCoord.GetY());
        color = color * 0.5 + datain.m_Color * 0.5;
        color.SetW(255.0f);
        return color;
    }
    
    
    void GetFrustumPlanes(std::vector<Vector4f> & result , Matrix4x4f &vp)
    {
        //左侧
        result[0].SetX( vp(0,3) + vp(0,0) );
        result[0].SetY( vp(1,3) + vp(1,0) );
        result[0].SetZ( vp(2,3) + vp(2,0) );
        result[0].SetW( vp(3,3) + vp(3,0) );
        //右侧
        result[1].SetX( vp(0,3) - vp(0,0) );
        result[1].SetY( vp(1,3) - vp(1,0) );
        result[1].SetZ( vp(2,3) - vp(2,0) );
        result[1].SetW( vp(3,3) - vp(3,0) );
        //上侧
        result[2].SetX( vp(0,3) - vp(0,1) );
        result[2].SetY( vp(1,3) - vp(1,1) );
        result[2].SetZ( vp(2,3) - vp(2,1) );
        result[2].SetW( vp(3,3) - vp(3,1) );
        //下侧
        result[3].SetX( vp(0,3) + vp(0,1) );
        result[3].SetY( vp(1,3) + vp(1,1) );
        result[3].SetZ( vp(2,3) + vp(2,1) );
        result[3].SetW( vp(3,3) + vp(3,1) );
        //Near
        result[4].SetX( vp(0,3) + vp(0,2) );
        result[4].SetY( vp(1,3) + vp(1,2) );
        result[4].SetZ( vp(2,3) + vp(2,2) );
        result[4].SetW( vp(3,3) + vp(3,2) );
        //Far
        result[5].SetX( vp(0,3) - vp(0,2) );
        result[5].SetY( vp(1,3) - vp(1,2) );
        result[5].SetZ( vp(2,3) - vp(2,2) );
        result[5].SetW( vp(3,3) - vp(3,2) );
    }

    //点到平面距离 d =  Ax + By + Cz + D;
    // d < 0 点在平面法向反方向所指的区域
    // d > 0 点在平面法向所指的区域
    // d = 0 在平面上
    // d < 0为 false
    bool Point2Plane(const Vector4f &v,const Vector4f &p)
    {
        return p.GetX() * v.GetX() + p.GetY() * v.GetY() + p.GetZ() * v.GetZ() + p.GetW() >= 0.0f;
    }

    void UpdateViewPlanes()
    {
        Matrix4x4f vp = m_ProjectMatrix * m_ViewMatrix;
        if(m_ViewPlanes.empty())
        {
            m_ViewPlanes.resize(6);
        }
        //左侧
        m_ViewPlanes[0].SetX( vp(0,3) + vp(0,0) );
        m_ViewPlanes[0].SetY( vp(1,3) + vp(1,0) );
        m_ViewPlanes[0].SetZ( vp(2,3) + vp(2,0) );
        m_ViewPlanes[0].SetW( vp(3,3) + vp(3,0) );
        //右侧
        m_ViewPlanes[1].SetX( vp(0,3) - vp(0,0) );
        m_ViewPlanes[1].SetY( vp(1,3) - vp(1,0) );
        m_ViewPlanes[1].SetZ( vp(2,3) - vp(2,0) );
        m_ViewPlanes[1].SetW( vp(3,3) - vp(3,0) );
        //上侧
        m_ViewPlanes[2].SetX( vp(0,3) - vp(0,1) );
        m_ViewPlanes[2].SetY( vp(1,3) - vp(1,1) );
        m_ViewPlanes[2].SetZ( vp(2,3) - vp(2,1) );
        m_ViewPlanes[2].SetW( vp(3,3) - vp(3,1) );
        //下侧
        m_ViewPlanes[3].SetX( vp(0,3) + vp(0,1) );
        m_ViewPlanes[3].SetY( vp(1,3) + vp(1,1) );
        m_ViewPlanes[3].SetZ( vp(2,3) + vp(2,1) );
        m_ViewPlanes[3].SetW( vp(3,3) + vp(3,1) );
        //Near
        m_ViewPlanes[4].SetX( vp(0,3) + vp(0,2) );
        m_ViewPlanes[4].SetY( vp(1,3) + vp(1,2) );
        m_ViewPlanes[4].SetZ( vp(2,3) + vp(2,2) );
        m_ViewPlanes[4].SetW( vp(3,3) + vp(3,2) );
        //Far
        m_ViewPlanes[5].SetX( vp(0,3) - vp(0,2) );
        m_ViewPlanes[5].SetY( vp(1,3) - vp(1,2) );
        m_ViewPlanes[5].SetZ( vp(2,3) - vp(2,2) );
        m_ViewPlanes[5].SetW( vp(3,3) - vp(3,2) );
    }

    bool ViewCull(const Vector4f &v1,const Vector4f &v2,const Vector4f &v3)
    {
        Vector4f w1 = m_ModelMatrix * v1;
        Vector4f w2 = m_ModelMatrix * v2;
        Vector4f w3 = m_ModelMatrix * v3;
        
        Vector4f minPoint, maxPoint;
        minPoint.SetX( fmin(w1.GetX(), fmin(w2.GetX(), w3.GetX())) );
        minPoint.SetY( fmin(w1.GetY(), fmin(w2.GetY(), w3.GetY())) );
        minPoint.SetZ( fmin(w1.GetZ(), fmin(w2.GetZ(), w3.GetZ())) );
        maxPoint.SetX( fmax(w1.GetX(), fmax(w2.GetX(), w3.GetX())) );
        maxPoint.SetY( fmax(w1.GetY(), fmax(w2.GetY(), w3.GetY())) );
        maxPoint.SetZ( fmax(w1.GetZ(), fmax(w2.GetZ(), w3.GetZ())) );
        // Near 和 Far 剔除时只保留完全在内的
        if (!Point2Plane(minPoint, m_ViewPlanes[4]) || !Point2Plane(maxPoint, m_ViewPlanes[4])) {
            return false;
        }
        if (!Point2Plane(minPoint, m_ViewPlanes[5]) || !Point2Plane(maxPoint, m_ViewPlanes[5])) {
            return false;
        }
        if (!Point2Plane(minPoint, m_ViewPlanes[0]) && !Point2Plane(maxPoint, m_ViewPlanes[0])) {
            return false;
        }
        if (!Point2Plane(minPoint, m_ViewPlanes[1]) && !Point2Plane(maxPoint, m_ViewPlanes[1])) {
            return false;
        }
        if (!Point2Plane(minPoint, m_ViewPlanes[2]) && !Point2Plane(maxPoint, m_ViewPlanes[2])) {
            return false;
        }
        if (!Point2Plane(minPoint, m_ViewPlanes[3]) && !Point2Plane(maxPoint, m_ViewPlanes[3])) {
            return false;
        }
        return true;
    }
    
private:
    Matrix4x4f m_ModelMatrix;
    Matrix4x4f m_ViewMatrix;
    Matrix4x4f m_ProjectMatrix;
    
    Texture m_Sample2D;
    
    std::vector<Vector4f> m_ViewPlanes;
};



#endif /* Shader_hpp */
