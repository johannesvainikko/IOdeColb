#include "globals.h"

int g_width=1000;
int g_height=1000;
int g_lowB=0;
int g_lowG=0;
int g_lowR=0;
int g_upB=255;
int g_upG=255;
int g_upR=255;
int g_ksize=11;
int g_kdev=4;
int g_dev=50;
cv::VideoCapture g_cap(0);
cv::Mat g_elemDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
cv::Mat g_elemErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9,9));
