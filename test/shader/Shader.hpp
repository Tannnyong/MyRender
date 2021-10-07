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
    
    
private:
    Matrix4x4f m_ModelMatrix;
    Matrix4x4f m_ViewMatrix;
    Matrix4x4f m_ProjectMatrix;
    
    Texture m_Sample2D;
};



#endif /* Shader_hpp */
