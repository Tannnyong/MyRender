//
//  Shader.hpp
//  test
//
//  Created by 谭勇 on 2021/9/28.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
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
    
    ShaderData VertexShaderProcess(const Vertex& vertex)
    {
        ShaderData dataout;
        
//        Matrix4x4f test;
//        test(1,3) = -1.0f;
//        test(2,2) = -1.0f;
//        test(2,3) = -10.0f;
//
        Matrix4x4f temp = m_ProjectMatrix * m_ViewMatrix * m_ModelMatrix;
        dataout.m_PreviewPos = temp * vertex.m_Position;
        dataout.m_Color = vertex.m_Color;
        return dataout;
    }
    
    Vector4f FragmentShaderProcess(ShaderData& datain)
    {
        return datain.m_Color;
    }
    
    
private:
    Matrix4x4f m_ModelMatrix;
    Matrix4x4f m_ViewMatrix;
    Matrix4x4f m_ProjectMatrix;
};



#endif /* Shader_hpp */
