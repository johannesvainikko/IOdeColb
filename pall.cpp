#include "pall.h"

Pall::Pall(int nsuurus, int nx, int ny)
:suurus{nsuurus},
x{nx},
y{ny}
{
}

std::ostream& operator<<(std::ostream& out, const Pall& p){
    out << "suurus: " << p.suurus << "\n keskpunkt: " << p.x << " " << p.y << std::endl;
    return out;
}


