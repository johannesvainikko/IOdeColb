#include "pildiT66tlus.h"

std::vector<std::vector <cv::Point>> kontuurid() {
    cv::Mat frame;
    cv::Mat elemDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::Mat elemErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9,9));
    std::vector<std::vector <cv::Point>> contours;
    cv::Vector<cv::Vec4i> hierarchy;
    //cv::VideoCapture cap(0);
    
    try {
        g_cap >> frame;
        //frame = cv::imread("/Users/reti/Dropbox/katsepilt.png"); testimiseks, don't mind this
        cv::GaussianBlur(frame, frame, cv::Size(g_ksize,g_ksize), g_kdev);
        cv::inRange(frame, cv::Scalar(g_lowB,g_lowG,g_lowR), cv::Scalar(g_upB,g_upG,g_upR), frame);
        cv::dilate(frame,frame,elemDilate);
        cv::erode(frame, frame,elemErode);
        cv::findContours(frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    }
    catch (cv::Exception& e) {
        const char* err_msg = e.what();
        std::cout << err_msg << std::endl;
    }
    return contours;
}

std::vector<Pall> palliSort(std::vector<std::vector <cv::Point>> contours){
    std::vector<Pall> pallid;
    std::vector<cv::Moments> mu(contours.size());
    int suurus,x,y;
    
    for (int i=0; i<contours.size(); i++) {
        mu[i]=cv::moments(contours[i],false);
        suurus=mu[i].m00;
        if(suurus>200){
            x=mu[i].m10/mu[i].m00;
            y=mu[i].m01/mu[i].m00;
            pallid.push_back(Pall(suurus, x, y));
        }
    }
    
    return pallid;
}

void parameetrid() {
    cv::namedWindow("video", 1);
    cv::namedWindow("original", 1);
    //cv::VideoCapture cap(0);
    cv::Mat frame, binary;
    
    cv::Mat elemDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::Mat elemErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9,9));
    
    cv::createTrackbar("Lower B", "video", &g_lowB, 255);
    cv::createTrackbar("Upper B", "video", &g_upB, 255);
    cv::createTrackbar("Lower G", "video", &g_lowG, 255);
    cv::createTrackbar("Upper G", "video", &g_upG, 255);
    cv::createTrackbar("Lower R", "video", &g_lowR, 255);
    cv::createTrackbar("Upper R", "video", &g_upR, 255);
    
    /*cv::createTrackbar("Kernel size", "video", &ksize, 10);
     cv::createTrackbar("Kernel dev", "video", &kdev, 10);
     if (ksize % 2 == 0) {
     ksize++;
     }
     */
    
    while (true) {
        g_cap >> frame;
        imshow("original", frame);
        cv::GaussianBlur(frame, frame, cv::Size(g_ksize,g_ksize), g_kdev);
        cv::inRange(frame, cv::Scalar(g_lowB, g_lowG, g_lowR), cv::Scalar(g_upB, g_upG, g_upR), binary);
        cv::dilate(binary,binary,elemDilate);
        cv::erode(binary, binary,elemErode);
        imshow("video", binary);
        
        
        if(cv::waitKey(1) >= 0){
            cv::destroyWindow("original");
            cv::destroyWindow("video");
            break;
        }
    }
    return;
}






