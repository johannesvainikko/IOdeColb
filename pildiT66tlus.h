#ifndef pildiT66tlus
#define pildiT66tlus

#include "pall.h"
#include "globals.h"
#include <opencv2/opencv.hpp>

/*sisse BGR vahemik, välja kontuuride massiiv*/
std::vector<std::vector <cv::Point> > kontuurid();

//sisse kontuurid, välja pallide vektor
std::vector<Pall> palliSort(std::vector<std::vector <cv::Point> > contours);

//värvide ja muude paikapanekuks
void parameetrid();
#endif