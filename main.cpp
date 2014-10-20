#include "pildiT66tlus.h"
#include "pall.h"
#include "globals.h"
#include "RobotManager.hpp"

int main(int argC, char *argV[]){
	
	RobotManager manager;
    manager.initSerial();
	
    bool isPall;
    int suurimI;
    cv::Mat frame;
    std::vector<std::vector <cv::Point> > contours;
    std::vector<Pall> pallid;
    g_ksize=11;
    g_kdev=4;
    g_cap >> frame;
    cv::Size su = frame.size();
    g_width = su.width/2;
    g_height = su.height;
    parameetrid(); //esc lõpetamiseks
    
    if (argC>1){
        if (strcmp(argV[1], "-computer")==0) {
            cv::namedWindow("test", 1);
            std::string punktS;
            cv::Point punkt;
            while(true){
                contours = kontuurid();
                pallid = palliSort(contours);
                
                g_cap >> frame;
                //cv::flip(frame,frame,1);
                isPall= onPall(pallid);
                suurimI=suurim(pallid);
                if (isPall) {
                    punkt=cv::Point(pallid[suurimI].x,pallid[suurimI].y);
                    punktS = std::to_string(pallid[suurimI].x)+" "+std::to_string(pallid[suurimI].y);
                    cv::circle(frame, punkt, 1, cv::Scalar(0,0,255));
                    cv::putText(frame, punktS, punkt, 1, 1, cv::Scalar(0,0,255));
                    
                    cv::line(frame, cv::Point(g_width-g_dev,0), cv::Point(g_width-g_dev,g_height), cv::Scalar(0,0,255));
                    cv::line(frame, cv::Point(g_width+g_dev,0), cv::Point(g_width+g_dev,g_height), cv::Scalar(0,0,255));
                    if((g_width-g_dev)<(pallid[suurimI].x)){
                        if ((pallid[suurimI].x)<(g_width+g_dev)) {
                            std::cout << "otse" << std::endl;
                        }
                        else{
                            std::cout << "vasak" << std::endl;
                        }
                    }
                    else{
                        std::cout << "parem" << std::endl;
                    }
                }
                cv::imshow("test", frame);
                if(cv::waitKey(1) >= 0){
                    cv::destroyAllWindows();
                }
            }
        }
    }
    
    else{
        while(true){
            contours = kontuurid();
            pallid = palliSort(contours);
            
            isPall= onPall(pallid);
            if (isPall) {
                suurimI=suurim(pallid);
                if((g_width-g_dev)<(pallid[suurimI].x)){
                    if ((pallid[suurimI].x)<(g_width+g_dev)) {
                        //move straight
                        manager.moveRobot(0, 20, 0);
                    }
                    else{
                        //turn left
                        manager.moveRobot(0, 0, -10);
                    }
                }
                else{
                    //turn right
                    manager.moveRobot(0, 0, 10);
                }
            }
        }
    }
    return 0;
}
