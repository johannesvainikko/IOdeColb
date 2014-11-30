#include "object.hpp"
#include "PictureManager.hpp"
#include "RobotManager.hpp"
#include <unistd.h>

int main(int argC, char *argV[]){
	
	
	
	
	
	
    cv::namedWindow("frame", 1);
    int target = BALL;
	RobotManager *tmpManager = new RobotManager();
	//tmpManager->hasSerial = false;
	int goalColor;
	
    PictureManager camera;
    
    if (argC>1){
        if (strcmp(argV[1], "-BLUE")==0) {
			goalColor = BLUE;
        }
    } else {
		tmpManager->initSerial();
		goalColor = YELLOW;
	}
	camera.init(tmpManager);
	
	RobotManager *manager = tmpManager;
	
	
	
	
	camera.maxGoalDist = 0;
	
    
    
    int timeout = 0;
    int ballTimeout = 0;
    bool movingCloserToGoal = false;
    
    
    
	int checkSwitch = 30;
    
    
    
    //std::cout << "waiting for run switch" << std::endl;
    //bool wait = false;
	//while (!wait) {
	//	std::cout << ".";
	//	 wait = tmpManager->getSwitch(2);
	//	 usleep(1000000);
	//}
	
	
	bool run = true;
    
    while (run){
		bool search = true;
		while(search){
			camera.capFrame();
			if (target == BALL) {
				camera.refresh(BALL);
				camera.where(target);
				if (camera.isPall) {
					std::cout << "b " << camera.dir;
					switch (camera.dir) { //ball detected
						case FORWARD:
							std::cout << "f ";
							/*if (camera.largestB.y < (camera.heightImg/4)) {
								std::cout << "f ";
							} else if (camera.largestB.y < (camera.heightImg/2)) {
								std::cout << "lf ";
							} else if (camera.largestB.y < (camera.heightImg*0.75)) {
								std::cout << "c ";
							} else {
								std::cout << "wc ";
							}*/
							
							
							//std::cout << "bforward" << std::endl;
							if (camera.largestB.y < (camera.heightImg/2)) {
								manager->moveRobot(0, 100, 0);
								std::cout << "f ";
							} else if (camera.largestB.y < (camera.heightImg*0.75)) {
								manager->moveRobot(0, 40, 0);
								std::cout << "m ";
							} else {
								manager->moveRobot(0, 30, 0);
								std::cout << "c ";
							}
							break;
						case LEFT:
						std::cout << "l ";
							//std::cout << "bleft" << std::endl;
							timeout = 0;
							/*if ((camera.widthImg - camera.largestB.x) < 20) { // correct course when moving forward
								if (camera.largestB.y < (camera.heightImg/3)) {
									manager->moveRobot(0, 100, 10);
									//manager->setSpeeds(80, -10, 80);
									std::cout << "f ";
								} else {
									manager->moveRobot(0, 0, -10);
								}
							} else if ((camera.widthImg - camera.largestB.x) < 50) {
								manager->moveRobot(0, 0, -10);
							} else {
								manager->moveRobot(0, 0, -20);
							}
							*/
							if (camera.largestB.y < (camera.heightImg/2)) {
								int turn = (camera.largestB.x - camera.widthImg)/-30;
								manager->moveRobot(0, 50, turn);
								std::cout << "f " << turn << " ";
							} else if (camera.largestB.y < (camera.heightImg*0.75)) {
								int turn = (camera.largestB.x - camera.widthImg)/-20;
								manager->moveRobot(0, 50, turn);
								std::cout << "m " << turn << " ";
							} else {
								int turn = (camera.largestB.x - camera.widthImg)/-15;
								manager->moveRobot(0, 0, turn);
								std::cout << "c " << turn << " ";
							}
							
							break;
						case RIGHT:
							std::cout << "r ";
							//std::cout << "bright" << std::endl;
							timeout = 0;
							/*if ((camera.largestB.x - camera.widthImg) < 20) {
								if (camera.largestB.y < (camera.heightImg/3)) {
									manager->moveRobot(0, 100, -10);
									//manager->setSpeeds(80, 10, 80);
									std::cout << "f ";
								} else {
									manager->moveRobot(0, 0, 10);
								}
							} else if ((camera.largestB.x - camera.widthImg) < 50) {
								manager->moveRobot(0, 0, 10);
							} else {
								manager->moveRobot(0, 0, 20);
							}*/
							if (camera.largestB.y < (camera.heightImg/2)) {
								int turn = -(camera.largestB.x - camera.widthImg )/15;
								manager->moveRobot(0, 100, turn);
								std::cout << "f " << turn << " ";
							} else if (camera.largestB.y < (camera.heightImg*0.75)) {
								int turn = -(camera.largestB.x - camera.widthImg )/8;
								manager->moveRobot(0, 50, turn);
								std::cout << "m " << turn << " ";
							} else {
								int turn = -(camera.largestB.x - camera.widthImg)/15;
								manager->moveRobot(0, 0, turn);
								std::cout << "c " << turn << " ";
							}
							
							break;
						case STOP:
							std::cout << "bstop" << std::endl;
							int moveToBall = 0;
							manager->moveRobot(0, 20, 0);
							while (moveToBall < 20) {
								//bool meh =camera.isBallForward();
								//if (meh) std::cout << "ball in the way" << std::endl;
								//else std::cout << "ball not in the way" << std::endl;
								moveToBall += 1;
								if (!tmpManager->getSwitch(1)) {// check if ball in dribbler
									std::cout << "ball in dribbler" << std::endl;
									if (target==BALL) target=GOAL;
									else target=BALL;
									break; //switch to goal search
								}
								usleep(100000);
							}
							if (!manager->hasSerial) {
								std::cout << "computer captured ball" << std::endl;
								if (target==BALL) target=GOAL;
								else target=BALL;
								break; //switch to goal search
							}
							timeout = 0;
							
							//usleep(500000);
							
						}
				} else { // no ball detected
					std::cout << "bnf ";
					if (ballTimeout < 20000){
						ballTimeout += 1;
						//std::cout << ballTimeout << std::endl;
						manager->moveRobot(0, 0, -10);
					} else {
						target = GOAL;
						movingCloserToGoal = false;
						camera.maxGoalDist += 10;
						ballTimeout = 0;
						std::cout << "move closer ro goal" << std::endl;
					}
				}
				
			} else if (target == GOAL) {
				camera.refresh(goalColor);
				camera.where(target);
				if (camera.isGoal) {
					std::cout << "g ";
					switch (camera.dir) { //goal detected
						case FORWARD:
							//std::cout << "gforward" << std::endl;
							manager->moveRobot(0, 20, 0);
							//manager.shootCoil();
							//search = false;
							break;
						case LEFT:
							//std::cout << "gleft" << std::endl;
							timeout = 0;
							//manager->moveRobot(90, 10, -11);
							manager->moveRobot(0, 0, -13);
							break;
						case RIGHT:
							//std::cout << "gright" << std::endl;
							timeout = 0;
							//manager->moveRobot(270, 10, 11);
							manager->moveRobot(0, 0, 13);
							break;
						case STOP:
							std::cout << "gstop" << std::endl;
							timeout = 0;
							manager->moveRobot(0, 0, 0);
							if (!movingCloserToGoal) {
								manager->shootCoil();
								if (!tmpManager->getSwitch(1)) manager->shootCoilLong(); 
								if (!tmpManager->getSwitch(1)) manager->shootCoilLong();
							} else std::cout << "noshoot, just moving" << std::endl;
							movingCloserToGoal = false;
							if (target==BALL) target=GOAL;
							else target=BALL;
							search = false;
							break;
						}
				} else {
					std::cout << "gnf ";
					manager->moveRobot(0, 0, 20);
					//std::cout << camera.isGoal << camera.isBall <<std::endl;
				}
			} 
			
		//check switch
		if (checkSwitch < 30) {
			checkSwitch += 1;
			//std::cout<< "chk sw" << checkSwitch << std::endl;
			}
		else {
			checkSwitch = 0;
			//std::cout<< "end it " << tmpManager->getSwitch(2) << std::endl;
			//bool sw = tmpManager->getSwitch(2);
			//run = sw;
			//search = sw;
			if (!tmpManager->getSwitch(2)) {
				manager->moveRobot(0, 0, 0);
				std::cout<< std::endl << "code paused, waitng for input" << std::endl;
				bool wait = false;
				while (!wait) {
					std::cout << ".";
					usleep(100000);
					if (!tmpManager->getSwitch(1)){
						std::cout<< std::endl << "Dribler lifted, code will exit" << std::endl;
						manager->moveRobot(180, 30, 0);
						usleep(300000);
						manager->moveRobot(0, 0, 0);
						return 0;
					} 
					wait = tmpManager->getSwitch(2);
					
				}
				
				if (tmpManager->getSwitch(3)){
					std::cout << "set BLUE"<< std::endl;
					goalColor = BLUE;
					//manager->moveRobot(0, 0, 10);
				} else {
					std::cout << "set YELLOW" << std::endl;
					goalColor = YELLOW;
					//manager->moveRobot(0, 0, -10);
				}
			}
		}
		//std::cout << "bto " <<ballTimeout << std::endl;
		std::cout<< " " << camera.isGoal << camera.isPall << std::endl;
		}
		//std::cout << "runs "<<runs << std::endl;
		//runs = runs-1;
        imshow("frame", camera.frame);
    }
    
    return 0;
}
