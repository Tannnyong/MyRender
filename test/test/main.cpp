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
    
    Vector4f clearColor(0.0f,0.0f,0.0f,0.0f);
    while (true)
    {
        drawer->ClearBuffer(clearColor);
        drawer->DrawTriangle(v1, v2, v3);
        drawer->Show();
    }
    
    cv::waitKey(0);
    return 0;
}
