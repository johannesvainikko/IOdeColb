#include "object.hpp"
#include "PictureManager.hpp"
#include "RobotManager.hpp"
#include <unistd.h>

int main(int argC, char *argV[]){
	
	
	
	
	
	

    int target = BALL;
	RobotManager *tmpManager = new RobotManager();
	//tmpManager->hasSerial = false;
	int goalColor = YELLOW;
	
    PictureManager camera;
    
    if (argC>1){
        if (strcmp(argV[1], "-BLUE")==0) {
			goalColor = BLUE;
        }
    } else {
		tmpManager->initSerial();
	}
	camera.init(tmpManager);
	
	RobotManager *manager = tmpManager;
	
	
	
	
	camera.maxGoalDist = 0;
	
    
    
    int timeout = 0;
    int ballTimeout = 0;
    bool movingCloserToGoal = false;
    
    
    
	int checkSwitch = 30;
    
    
    
    std::cout << "waiting for run switch" << std::endl;
    bool wait = false;
	while (!wait) {
		std::cout << ".";
		 wait = tmpManager->getSwitch(2);
		 usleep(1000000);
	}
	
	
	bool run = true;
    
    while (run){
		bool search = true;
		while(search){
			camera.capFrame();
			if (target == BALL) {
				camera.refresh(BALL);
				camera.where(target);
				if (camera.isPall) {
					switch (camera.dir) { //ball detected
						std::cout << "b ";
						case FORWARD:
							//std::cout << "bforward" << std::endl;
							if (camera.largestB.y > (camera.heightImg/3)) {
								manager->moveRobot(0, 50, 0);
							} else {
								manager->moveRobot(0, 100, 0);
							}
							break;
						case LEFT:
							//std::cout << "bleft" << std::endl;
							timeout = 0;
							if ((camera.widthImg - camera.largestB.x) < 20) { // correct course when moving forward
								if (camera.largestB.y < (camera.heightImg/3)) {
								manager->moveRobot(0, 100, -10);
								} else {
									manager->moveRobot(0, 0, -10);
								}
							} else if ((camera.widthImg - camera.largestB.x) < 50) {
								manager->moveRobot(0, 0, -10);
							} else {
								manager->moveRobot(0, 0, -20);
							}
							break;
						case RIGHT:
							//std::cout << "bright" << std::endl;
							timeout = 0;
							if ((camera.largestB.x - camera.widthImg) < 20) {
								if (camera.largestB.y < (camera.heightImg/3)) {
								manager->moveRobot(0, 100, 10);
								} else {
									manager->moveRobot(0, 0, 10);
								}
							} else if ((camera.largestB.x - camera.widthImg) < 50) {
								manager->moveRobot(0, 0, 10);
							} else {
								manager->moveRobot(0, 0, 20);
							}
							break;
						case STOP:
							std::cout << "bstop" << std::endl;
							int moveToBall = 0;
							manager->moveRobot(0, 20, 0);
							while (moveToBall < 20) {
								moveToBall += 1;
								if (!tmpManager->getSwitch(1)) {// check if ball in dribbler
									std::cout << "ball in dribbler" << std::endl;
									if (target==BALL) target=GOAL;
									else target=BALL;
									break; //switch to goal search
								}
								usleep(100000);
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
						movingCloserToGoal = true;
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
				std::cout << "waiting dribler lift" << std::endl;
				int times = 0;
				while (times < 10) { // 10 seconds to switch the goal or code exits
					std::cout << ".";
					
					if (!tmpManager->getSwitch(1)){ // Troggered by lifting dribbler before timeout
						times = 11;
						if (tmpManager->hasSerial) {
							if (!tmpManager->getSwitch(3)){
								std::cout << "set BLUE"<< std::endl;
								goalColor = BLUE;
								manager->moveRobot(0, 0, 10);
							} else {
								std::cout << "set YELLOW" << std::endl;
								goalColor = YELLOW;
								manager->moveRobot(0, 0, -10);
							}
							usleep(400000);
							manager->moveRobot(0, 0, 0);
							
							std::cout << "waiting for run switch";
							wait = false;
							while (!wait) {
								std::cout << ".";
								wait = tmpManager->getSwitch(2);
								usleep(1000000);
							}
						}
					}
					times += 1;
					usleep(1000000);
				}
				if (times == 10) return 0;
			}
		}
		//std::cout << "bto " <<ballTimeout << std::endl;
		//std::cout<< " " << camera.isGoal << camera.isPall << std::endl;
		}
		//std::cout << "runs "<<runs << std::endl;
		//runs = runs-1;
		
	}
    
    
    
    return 0;
}
