#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

void pall(std::vector<std::vector <cv::Point>> contours){ /* hmmmm kuidas kõige targem info kättesaadavaks teha */
    std::vector<cv::Moments> mu(contours.size());
    for (int i=0; i<contours.size(); i++) {
        mu[i]=cv::moments(contours[i],false);
        std::cout<< "pall: " << i << std::endl;
        std::cout<< "keskpunkt: " << mu[i].m10/mu[i].m00 << " " << mu[i].m01/mu[i].m00 << std::endl;
        std::cout<< "suurus: " << mu[i].m00 << std::endl;
    }
    return;
}

int main() {
    cv::VideoCapture cap(0);
    cv::Mat frame;
    std::vector<std::vector <cv::Point>> contours;
    cv::Vector<cv::Vec4i> hierarchy;
    int lowerH, lowerS, lowerV, upperH, upperS, upperV;
    
    lowerH = 100;
    lowerS = 100;
    lowerV = 100;
    upperH = 200;
    upperS = 200;
    upperV = 200;
    
    while(true){
        cap >> frame;
        cv::cvtColor(frame, frame, CV_BGR2HSV);
        cv::inRange(frame, cv::Scalar(lowerH, lowerS, lowerV), cv::Scalar(upperH, upperS, upperV), frame);
        try {
            cv::findContours(frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        } catch (cv::Exception& e) {
            const char* err_msg = e.what();
            std::cout << err_msg << std::endl;
        }
        pall(contours);
        if(cv::waitKey(1) >= 0) break;
    }
    return 0;
}
