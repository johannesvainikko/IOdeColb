#ifndef pall
#define pall

#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

class Pall{
    
public:
    int suurus;
    int x;
    int y;
    Pall(int nsuurus, int nx, int ny);
    friend std::ostream& operator<<(std::ostream& out, const Pall& p);
};

extern std::vector<Pall> palliV;


#endif
