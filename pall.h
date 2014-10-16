#ifndef pall
#define pall

#include <iostream>
#include <opencv2/opencv.hpp>

class Pall{
    
public:
    int suurus;
    int x;
    int y;
    Pall(int nsuurus, int nx, int ny);
    friend std::ostream& operator<<(std::ostream& out, const Pall& p);
};

bool onPall(std::vector<Pall> palliV);

//asukoht vektoris
int suurim(std::vector<Pall> palliV);

#endif

