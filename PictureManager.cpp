#include "PictureManager.hpp"
#include <string>

void PictureManager::init(int f, RobotManager *manager){
    cap.open(0);
    cap >> frame;
    cv::Size su = frame.size();
    widthImg = (su.width)/2;
    heightImg = su.height;
    elemDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    elemErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9,9));
    paramFromFile(f);
    parameetrid(BALL, manager);
    parameetrid(GOAL, manager);
    paramToFile(f);
    
}

void PictureManager::refresh(int f){
    contourFinder(f);
    objectSort(f);
    isObjectF(f);
    largest(f);
    clear(f);
}

void PictureManager::clear(int f){
    if (f==GOAL) goal.clear();
    else pallid.clear();
}

void PictureManager::where(int f){
    Object largestObject(0,0,0);
    int dev;
    if (f==BALL) {
        largestObject=largestB;
        dev=DEV;
    }
    else {
        dev=(largestG.rect.width)/4;
        dev=(largestG.rect.width)/4;
        largestObject=largestG;
    }
    if((widthImg-dev)<(largestObject.x)){
        if ((largestObject.x)<(widthImg+dev)) {
            if (((f==BALL)&&(largestObject.y > 450))||((f==GOAL)&&(largestObject.y+largestObject.rect.height > maxGoalDist))) {
                // stop, ball/gate found
                dir=STOP;
            }
            else {
                dir=FORWARD;
            }
            
        }
        else{
            //turn left
            dir=LEFT;
        }
    }
    else{
        //turn right
        dir=RIGHT;
    }
}

void PictureManager::paramFromFile(int f){
    std::string goal;
    if (f==YELLOW) {
        goal="YELLOW";
    }
    else{
        goal="BLUE";
    }
    std::string object="";
    int hL, sL, vL, hU, sU, vU;
    std::string line = " ";
    std::ifstream in;
    std::ofstream out;
    
    in.open("values.txt");
    if (!in.is_open()){
        out.open("values.txt", std::fstream::trunc);
        out <<" "<<std::endl;
        out << "BALL"<<" "<<0<<" "<<0<<" "<<0<<" "<<255<<" "<<255<<" "<<255<< std::endl;
        out << "YELLOW"<<" "<<0<<" "<<0<<" "<<0<<" "<<255<<" "<<255<<" "<<255<< std::endl;
        out << "BLUE"<<" "<<0<<" "<<0<<" "<<0<<" "<<255<<" "<<255<<" "<<255<<  std::endl;
        out <<" "<<std::endl;
        out.close();
    }
    else
    {
        while (getline(in,line))
        {
            in >> object >> hL >> sL >> vL >>  hU >>  sU >>  vU;
            
            if (object=="BALL"){
                lowH_B=hL;
                lowS_B=sL;
                lowV_B=vL;
                upH_B=hU;
                upS_B=sU;
                upV_B=vU;
            }
            else if (object==goal) {
                lowH_G=hL;
                lowS_G=sL;
                lowV_G=vL;
                upH_G=hU;
                upS_G=sU;
                upV_G=vU;
            }
        }
        in.close();
    }
}

void PictureManager::paramToFile(int f){
    std::string otherGoal;
    std::string goal;
    std::ifstream in;
    std::ofstream out;
    if (f==YELLOW) {
        goal="YELLOW";
        otherGoal="BLUE";
    }
    else{
        goal="BLUE";
        otherGoal="YELLOW";
    }
    std::string object;
    int hL, sL, vL, hU, sU, vU;
    std::string line=" ";
    in.open("values.txt");
    while (getline(in,line))
    {
        in >> object >> hL >> sL >> vL >>  hU >>  sU >>  vU;
        if (object==otherGoal){
            in.close();
            out.open("values.txt");
            out <<" "<<std::endl;
            out << "BALL"<<" "<<lowH_B<<" "<<lowS_B<<" "<<lowV_B<<" "<<upH_B<<" "<<upS_B<<" "<<upV_B << std::endl;
            out << goal <<" "<<lowH_G<<" "<<lowS_G<<" "<<lowV_G<<" "<<upH_G<<" "<<upS_G<<" "<<upV_G<<  std::endl;
            out << otherGoal<<" "<<hL<<" "<<sL<<" "<<vL<<" "<<hU<<" "<<sU<<" "<<vU<<  std::endl;
            out <<" "<<std::endl;
            out.close();
            break;
        }
    }
}

