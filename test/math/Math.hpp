//
//  Math.hpp
//  test
//
//  Created by 谭勇 on 2021/9/28.
//

#ifndef Math_hpp
#define Math_hpp

#include <iostream>
#include <stdio.h>
#include "opencv2/core/matx.hpp"
#include "opencv2/core/mat.hpp"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 300

class Vector2f
{
public:
    Vector2f():m_Point(0.0f,0.0f){}
    Vector2f(float x,float y):m_Point(x,y){}
    Vector2f(const Vector2f& vec2)
    {
        m_Point[0] = vec2.GetX();
        m_Point[1] = vec2.GetY();
    }
    ~Vector2f()=default;
    
public:
    float GetX() const
    {
        return m_Point[0];
    };
    float GetY() const
    {
        return m_Point[1];
    };
    void SetX(float x)
    {
        m_Point[0] = x;
    };
    void SetY(float y)
    {
        m_Point[1] = y;
    };
    
    Vector2f operator+(const Vector2f& vec) const
    {
        return Vector2f(m_Point[0] + vec.GetX(),m_Point[1] + vec.GetY());
    }
    Vector2f operator-(const Vector2f& vec) const
    {
        return Vector2f(m_Point[0] - vec.GetX(),m_Point[1] - vec.GetY());
    }
    Vector2f operator*(float factor) const
    {
        return Vector2f(m_Point[0] * factor,m_Point[1] * factor);
    }
    Vector2f operator/(float factor) const
    {
        return Vector2f(m_Point[0] / factor,m_Point[1] / factor);
    }
    
    
private:
    cv::Vec2f m_Point;
};


class Vector3f
{
public:
    Vector3f():m_Point(0.0f,0.0f,0.0f){}
    Vector3f(float x,float y,float z):m_Point(x,y,z){}
    ~Vector3f()=default;
    
public:
    float GetX() const
    {
        return m_Point[0];
    };
    float GetY() const
    {
        return m_Point[1];
    };
    float GetZ() const
    {
        return m_Point[2];
    };
    void SetX(float x)
    {
        m_Point[0] = x;
    };
    void SetY(float y)
    {
        m_Point[1] = y;
    };
    void SetZ(float z)
    {
        m_Point[2] = z;
    };
    
    Vector3f operator-(const Vector3f& vec) const
    {
        return Vector3f(m_Point[0] - vec.GetX(),m_Point[1] - vec.GetY(),m_Point[2] - vec.GetZ());
    }
    
    
    static void VecCross(Vector3f& v1,Vector3f& v2,Vector3f& dst)
    {
        float x = v1.GetY() * v2.GetZ() - v1.GetZ() * v2.GetY();
        float y = v1.GetZ() * v2.GetX() - v1.GetX() * v2.GetZ();
        float z = v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX();
        dst.SetX(x);
        dst.SetY(y);
        dst.SetZ(z);
    }
    
    static float VecDot(Vector3f& v1,Vector3f& v2)
    {
        float ret = v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY() + v1.GetZ() * v2.GetZ();
        return ret;
    }
    
    void Normalize()
    {
        float len = sqrt(m_Point[0] * m_Point[0] + m_Point[1] * m_Point[1] + m_Point[2] * m_Point[2]);
        if(len > 0.0f)
        {
            m_Point[0] = m_Point[0] / len;
            m_Point[1] = m_Point[1] / len;
            m_Point[2] = m_Point[2] / len;
        }
    }
    
private:
    cv::Vec3f m_Point;
};


class Vector4f
{
public:
    Vector4f():m_Point(0.0f,0.0f,0.0f,0.0f){}
    Vector4f(float x,float y,float z,float w):m_Point(x,y,z,w){}
    Vector4f(const Vector4f& vec4)
    {
        m_Point[0] = vec4.GetX();
        m_Point[1] = vec4.GetY();
        m_Point[2] = vec4.GetZ();
        m_Point[3] = vec4.GetW();
    }
    ~Vector4f()=default;
    
public:
    float GetX() const
    {
        return m_Point[0];
    };
    float GetY() const
    {
        return m_Point[1];
    };
    float GetZ() const
    {
        return m_Point[2];
    };
    float GetW() const
    {
        return m_Point[3];
    };
    void SetX(float x)
    {
        m_Point[0] = x;
    };
    void SetY(float y)
    {
        m_Point[1] = y;
    };
    void SetZ(float z)
    {
        m_Point[2] = z;
    };
    void SetW(float w)
    {
        m_Point[3] = w;
    };
    
    void Normalize()
    {
        float len = sqrt(m_Point[0] * m_Point[0] + m_Point[1] * m_Point[1] + m_Point[2] * m_Point[2]);
        if(len > 0.0f)
        {
            m_Point[0] = m_Point[0] / len;
            m_Point[1] = m_Point[1] / len;
            m_Point[2] = m_Point[2] / len;
        }
    }
    
    
    Vector4f operator+(const Vector4f& vec) const
    {
        return Vector4f(m_Point[0] + vec.GetX(),m_Point[1] + vec.GetY(),m_Point[2] + vec.GetZ(),m_Point[3] + vec.GetW());
    }
    Vector4f operator-(const Vector4f& vec) const
    {
        return Vector4f(m_Point[0] - vec.GetX(),m_Point[1] - vec.GetY(),m_Point[2] - vec.GetZ(),m_Point[3] - vec.GetW());
    }
    Vector4f operator*(float factor) const
    {
        return Vector4f(m_Point[0] * factor,m_Point[1] * factor,m_Point[2] * factor,m_Point[3] * factor);
    }
    Vector4f operator/(float factor) const
    {
        return Vector4f(m_Point[0] / factor,m_Point[1] / factor,m_Point[2] / factor,m_Point[3] / factor);
    }
    
    std::vector<float> GetVectorData() const
    {
        return std::vector<float>{m_Point[0],m_Point[1],m_Point[2],m_Point[3]};
    }
    
    
private:
    cv::Vec4f m_Point;
};


class Matrix4x4f
{
public:
    Matrix4x4f()
    {
        m_Matrix = cv::Matx44f::eye();
    }
    Matrix4x4f(const std::vector<float>& vec):m_Matrix(vec.data())
    {
        if(vec.size() != 16)
        {
            //todo
        }
    }
    Matrix4x4f(const Matrix4x4f& mat):m_Matrix(mat.GetMatrixData()){}
    Matrix4x4f(const cv::Matx44f& mat):m_Matrix(mat){}
    
    cv::Matx44f GetMatrixData() const
    {
        return m_Matrix;
    }
    
    Matrix4x4f operator*(const Matrix4x4f& mat)
    {
        cv::Matx44f temp(mat.GetMatrixData());
        temp = m_Matrix*temp;
        Matrix4x4f ret(temp);
        return ret;
    }
    
    Vector4f operator*(const Vector4f& mat)
    {
        std::vector<float> vecdata = mat.GetVectorData();
        cv::Mat1f temp(vecdata);
        temp = m_Matrix * temp;
        vecdata = std::vector<float>(temp);
        Vector4f ret(vecdata[0],vecdata[1],vecdata[2],vecdata[3]);
        return ret;
    }
    
    float& operator()(int row,int col)
    {
        return m_Matrix(row,col);
    }
    
private:
    cv::Matx44f m_Matrix;
};


#endif /* Math_hpp */
