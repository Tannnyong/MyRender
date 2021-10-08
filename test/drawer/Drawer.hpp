//
//  Drawer.hpp
//  test
//
//  Created by 谭勇 on 2021/9/29.
//

#ifndef Drawer_hpp
#define Drawer_hpp

#include <stdio.h>
#include <unistd.h>
#include <chrono>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "../shader/Shader.hpp"
#include "../buffer/FrameBuffer.hpp"
#include "../vertex/Vertex.hpp"
#include "../rasterization/Rasterization.hpp"
#include "../mesh/MeshData.hpp"


class Drawer
{

public:
    Drawer():m_Buffer(nullptr),
            m_Shader(nullptr),
            m_Rasterization(nullptr),
            m_IsInit(false),
            m_Width(WINDOW_WIDTH),
            m_Height(WINDOW_HEIGHT),
            m_FPSIndex(0),
            m_TimeIndex(-1)
    {
    }
    ~Drawer()
    {
        _Release();
    }
    
    void Init(const int& w,const int& h)
    {
        _Release();
        m_Width = w;
        m_Height = h;
        m_Shader = new Shader();
        m_Shader->LoadTexture("");
        m_Buffer = new FrameBuffer(m_Width,m_Height);
        m_Rasterization = new Rasterization();
        m_Rasterization->SetFrameBufferAndShader(m_Buffer, m_Shader);
        m_IsInit = true;
    }
    
    void SetModelMatrix(const Matrix4x4f& model)
    {
        if(!m_IsInit) return;
        m_Shader->SetModelMatrix(model);
    }
    void SetViewMatrix(const Matrix4x4f& view)
    {
        if(!m_IsInit) return;
        m_Shader->SetViewMatrix(view);
    }
    void SetProjectMatrix(const Matrix4x4f& projection)
    {
        if(!m_IsInit) return;
        m_Shader->SetProjectMatrix(projection);
    }
    
    void ClearBuffer(const  Vector4f&color)
    {
        if(!m_IsInit) return;
        m_Buffer->ClearColorBuffer(color);
        m_Buffer->ClearDepthBuffer();
        
    }
    
    void Show()
    {
        if(!m_IsInit) return;
        
        uchar* ptr = (uchar*)m_Buffer->GetDataPtr();
        cv::Mat image(m_Height,m_Width,CV_8UC4,ptr);
        cv::imshow("test",image);

        _CalculatFPS();
    }
    
    void DrawTriangle(const Vertex &v1,const Vertex &v2,const Vertex &v3)
    {
        if(!m_IsInit) return;
        
        //视锥体剔除
        m_Shader->UpdateViewPlanes();
        if (!m_Shader->ViewCull(v1.m_Position,v2.m_Position,v3.m_Position))
        {
            return;
        }
        
        //顶点着色器处理
        ShaderData data1 = m_Shader->VertexShaderProcess(v1);
        ShaderData data2 = m_Shader->VertexShaderProcess(v2);
        ShaderData data3 = m_Shader->VertexShaderProcess(v3);
        
        
        ShaderData::PerspectiveDivision(data1);
        ShaderData::PerspectiveDivision(data2);
        ShaderData::PerspectiveDivision(data3);
        
        //背面剔除
        if(!_FaceCulling(data1,data2,data3))
        {
            return;
        }
        
        //转换为屏幕坐标
        Matrix4x4f viewportMat = ShaderData::GetViewPortMatrix(0, 0, m_Width, m_Height);
        data1.m_PreviewPos = viewportMat * data1.m_PreviewPos;
        data2.m_PreviewPos = viewportMat * data2.m_PreviewPos;
        data3.m_PreviewPos = viewportMat * data3.m_PreviewPos;
        
        //光栅化
        m_Rasterization->ScanLineTriangle(data1, data2, data3);
    }
    
