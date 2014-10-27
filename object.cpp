#include "object.hpp"

Object::Object(int nsuurus, int nx, int ny)
:suurus(nsuurus),
x(nx),
y(ny)
{
}

Object::Object(int nsuurus, int nx, int ny, cv::Rect nrect)
:suurus(nsuurus),
x(nx),
y(ny),
rect(nrect)
{
}

std::ostream& operator<<(std::ostream& out, const Object& p){
    out << "suurus: " << p.suurus << "\n keskpunkt: " << p.x << " " << p.y << std::endl;
    return out;
}



