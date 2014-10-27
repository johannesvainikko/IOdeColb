#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <iostream>
#include <opencv2/opencv.hpp>

class Object{
    
public:
    int suurus;
    int x;
    int y;
    cv::Rect rect;
    Object(int nsuurus, int nx, int ny, cv::Rect nrect);
    Object(int nsuurus, int nx, int ny);
    Object(){};
    friend std::ostream& operator<<(std::ostream& out, const Object& p);
};

#endif