void PictureManager::parameetrid(int f, RobotManager *manager) {
    int * lowH;
    int * lowS;
    int * lowV;
    int * upH;
    int * upS;
    int * upV;
    
    std::string vName;
    if (f==GOAL) {
        lowH=&lowH_G;
        lowS=&lowS_G;
        lowV=&lowV_G;
        upH=&upH_G;
        upS=&upS_G;
        upV=&upV_G;
        vName = "goal";
    }
    else{
        lowH=&lowH_B;
        lowS=&lowS_B;
        lowV=&lowV_B;
        upH=&upH_B;
        upS=&upS_B;
        upV=&upV_B;
        vName = "ball";
    }
    cv::namedWindow("video", 1);
    cv::namedWindow(vName, 1);
    cv::Mat binary;
    
    cv::createTrackbar("LowH", "video", lowH, 255);
    cv::createTrackbar("UpH", "video", upH, 255);
    cv::createTrackbar("LowS", "video", lowS, 255);
    cv::createTrackbar("UpS", "video", upS, 255);
    cv::createTrackbar("LowV", "video", lowV, 255);
    cv::createTrackbar("UpV", "video", upV, 255);
    
    int pressed = 0;
    while (true) {
        cap >> frame;
        imshow(vName, frame);
        cv::GaussianBlur(frame, frame, cv::Size(KSIZE,KSIZE), KDEV);
        cv::inRange(frame, cv::Scalar(*lowH, *lowS, *lowV), cv::Scalar(*upH, *upS, *upV), binary);
        cv::dilate(binary,binary,elemDilate);
        cv::erode(binary, binary,elemErode);
        imshow("video", binary);
        
        int key = cv::waitKey(1); 
        if(key == 27){
            cv::destroyWindow(vName);
            cv::destroyWindow("video");
            break;
        }
        else if (key == 100) {
			if (pressed == 0) {
				manager->moveRobot(0, 0, -15);
			}
			pressed += 1;
		}
		else if (key == 119) {
			if (pressed == 0) manager->moveRobot(0, 30, 0);
			pressed += 1;
		}
		else if (key == 97) {
			if (pressed == 0) manager->moveRobot(0, 0, 15);
			pressed += 1;
		}
		else if (key == 99) {
			manager->shootCoil();
		}
		else {
			if (pressed > 10) {
				manager->moveRobot(0, 0, 0);
				pressed = 0;
			} else {
				pressed +=1;
			}
			
		}
    }
}

void PictureManager::contourFinder(int f) {
    int * lowH;
    int * lowS;
    int * lowV;
    int * upH;
    int * upS;
    int * upV;
    std::vector<std::vector <cv::Point> > * contours;
    if (f==GOAL) {
        lowH=&lowH_G;
        lowS=&lowS_G;
        lowV=&lowV_G;
        upH=&upH_G;
        upS=&upS_G;
        upV=&upV_G;
        contours=&contours_G;
    }
    else{
        lowH=&lowH_B;
        lowS=&lowS_B;
        lowV=&lowV_B;
        upH=&upH_B;
        upS=&upS_B;
        upV=&upV_B;
        contours=&contours_B;
    }
    cv::Vector<cv::Vec4i> hierarchy;
    cap >> frame;
    cv::GaussianBlur(frame, frame, cv::Size(KSIZE,KSIZE), KDEV);
    cv::inRange(frame, cv::Scalar(*lowH,*lowS,*lowV), cv::Scalar(*upH,*upS,*upV), frame);
    cv::dilate(frame,frame,elemDilate);
    cv::erode(frame, frame,elemErode);
    cv::findContours(frame, (*contours), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
}

void PictureManager::objectSort(int f){
    std::vector<Object> * objects;
    std::vector<std::vector <cv::Point> > * contours;
    if (f==GOAL) {
        contours=&contours_G;
        objects=&goal;
    }
    else{
        contours=&contours_B;
        objects=&pallid;
    }
    std::vector<cv::Moments> mu((*contours).size());
    int suurus,x,y;
    if (f==GOAL) {
        if (((*contours).size())>0) {
            int loc = 0;
            int oldSize = 0;
            x=-1;
            y=-1;
            for (int i=0; i<(*contours).size(); i++) {
                mu[i]=cv::moments((*contours)[i],false);
                suurus=mu[i].m00;
                if (suurus>oldSize) {
                    loc=i;
                    oldSize=suurus;
                    x=mu[i].m10/mu[i].m00;
                    y=mu[i].m01/mu[i].m00;
                }
            }
            std::vector<cv::Point> poly;
            cv::Rect exPoints;
            cv::approxPolyDP(cv::Mat(contours_G[loc]), poly, 3, true);
            exPoints = cv::boundingRect(poly);
            (*objects).push_back(Object(oldSize, x, y, exPoints));
        }
    }
    else{
        for (int i=0; i<(*contours).size(); i++) {
            mu[i]=cv::moments((*contours)[i],false);
            suurus=mu[i].m00;
            if(suurus>5){
                x=mu[i].m10/mu[i].m00;
                y=mu[i].m01/mu[i].m00;
                (*objects).push_back(Object(suurus, x, y));
            }
        }
    }
}

void PictureManager::isObjectF(int f){
    bool * isObject;
    std::vector<Object> * objects;
    if (f==GOAL) {
        objects=&goal;
        isObject=&isGoal;
    }
    else{
        objects=&pallid;
        isObject=&isPall;
    }
    if((*objects).size()>0) *isObject=true;
    else *isObject=false;
}

void PictureManager::largest(int f){
    Object * largestObject;
    std::vector<Object> * objects;
    if (f==GOAL) {
        objects=&goal;
        largestObject=&largestG;
    }
    else{
        objects=&pallid;
        largestObject=&largestB;
    }
    Object tyhi = Object(0,0,0);
    for (int i=0; i<(*objects).size(); i++) {
        if ((*objects)[i].suurus>tyhi.suurus) {
            tyhi=(*objects)[i];
        }
    }
    (*largestObject)=tyhi;
}









