#include "object.hpp"
#include "PictureManager.hpp"
#include "RobotManager.hpp"
#include <unistd.h>

int main(int argC, char *argV[]){
	
	
	
	
	
	

    int target = BALL;
	RobotManager *tmpManager = new RobotManager();
	//tmpManager->hasSerial = false;
	
	
    PictureManager camera;
    
    if (argC>1){
        if (strcmp(argV[1], "-cmp")==0) {
            camera.init(YELLOW, tmpManager);
        }
        if (strcmp(argV[1], "-BLUE")==0) {
			tmpManager->initSerial();
            camera.init(BLUE, tmpManager);
        }
    } else {
		tmpManager->initSerial();
		
		if (tmpManager->getSwitch(3)){
			 std::cout << "init BLUE"<< std::endl;
			 camera.init(BLUE, tmpManager);
		 } else {
			 std::cout << "init YELLOW" << std::endl;
			 camera.init(YELLOW, tmpManager);
		 }
	}
	
	
	RobotManager *manager = tmpManager;
	
	
	
	
	camera.maxGoalDist = 10;
	
    
    
    int timeout = 0;
    int ballTimeout = 0;
    bool movingCloserToGoal = false;
    
    
	int checkSwitch = 0;
    
    bool wait = false;
	while (!wait) {
		 wait = tmpManager->getSwitch(2);
		 usleep(1000000);
	}
	
	bool run = true;
    
    while (run){
		bool search = true;
		while(search){
			camera.refresh(target);
			camera.where(target);
			if (target == BALL) {
				if (camera.isPall) {
					switch (camera.dir) { //ball detected
						std::cout << "b ";
						case FORWARD:
							//std::cout << "bforward" << std::endl;
							manager->moveRobot(0, 40, 0);
							break;
						case LEFT:
							//std::cout << "bleft" << std::endl;
							timeout = 0;
							manager->moveRobot(0, 0, -15);
							break;
						case RIGHT:
							//std::cout << "bright" << std::endl;
							timeout = 0;
							manager->moveRobot(0, 0, 15);
							break;
						case STOP:
							std::cout << "bstop" << std::endl;
						timeout = 0;
							manager->moveRobot(0, 20, 0);
							usleep(500000);
							if (target==BALL) target=GOAL;
							else target=BALL;
							break; //switch to goal search
						}
				} else { // no ball detected
					std::cout << "bnf ";
					if (ballTimeout < 50){
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
					manager->moveRobot(0, 0, 10);
					//std::cout << camera.isGoal << camera.isBall <<std::endl;
				}
			} 
			
		//check switch
		if (checkSwitch < 50) {checkSwitch += 1;}
		else {
			checkSwitch = 0;
			run = tmpManager->getSwitch(2);
		}
		//std::cout << "bto " <<ballTimeout << std::endl;
		std::cout<< " " << camera.isGoal << camera.isPall << std::endl;
		}
		//std::cout << "runs "<<runs << std::endl;
		//runs = runs-1;
		
	}
    
    
    
    return 0;
}
