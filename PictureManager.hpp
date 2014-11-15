#ifndef PICTUREMANAGER_HPP_INCLUDED
#define PICTUREMANAGER_HPP_INCLUDED

#define KSIZE 11
#define KDEV 4
#define DEV 50

#define BALL 1
#define GOAL 2
#define YELLOW 3
#define BLUE 4

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
    int widthImg;
    int heightImg;
    int lowH_B;
    int lowS_B;
    int lowV_B;
    int upH_B;
    int upS_B;
    int upV_B;
    int lowH_G;
    int lowS_G;
    int lowV_G;
    int upH_G;
    int upS_G;
    int upV_G;
    Object largestB;
    Object largestG;
    cv::Mat frame;
    std::vector<std::vector <cv::Point> > contours_B;
    std::vector<std::vector <cv::Point> > contours_G;
    std::vector<Object> pallid;
    std::vector<Object> goal;
    cv::VideoCapture cap;
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
    
public:
    PictureManager(){};
    int dir;
    int maxGoalDist;
    bool isPall;
    bool isGoal;
    void init(int f, RobotManager *manager);
    void refresh(int f);
    void where(int f);
};


#endif 
