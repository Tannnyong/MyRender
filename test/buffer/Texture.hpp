//
//  Texture.hpp
//  test
//
//  Created by 谭勇 on 2021/10/5.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <string>
#include <iostream>

#include "opencv2/imgcodecs.hpp"

#include "../math/Math.hpp"

class Texture
{
public:
    Texture() = default;
    ~Texture() = default;
    
    bool Load(const std::string& path)
    {
        m_Image = cv::imread("./3.jpeg",1);
        if(m_Image.empty())
        {
            std::cout<<" Texture::Load Error! " << std::endl;
            return false;
        }
        m_Height = m_Image.rows;
        m_Width = m_Image.cols;
        m_Channels = m_Image.channels();
        return true;
    }
    
    Vector4f Sample2D(const float& x, const float& y)
    {
        //GL_REPEAT
        int ximg = (x - (float)floor(x)) * (m_Width-1);
        int yimg = (y - (float)floor(y)) * (m_Height-1);

        Vector4f ret;
        if (ximg < 0 || ximg > m_Width || yimg < 0 || yimg > m_Height)
        {
            return ret;
        }
        
        int xy = m_Channels * (yimg * m_Width + ximg);
        switch (m_Channels) {
            case 1:
                ret.SetX(m_Image.at<float>(xy));
                break;
            case 2:
                ret.SetX(m_Image.at<float>(xy));
                ret.SetY(m_Image.at<float>(xy+1));
                break;
            case 3:
                ret.SetX(*(m_Image.data + xy));
                ret.SetY(*(m_Image.data + xy+1));
                ret.SetZ(*(m_Image.data + xy+2));
                break;
            case 4:
                ret.SetX(m_Image.at<float>(xy));
                ret.SetY(m_Image.at<float>(xy+1));
                ret.SetZ(m_Image.at<float>(xy+2));
                ret.SetW(m_Image.at<float>(xy+3));
                break;
            default:
                break;
        }
        
        return ret;
    }
    
private:
    int m_Width;
    int m_Height;
    int m_Channels;
    cv::Mat m_Image;
};


#endif /* Texture_hpp */
