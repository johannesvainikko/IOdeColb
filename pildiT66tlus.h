#ifndef pildiT66tlus
#define pildiT66tlus

#include "pall.h"

/*sisse BGR vahemik, välja kontuuride massiiv*/
std::vector<std::vector <cv::Point>> kontuurid(int lowB, int lowG, int lowR, int upB, int upG, int upR, int ksize, int kdev);

//sisse kontuurid, välja pallide vektor
std::vector<Pall> palliSort(std::vector<std::vector <cv::Point>> contours);

//värvide ja muude paikapanekuks
int parameetrid();
#endif 