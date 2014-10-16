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

bool onPall(std::vector<Pall> palliV){
    if(palliV.size()>0) return true;
    else return false;
}

int suurim(std::vector<Pall> palliV){
    int loc = 0;
    Pall tyhi = Pall(0,0,0);
    for (int i=0; i<palliV.size(); i++) {
        if (palliV[i].suurus>tyhi.suurus) {
            tyhi=palliV[i];
            loc=i;
        }
    }
    return loc;
}


