//
//  main.cpp
//  Render
//
//  Created by 谭勇 on 2021/9/27.
//

#include <iostream>
#include "string"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"


int main(int arge, char** argv)
{
         cv::Mat img = cv::imread("../../1.jpeg",1);
         if (img.empty())
         {
                  printf("could not show image...");
                  return -1;
         }
         cv::imshow("test",img);
         cv::waitKey(0);
         return 0;
}
