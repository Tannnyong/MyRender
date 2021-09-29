//
//  Vertex.hpp
//  test
//
//  Created by 谭勇 on 2021/9/28.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include "../math/Math.hpp"

class Vertex
{
public:
    //顶点坐标
    Vector4f m_Position;
    //顶点颜色
    Vector4f m_Color;
    
public:
    
    Vertex()=default;
    ~Vertex()=default;
};



#endif /* Vertex_hpp */
