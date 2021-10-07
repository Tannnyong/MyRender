//
//  MeshData.hpp
//  test
//
//  Created by 谭勇 on 2021/9/29.
//

#ifndef MeshData_hpp
#define MeshData_hpp

#include <stdio.h>
#include "../vertex/Vertex.hpp"
#include "../math/Math.hpp"

class MeshData
{
public:
    MeshData() = default;
    ~MeshData() = default;
    
    
    const std::vector<Vertex>& GetVBO() const
    {
        return m_VBO;
    }
    
    const std::vector<unsigned int>& GetEBO() const
    {
        return m_EBO;
    }
    
    void AddMesh(const MeshData& mesh)
    {
        unsigned int offset = (unsigned int)m_VBO.size();
        const std::vector<Vertex>& vbo = mesh.GetVBO();
        const std::vector<unsigned int>& ebo = mesh.GetEBO();
        m_VBO.insert(m_VBO.end(),vbo.begin(), vbo.end());
        for (int i = 0; i < ebo.size(); i++)
        {
            m_EBO.push_back(ebo[i] + offset);
        }
    }
    
    void AddTriangle(const Vertex&v1, const Vertex&v2, const Vertex&v3)
    {
        unsigned int offset = (unsigned int)m_VBO.size();
        m_VBO.push_back(v1);
        m_VBO.push_back(v2);
        m_VBO.push_back(v3);

        m_EBO.push_back(offset);
        m_EBO.push_back(offset + 1);
        m_EBO.push_back(offset + 2);
    }
    
    void AddVertexs(const std::vector<Vertex>& vertexs)
    {
        m_VBO.insert(m_VBO.end(),vertexs.begin(), vertexs.end());
    }
    
    void AddIndexs(const std::vector<unsigned int>& indexs)
    {
        unsigned int offset = (unsigned int)m_EBO.size();
        for (int i = 0; i < indexs.size(); i++)
        {
            m_EBO.push_back(indexs[i] + offset);
        }
    }
    
    void CreatBox(const Vector4f& center,float radius)
    {
        MeshData front = CreatePlane(
                center + Vector4f(-radius, radius, radius,0.0f),
                center + Vector4f(-radius, -radius, radius,0.0f),
                center + Vector4f(radius, -radius, radius,0.0f),
                center + Vector4f(radius, radius, radius,0.0f)
            );
        AddMesh(front);

        MeshData left = CreatePlane(
                center + Vector4f(-radius, radius, -radius,0.0f),
                center + Vector4f(-radius, -radius, -radius,0.0f),
                center + Vector4f(-radius, -radius, radius,0.0f),
                center + Vector4f(-radius, radius, radius,0.0f)
            );
        AddMesh(left);

        MeshData right = CreatePlane(
                center + Vector4f(radius, radius, radius,0.0f),
                center + Vector4f(radius, -radius, radius,0.0f),
                center + Vector4f(radius, -radius, -radius,0.0f),
                center + Vector4f(radius, radius, -radius,0.0f)
            );
        AddMesh(right);

        MeshData back = CreatePlane(
                center + Vector4f(radius, radius, -radius,0.0f),
                center + Vector4f(radius, -radius, -radius,0.0f),
                center + Vector4f(-radius, -radius, -radius,0.0f),
                center + Vector4f(-radius, radius, -radius,0.0f)
            );
        AddMesh(back);

        MeshData up = CreatePlane(
                center + Vector4f(-radius, radius, -radius,0.0f),
                center + Vector4f(-radius, radius, radius,0.0f),
                center + Vector4f(radius, radius, radius,0.0f),
                center + Vector4f(radius, radius, -radius,0.0f)
            );
        AddMesh(up);

        MeshData down = CreatePlane(
                center + Vector4f(-radius, -radius, radius,0.0f),
                center + Vector4f(-radius, -radius, -radius,0.0f),
                center + Vector4f(radius, -radius, -radius,0.0f),
                center + Vector4f(radius, -radius, radius,0.0f)
            );
        AddMesh(down);
    }
    
    //输入四个点坐标，分别是左上 左下 右下 右上
    MeshData CreatePlane(const Vector4f& leftTop,
                         const Vector4f& leftBottom,
                         const Vector4f& rightBottom,
                         const Vector4f& rightTop)
    {
        std::vector<Vertex> vertexs;
        vertexs.push_back(Vertex(leftTop,Vector4f(255.0f, 0.0f,0.0f,255.0f),Vector2f(0.0f,1.0f)));
        vertexs.push_back(Vertex(rightTop,Vector4f(0.0f, 255.0f,0.0f,255.0f),Vector2f(1.0f,1.0f)));
        vertexs.push_back(Vertex(rightBottom,Vector4f(0.0f, 0.0f,255.0f,255.0f),Vector2f(1.0f,0.0f)));
        vertexs.push_back(Vertex(leftBottom,Vector4f(255.0f, 0.0f,255.0f,255.0f),Vector2f(0.0f,0.0f)));
        
        std::vector<unsigned int> indexs{0,2,1,0,3,2};
        
        MeshData result;
        result.AddVertexs(vertexs);
        result.AddIndexs(indexs);
        return result;
    }
    
private:
    std::vector<Vertex> m_VBO;
    std::vector<unsigned int> m_EBO;
    
    
};
#endif /* MeshData_hpp */
