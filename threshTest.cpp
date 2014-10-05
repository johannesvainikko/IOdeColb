#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;


int mainalt() {
	namedWindow("video", 1);
	namedWindow("Original", 1);
	
	VideoCapture cap(0);
	Mat frame, hsv;
	Mat ranged;
	int lowerH = 100;
	int lowerS = 100;
	int lowerV = 100;
	
	int upperH = 200;
	int upperS = 200;
	int upperV = 200;
	
	createTrackbar("Lower Hue", "video", &lowerH, 255);
	createTrackbar("Upper Hue", "video", &upperH, 255);
	
	createTrackbar("Lower Sat", "video", &lowerS, 255);
	createTrackbar("Upper Sat", "video", &upperS, 255);
	createTrackbar("Lower Value", "video", &lowerV, 255);
	createTrackbar("Upper Value", "video", &upperV, 255); 
	
	while (true) {
		cap >> frame;
		imshow("Original", frame);
		
		cvtColor(frame, hsv, CV_BGR2HSV);
		//assert(frame.type() == CV_8UC3);
		
		inRange(frame, Scalar(lowerH, lowerS, lowerV), Scalar(upperH, upperS, upperV), ranged);
		
		
		imshow("video", ranged);
		
		if(waitKey(1) >= 0) break;
	}

	return 0;
}
