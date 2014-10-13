#include "pildiT66tlus.h"
#include "pall.h"


int mainALT(){
    int * lowerB;
    int * lowerG;
    int * lowerR;
    int * upperB;
    int * upperG;
    int * upperR;
    int * ksize;
    int * kdev;
    int * height;
    int * width;
    
    int lowerBv=255;
    int lowerGv=255;
    int lowerRv=255;
    int upperBv=255;
    int upperGv=255;
    int upperRv=255;
    int ksizev=11;
    int kdevv=4;
    int heightv=2000;
    int widthv=2000;
    
    lowerB=&lowerBv;
    lowerG=&lowerGv;
    lowerR=&lowerRv;
    upperB=&upperBv;
    upperG=&upperGv;
    upperR=&upperRv;
    ksize=&ksizev;
    kdev=&kdevv;
    height=&heightv;
    width=&widthv;
    
    std::vector<std::vector <cv::Point>> contoursv;
    std::vector<Pall::Pall> pallidv;
    Pall::Pall suurimPallv;
    std::vector<std::vector <cv::Point>> * contours;
    std::vector<Pall::Pall> * pallid;
    Pall::Pall * suurimPall;
    
    contours=&contoursv;
    pallid=&pallidv;
    suurimPall=&suurimPallv;
    
    parameetrid(); //esc lõpetamiseks
    keskpunkt();
    
    while(true){
        kontuurid();
        palliSort();
        
        if (*pallid.size>0) {
            *suurimPall=Pall::Pall(0,0,0)
            for (int i=0; i<*pallid.size(); i++) {
                //
                std::cout << *pallid.size() << std::endl;
                std::cout << *pallid[i] << std::endl;
                //
                if((*pallid[i].suurus)>(*suurimPall.suurus)){
                    *suurimPall=*pallid[i];
                }
            }
            if((*width-200)<*suurimPall.X<(*width+200)){
                //move?
            }
        }
        
    }
    return 0;
}