    void DrawAxis()
    {
        if(!m_IsInit) return;
        Matrix4x4f viewportMat = ShaderData::GetViewPortMatrix(0, 0, m_Width, m_Height);
        
        Vertex v1;
        v1.m_Position = Vector4f(1.0f,0.0f,0.0f,1.0f);
        v1.m_Color = Vector4f(255.0f,0.0f,0.0f,0.0f);

        Vertex v2;
        v2.m_Position = Vector4f(0.0f,1.0f,0.0f,1.0f);
        v2.m_Color = Vector4f(0.0f,255.0f,0.0f,0.0f);

        Vertex v3;
        v3.m_Position = Vector4f(0.0f,0.0f,1.0f,1.0f);
        v3.m_Color = Vector4f(0.0f,0.0f,255.0f,0.0f);
        
        Vertex v4;
        v4.m_Position = Vector4f(0.0f,0.0f,0.0f,1.0f);
        v4.m_Color = Vector4f(0.0f,0.0f,0.0f,0.0f);
        
        ShaderData data1 = m_Shader->VertexShaderProcess(v1);
        ShaderData data2 = m_Shader->VertexShaderProcess(v2);
        ShaderData data3 = m_Shader->VertexShaderProcess(v3);
        ShaderData data4 = m_Shader->VertexShaderProcess(v4);
        
        ShaderData::PerspectiveDivision(data1);
        ShaderData::PerspectiveDivision(data2);
        ShaderData::PerspectiveDivision(data3);
        ShaderData::PerspectiveDivision(data4);
        
        
        data1.m_PreviewPos = viewportMat * data1.m_PreviewPos;
        data2.m_PreviewPos = viewportMat * data2.m_PreviewPos;
        data3.m_PreviewPos = viewportMat * data3.m_PreviewPos;
        data4.m_PreviewPos = viewportMat * data4.m_PreviewPos;
        
        m_Rasterization->ScanAxis(data1, data2, data3, data4);
    }
    
    void DrawMesh(const MeshData &mesh)
    {
        if(!m_IsInit) return;
        
        const std::vector<Vertex>& vbo = mesh.GetVBO();
        const std::vector<unsigned int>& ebo = mesh.GetEBO();
        if (ebo.empty())
        {
            return;
        }
        for (int i = 0; i < ebo.size(); i += 3)
        {
            Vertex p1, p2, p3;
            p1 = vbo[ebo[i]];
            p2 = vbo[ebo[i+1]];
            p3 = vbo[ebo[i+2]];
            p1.m_Position.SetW(1.0f);
            p2.m_Position.SetW(1.0f);
            p3.m_Position.SetW(1.0f);
            
            DrawTriangle(p1,p2,p3);
        }
    }
    
private:
    void _Release()
    {
        if(m_Buffer) delete m_Buffer;
        if(m_Shader) delete m_Shader;
        if(m_Rasterization) delete m_Rasterization;
        m_IsInit = false;
    }
    
    void _CalculatFPS()
    {
        m_FPSIndex++;
        long tmp = std::chrono::system_clock::now().time_since_epoch().count();;
        if(m_FPSIndex == -1)
        {
            m_TimeIndex = tmp;
        }
        if(tmp - m_TimeIndex >= 1000000)
        {
            m_TimeIndex = tmp;
            std::cout<< "FPS = " << m_FPSIndex << std::endl;
            m_FPSIndex = 0;
        }
    }
    
    bool _FaceCulling(const ShaderData &v1,const ShaderData &v2,const ShaderData &v3)
    {
        Vector4f tmp1 = v2.m_PreviewPos - v1.m_PreviewPos;
        Vector4f tmp2 = v3.m_PreviewPos - v1.m_PreviewPos;

        float x = tmp1.GetY() * tmp2.GetZ() - tmp1.GetZ() * tmp2.GetY();
        float y = tmp1.GetZ() * tmp2.GetX() - tmp1.GetX() * tmp2.GetZ();
        float z = tmp1.GetX() * tmp2.GetY() - tmp1.GetY() * tmp2.GetX();
        
        Vector3f norm(x,y,z);
        norm.Normalize();
        
        Vector3f dview(0.0f,0.0f,-1.0f);
        float res = Vector3f::VecDot(norm, dview);
        return res < 0.0f;
    }
    
private:
    bool m_IsInit;
    int m_Width;
    int m_Height;
    FrameBuffer* m_Buffer;
    Shader* m_Shader;
    Rasterization* m_Rasterization;
    
    long m_FPSIndex;
    long m_TimeIndex;
};

#endif /* Drawer_hpp */
