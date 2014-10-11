#include "pildiT66tlus.h"
#include "pall.h"

int main(){
    std::vector<std::vector <cv::Point>> contours;
    std::vector<Pall> pallid;
    int lowerB,lowerG,lowerR,upperB,upperG,upperR,kersize,kdev = parameetrid(); //esc lõpetamiseks
    while(true){
        contours = kontuurid(lowerB,lowerG,lowerR,upperB,upperG,upperR,kersize,kdev);
        pallid = palliSort(contours);
        
        for (int i=0; i<pallid.size(); i++) {
            std::cout << pallid.size() << std::endl;
            std::cout << pallid[i] << std::endl;
        }
        if(cv::waitKey(1) >= 0){
            break;
        }
}
    return 0;
}