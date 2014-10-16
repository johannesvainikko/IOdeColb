#ifndef globals
#define globals
#include <opencv2/opencv.hpp>

extern int g_width;
extern int g_height;
extern int g_lowB;
extern int g_lowG;
extern int g_lowR;
extern int g_upB;
extern int g_upG;
extern int g_upR;
extern int g_ksize;
extern int g_kdev;
extern int g_dev;
extern cv::VideoCapture g_cap;
extern cv::Mat g_elemDilate;
extern cv::Mat g_elemErode;


#endif
