#include "pildiT66tlus.h"

std::vector<std::vector <cv::Point>> kontuurid(int lowB, int lowG, int lowR, int upB, int upG, int upR, int ksize, int kdev) {
    cv::VideoCapture cap(0);
    cv::Mat frame;
    cv::Mat elemDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::Mat elemErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9,9));
    std::vector<std::vector <cv::Point>> contours;
    cv::Vector<cv::Vec4i> hierarchy;
    
    try {
        cap >> frame;
        //frame = cv::imread("/Users/reti/Dropbox/katsepilt.png"); testimiseks, don't mind this
        cv::GaussianBlur(frame, frame, cv::Size(ksize,ksize), kdev);
        cv::inRange(frame, cv::Scalar(lowB,lowG,lowR), cv::Scalar(upB,upG,upR), frame);
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

int parameetrid() {
    cv::namedWindow("video", 1);
    cv::namedWindow("original", 1);
    cv::VideoCapture cap(0);
    cv::Mat frame, binary;
    
    int lowerB, lowerG, lowerR, upperB, upperG, upperR = 0;
    
    int ksize=11;
    int kdev=4;
    
    cv::Mat elemDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::Mat elemErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9,9));
    
    lowerB = cv::createTrackbar("Lower B", "video", &lowerB, 255);
    upperB = cv::createTrackbar("Upper B", "video", &upperB, 255);
    lowerG = cv::createTrackbar("Lower G", "video", &lowerG, 255);
    upperG = cv::createTrackbar("Upper G", "video", &upperG, 255);
    lowerR = cv::createTrackbar("Lower R", "video", &lowerR, 255);
    upperR = cv::createTrackbar("Upper R", "video", &upperR, 255);
    
    /*cv::createTrackbar("Kernel size", "video", &ksize, 10);
    cv::createTrackbar("Kernel dev", "video", &kdev, 10);
    if (ksize % 2 == 0) {
        ksize++;
    }
    */
    
    while (true) {
        cap >> frame;
        imshow("original", frame);
        cv::GaussianBlur(frame, frame, cv::Size(ksize,ksize), kdev);
        cv::inRange(frame, cv::Scalar(lowerB, lowerG, lowerR), cv::Scalar(upperB, upperG, upperR), binary);
        cv::dilate(binary,binary,elemDilate);
        cv::erode(binary, binary,elemErode);
        imshow("video", binary);
        
        
        if(cv::waitKey(1) >= 0){
            cv::destroyAllWindows();
            break;
            }
    }
    return lowerB,lowerG,lowerR,upperB,upperG,upperR,ksize,kdev;
}






