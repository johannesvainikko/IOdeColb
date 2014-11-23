#ifndef PICTUREMANAGER_HPP_INCLUDED
#define PICTUREMANAGER_HPP_INCLUDED

#define KSIZE 11
#define KDEV 4
#define DEV 66
#define EDSIZE 24

#define BALL 1
#define GOAL 2
#define YELLOW 3
#define BLUE 4
#define FIELD 5

#define LEFT 1
#define RIGHT 2
#define FORWARD 3
#define STOP 4

#include <opencv2/opencv.hpp>
#include "object.hpp"
#include <iostream>
#include <fstream>
#include "RobotManager.hpp"

class PictureManager {
    
private:
    
    int lowH_B;
    int lowS_B;
    int lowV_B;
    int upH_B;
    int upS_B;
    int upV_B;
    int lowH_F;
    int lowS_F;
    int lowV_F;
    int upH_F;
    int upS_F;
    int upV_F;
    int lowH_G;
    int lowS_G;
    int lowV_G;
    int upH_G;
    int upS_G;
    int upV_G;
    
    cv::Mat frame;
    std::vector<std::vector <cv::Point> > contours_B;
    std::vector<std::vector <cv::Point> > contours_G;
    std::vector<Object> pallid;
    std::vector<Object> goal;
    cv::VideoCapture cap;
    cv::VideoWriter video;
    cv::Mat elemDilate;
    cv::Mat elemErode;
    void paramFromFile(int f);
    void paramToFile(int f);
    void parameetrid(int f, RobotManager *manager);
    void contourFinder(int f);
    void objectSort(int f);
    void largest(int f);
    void isObjectF(int f);
    void clear(int f);
    void fieldmask();
    
public:
	int widthImg;
    int heightImg;
	Object largestB;
    Object largestG;
    PictureManager(){};
    int dir;
    int maxGoalDist;
    bool isPall;
    bool isGoal;
    bool isBallForward();
    void init(int f, RobotManager *manager);
    void refresh(int f);
    void where(int f);
};


#endif 
