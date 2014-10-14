#include "pildiT66tlus.h"
#include "pall.h"
#include "globals.h"

int main(){
    std::vector<std::vector <cv::Point>> contours;
    std::vector<Pall> pallid;
    g_ksize=11;
    g_kdev=4;
    parameetrid(); //esc lõpetamiseks
    while(true){
        contours = kontuurid();
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