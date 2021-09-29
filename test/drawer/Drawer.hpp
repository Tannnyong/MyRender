//
//  Drawer.hpp
//  test
//
//  Created by 谭勇 on 2021/9/29.
//

#ifndef Drawer_hpp
#define Drawer_hpp

#include <stdio.h>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "../shader/Shader.hpp"
#include "../buffer/FrameBuffer.hpp"
#include "../vertex/Vertex.hpp"
#include "../rasterization/Rasterization.hpp"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Drawer
{

public:
    Drawer():m_Buffer(nullptr),
            m_Shader(nullptr),
            m_Rasterization(nullptr),
            m_IsInit(false),
            m_Width(WINDOW_WIDTH),
            m_Height(WINDOW_HEIGHT)
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
        m_Buffer->Clear(color);
    }
    
    void Show()
    {
        if(!m_IsInit) return;
        uchar* ptr = (uchar*)m_Buffer->GetDataPtr();
        cv::Mat image(m_Height,m_Width,CV_8UC4,ptr);
        cv::imshow("test",image);
        cv::waitKey(0);
    }
    
    void DrawTriangle(const Vertex &v1,const Vertex &v2,const Vertex &v3)
    {
        if(!m_IsInit) return;
        Matrix4x4f viewportMat = ShaderData::GetViewPortMatrix(0, 0, m_Width, m_Height);
        
        ShaderData data1 = m_Shader->VertexShaderProcess(v1);
        ShaderData data2 = m_Shader->VertexShaderProcess(v2);
        ShaderData data3 = m_Shader->VertexShaderProcess(v3);
        
        data1.m_PreviewPos = viewportMat * data1.m_PreviewPos;
        data2.m_PreviewPos = viewportMat * data2.m_PreviewPos;
        data3.m_PreviewPos = viewportMat * data3.m_PreviewPos;

        m_Rasterization->ScanLineTriangle(data1, data2, data3);
    }
    
private:
    void _Release()
    {
        if(m_Buffer) delete m_Buffer;
        if(m_Shader) delete m_Shader;
        if(m_Rasterization) delete m_Rasterization;
        m_IsInit = false;
    }
    
private:
    bool m_IsInit;
    int m_Width;
    int m_Height;
    FrameBuffer* m_Buffer;
    Shader* m_Shader;
    Rasterization* m_Rasterization;

};

#endif /* Drawer_hpp */
