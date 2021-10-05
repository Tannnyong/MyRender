//
//  main.cpp
//  test
//
//  Created by 谭勇 on 2021/9/28.
//
#include <iostream>
#include "string"
#include <unistd.h>

#include "../drawer/Drawer.hpp"
#include "../transform/MatTransform.hpp"
#include "../mesh/MeshData.hpp"

int main(int argc, const char * argv[]) {
    
    Drawer* drawer = new Drawer();
    drawer->Init(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    Vertex v1;
    v1.m_Position = Vector4f(-0.5f,-0.5f,0.0f,1.0f);
    v1.m_Color = Vector4f(255.0f,0.0f,0.0f,0.0f);

    Vertex v2;
    v2.m_Position = Vector4f(0.5f,-0.5f,0.0f,1.0f);
    v2.m_Color = Vector4f(0.0f,255.0f,0.0f,0.0f);

    Vertex v3;
    v3.m_Position = Vector4f(0.0f,0.5f,0.0f,1.0f);
    v3.m_Color = Vector4f(0.0f,0.0f,255.0f,0.0f);
    
    Matrix4x4f modelMat;
    Vector4f rotateAxis;
    //rotateAxis.SetX(1.0f);
    rotateAxis.SetY(1.0f);
    MatTransform::GetInstance().InitRotationAxisMat(modelMat, 30.0f, rotateAxis);
    
    Matrix4x4f viewMat;
    Vector3f pos(0.0f,0.0f,3.0f);
    Vector3f traget(0.0f,0.0f,-1.0f);
    Vector3f up(0.0f,1.0f,0.0f);
    MatTransform::GetInstance().InitViewMatrix(viewMat, pos,traget,up);
    
    Matrix4x4f projMat;
    MatTransform::GetInstance().InitPersProjMatrix(projMat, 60.0f, (float) WINDOW_WIDTH / WINDOW_HEIGHT, 0.3f, 1000.f);
    
    
    MeshData mesh;
    mesh.CreatBox(Vector4f(0.0f,0.0f,0.0f,0.0f), 0.5f);
    
    //mesh.AddTriangle(v1,v2,v3);
    
//    drawer->SetModelMatrix(modelMat);
    drawer->SetViewMatrix(viewMat);
    drawer->SetProjectMatrix(projMat);
    
    Vector4f clearColor(0.0f,0.0f,0.0f,0.0f);
    
    float angle = 0.0f;
    while (true)
    {
        drawer->ClearBuffer(clearColor);
        
        MatTransform::GetInstance().InitRotationAxisMat(modelMat, angle, rotateAxis);
        drawer->SetModelMatrix(modelMat);
        
        angle += 1.0f;
        
        //drawer->DrawTriangle(v1, v2, v3);
        drawer->DrawMesh(mesh);
        drawer->DrawAxis();
        
        drawer->Show();
    }
    
    cv::waitKey(0);
    return 0;
}
