#include "PictureManager.hpp"
#include <string>

void PictureManager::init(int f, RobotManager *manager){
    cap.open(0); //avab suhtluse kaameraga
    cap >> frame; //võtab esimese kaadri
    
    cv::Size su = frame.size(); //kaadri suurus
    widthImg = (su.width)/2; //kaadri laius /2
    heightImg = su.height;
    elemDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(EDSIZE,EDSIZE)); //millega hiljem erode ja dilatet teha
    elemErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(EDSIZE+6,EDSIZE+6));
    paramFromFile(f); //failist lugemine
    parameetrid(FIELD, manager);
    parameetrid(BALL, manager);
    parameetrid(GOAL, manager);
    paramToFile(f); //faili salvestamine
    
}

void PictureManager::refresh(int f){
    cap>>frame;
    cv::cvtColor(frame,frame,CV_BGR2HSV);
    if (f==BALL) fieldmask();
    contourFinder(f); //objekti kontuuride leidmine
    objectSort(f); //kontuuridest objektide leidmine
    isObjectF(f); //kas objektid leiti?
    largest(f); //suurim leitud objekt
    clear(f); //objektide info puhastamine
    
}

void PictureManager::clear(int f){ //puhastab leitud pallide/värava info
    if (f==GOAL) goal.clear();
    else pallid.clear();
}

void PictureManager::where(int f){ //palli või värava asukoht kaadri keskkoha suhtes
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
			//
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

void PictureManager::paramFromFile(int f){ //failist lugemine
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
        out << "FIELD"<<" "<<0<<" "<<0<<" "<<0<<" "<<255<<" "<<255<<" "<<255<<  std::endl;
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
            else if(object=="FIELD"){
                lowH_F=hL;
                lowS_F=sL;
                lowV_F=vL;
                upH_F=hU;
                upS_F=sU;
                upV_F=vU;
            }
        }
        in.close();
    }
}

void PictureManager::paramToFile(int f){ //faili kirjutamine
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
            out << "FIELD"<<" "<<lowH_F<<" "<<lowS_F<<" "<<lowV_F<<" "<<upH_F<<" "<<upS_F<<" "<<upV_F << std::endl;
            out <<" "<<std::endl;
            out.close();
            break;
        }
    }
}

void PictureManager::parameetrid(int f, RobotManager *manager) { //kalibreerimine
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
    else if(f==FIELD){
        lowH=&lowH_F;
        lowS=&lowS_F;
        lowV=&lowV_F;
        upH=&upH_F;
        upS=&upS_F;
        upV=&upV_F;
        vName = "field";

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
    cv::Mat binary2;
    
    cv::createTrackbar("LowH", "video", lowH, 255);
    cv::createTrackbar("UpH", "video", upH, 255);
    cv::createTrackbar("LowS", "video", lowS, 255);
    cv::createTrackbar("UpS", "video", upS, 255);
    cv::createTrackbar("LowV", "video", lowV, 255);
    cv::createTrackbar("UpV", "video", upV, 255);
    
    int pressed = 0;
    
    
    //cap >> frame;
    //video.open("video2.avi",CV_FOURCC('x','v','i','d'),20,cv::Size(frame.size()));
    while (true) {
        cap>>frame;
        cv::cvtColor(frame,frame,CV_BGR2HSV);
        if(!(f==GOAL)) fieldmask();
        imshow(vName, frame);
        cv::GaussianBlur(frame, frame, cv::Size(KSIZE,KSIZE), KDEV);
        cv::cvtColor(frame,frame,CV_BGR2HSV);
        cv::inRange(frame, cv::Scalar(*lowH, *lowS, *lowV), cv::Scalar(*upH, *upS, *upV), binary);
        bitwise_not(binary,binary2);
        cv::cvtColor(binary2, binary2, CV_GRAY2BGR);
        cv::subtract(frame, binary2, binary2);
        imshow("video", binary2);
        
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
		}
		else if (key == 119) {
			if (pressed == 0) manager->moveRobot(0, 30, 0);
		}
		else if (key == 97) {
			if (pressed == 0) manager->moveRobot(0, 0, 15);
		}
		else if (key == 115) {
			if (pressed == 0) manager->moveRobot(0, -30, 0);
		}
		else if (key == 117) {
			if (pressed == 0) manager->moveRobot(0, 100, 0);
		}
		else if (key == 99) {
			manager->shootCoil();
		}
		else if (key == -1){
			if (pressed == 0) {
				manager->moveRobot(0, 0, 0);
				pressed += 1;
			}
			
		}
		if (pressed > 3) pressed = 0;
		else pressed += 1;
    }
}

void PictureManager::contourFinder(int f) { //kontuuride leidmine vastavalt objektist
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
    cv::inRange(frame, cv::Scalar(*lowH,*lowS,*lowV), cv::Scalar(*upH,*upS,*upV), frame); //värvivahemike järgi väljaarvamine
    cv::findContours(frame, (*contours), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE); //kontuurileidmine (ainult välised)
}

void PictureManager::objectSort(int f){ //värava leidmine eeldusel et suurima alaga kontuur ja pallide puhul väikseid palliks ei loeta
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
    std::vector<cv::Moments> mu((*contours).size()); //kontuuride momendid
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
		refresh(GOAL);
        for (int i=0; i<(*contours).size(); i++) {
            mu[i]=cv::moments((*contours)[i],false);
            suurus=mu[i].m00;
            if(suurus>5){ //loeb pallide vektorisse pallid mis on suuremad kui
				x=mu[i].m10/mu[i].m00;
				y=mu[i].m01/mu[i].m00;
				if(!(largestG.rect.contains(cv::Point(x,y)))){ //ja ei ole kordinaatidega värava alas
					(*objects).push_back(Object(suurus, x, y));
				}
            }
        }
    }
}

void PictureManager::isObjectF(int f){ //kas pallid või värav on kaadris
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

void PictureManager::largest(int f){ //suurim objekt
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

void PictureManager::fieldmask(){
    cv::Mat binary;
    //cv::Mat binary2;
    //cv::Mat binary3 = cv::Mat::ones(frame.size(), binary.type())*255;
    cv::GaussianBlur(frame, frame, cv::Size(KSIZE,KSIZE), KDEV);
    cv::inRange(frame, cv::Scalar(lowH_F, lowS_F, lowV_F), cv::Scalar(upH_F, upS_F, upV_F), binary);
    cv::dilate(binary,binary,elemDilate);
    cv::erode(binary, binary,elemErode);
    cv::line(binary, cv::Point(0,binary.size().height), cv::Point(binary.size().width,binary.size().height), cv::Scalar(255,255,255),3);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    int suurus;
    int oldSize = 0;
    int loc = 0;
    std::vector<cv::Moments> mu(contours.size());
    for (int i=0; i<(contours.size()); i++) {
        mu[i]=cv::moments(contours[i],false);
        suurus=mu[i].m00;
        if (suurus>oldSize) {
            loc=i;
            oldSize=suurus;
        }
    }
    binary=cv::Mat::zeros(frame.size(), binary.type());
    cv::drawContours(binary, contours, loc, cv::Scalar(255,255,255),-1);

    bitwise_not ( binary, binary );
    
    cv::cvtColor(binary, binary, CV_GRAY2BGR);
    cv::subtract(frame, binary, frame);
}








