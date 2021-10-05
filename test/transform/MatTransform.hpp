//
//  MatTransform.hpp
//  test
//
//  Created by 谭勇 on 2021/9/29.
//

#ifndef MatTransform_hpp
#define MatTransform_hpp

#include <stdio.h>
#include "../math/Math.hpp"

#define MATTRANSFORM_PI (3.14f)

class MatTransform
{
private:
    MatTransform() = default;
    ~MatTransform() = default;
    MatTransform(const MatTransform&) = delete;
    MatTransform& operator=(const MatTransform&) = delete;

public:
    static MatTransform& GetInstance()
    {
        static MatTransform instance;
        return instance;
    }
    
public:
    
    void InitRotationXMat(Matrix4x4f& mat, const float angle)
    {
        const float cosAngle = cos(angle / 180.0 * MATTRANSFORM_PI);
        const float sinAngle = sin(angle / 180.0 * MATTRANSFORM_PI);
        
        mat(1,1) = cosAngle;
        mat(1,2) = -sinAngle;
        mat(2,1) = sinAngle;
        mat(2,2) = cosAngle;
    }
    
    void InitRotationYMat(Matrix4x4f& mat, const float angle)
    {
        const float cosAngle = cos(angle / 180.0 * MATTRANSFORM_PI);
        const float sinAngle = sin(angle / 180.0 * MATTRANSFORM_PI);
        
        mat(0,1) = cosAngle;
        mat(0,2) = sinAngle;
        mat(2,0) = -sinAngle;
        mat(2,2) = cosAngle;
    }
    
    void InitRotationZMat(Matrix4x4f& mat, const float angle)
    {
        const float cosAngle = cos(angle / 180.0 * MATTRANSFORM_PI);
        const float sinAngle = sin(angle / 180.0 * MATTRANSFORM_PI);
        
        mat(0,1) = cosAngle;
        mat(0,2) = -sinAngle;
        mat(1,1) = sinAngle;
        mat(1,2) = cosAngle;
    }
    
    void InitRotationAxisMat(Matrix4x4f& mat, const float angle,const Vector4f& axis)
    {
        const float cosAngle = cos(angle / 180.0f * MATTRANSFORM_PI);
        const float sinAngle = sin(angle / 180.0f * MATTRANSFORM_PI);
        const float oneMinusCos = 1.0f - cosAngle;
        const float x = axis.GetX();
        const float y = axis.GetY();
        const float z = axis.GetZ();
        
        mat(0,0) = x * x * oneMinusCos + cosAngle;
        mat(0,1) = x * y * oneMinusCos - z * sinAngle;
        mat(0,2) = x * z * oneMinusCos + y * sinAngle;
        mat(1,0) = x * y * oneMinusCos + z * sinAngle;
        mat(1,1) = y * y * oneMinusCos + cosAngle;
        mat(1,2) = y * z * oneMinusCos - x * sinAngle;
        mat(2,0) = x * z * oneMinusCos - y * sinAngle;
        mat(2,1) = y * z * oneMinusCos + x * sinAngle;
        mat(2,2) = z * z * oneMinusCos + cosAngle;
    }
    
    void InitViewMatrix(Matrix4x4f& mat,Vector3f& pos,Vector3f& target,Vector3f& up)
    {
        Vector3f zAxis = pos - target;
        Vector3f xAxis;
        Vector3f yAxis;
        Vector3f::VecCross(up,zAxis,xAxis);
        Vector3f::VecCross(zAxis,xAxis,yAxis);
        
        xAxis.Normalize();
        yAxis.Normalize();
        zAxis.Normalize();
        
        
        mat(0,0) = xAxis.GetX(); mat(0,1) = xAxis.GetY(); mat(0,2) = xAxis.GetZ();
        mat(1,0) = yAxis.GetX(); mat(1,1) = yAxis.GetY(); mat(1,2) = yAxis.GetZ();
        mat(2,0) = zAxis.GetX(); mat(2,1) = zAxis.GetY(); mat(2,2) = zAxis.GetZ();
        
        mat(0,3) = -Vector3f::VecDot(xAxis, pos);
        mat(1,3) = -Vector3f::VecDot(yAxis, pos);
        mat(2,3) = -Vector3f::VecDot(zAxis, pos);
    }
    
    void InitPersProjMatrix(Matrix4x4f& mat,float fov,const float aspect,float zNear,float zFar)
    {
//        zNear = -zNear;
//        zFar = -zFar;
//
//        float top = zNear * tan(fov * 0.5f / 180.0 * MATTRANSFORM_PI);
//        float bottom = -top;
//        float right = top*aspect;
//        float left = -right;
//
//        mat(0,0) = 2.0f * zNear / (right - left);
//        mat(0,2) = (right + left) / (right - left);
//        mat(1,1) = 2.0f * zNear / (top - bottom);
//        mat(1,2) = (top + bottom) / (top - bottom);
//        mat(2,2) = -(zFar + zNear) / (zFar - zNear);
//        mat(2,3) = -2.0 * zFar * zNear / (zFar - zNear);
//        mat(3,2) = -1.0f;
//        mat(3,3) = 0.0f;
        
        const float zRange = zFar - zNear;
        const float tanHalfFov = tan(fov * 0.5f / 180.0 * MATTRANSFORM_PI);

        mat(0,0) = 1.0f / (aspect * tanHalfFov);
        mat(1,1) = 1.0f / tanHalfFov;
        mat(2,2) = -(zFar + zNear) / zRange;
        mat(3,2) = -1.0f;
        mat(2,3) = -2.0f * zNear * zFar / zRange;
        mat(3,3) = 0.0f;
    }
    
};



#endif /* MatTransform_hpp */
