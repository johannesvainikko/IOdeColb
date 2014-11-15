#include "object.hpp"
#include "PictureManager.hpp"
#include "RobotManager.hpp"
#include <unistd.h>

int main(int argC, char *argV[]){

    int target = BALL;
	RobotManager *tmpManager = new RobotManager();
	tmpManager->hasSerial = false;
	
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
		camera.init(YELLOW, tmpManager);
		//manager.initSerial();
	}
	
	
	RobotManager *manager = tmpManager;
	
	//bool wait = false;
	//while (!wait) {
		// wait = manager.readSwitch1();
		 usleep(3000000);
	//}
	
	
	
	camera.maxGoalDist = 40;
	
    
    //manager.initSerial();
    
    int timeout = 0;
    int ballTimeout = 0;
    bool movingCloserToGoal = false;
    
    
    int runs = 10;
    
    while (runs > 0){
		bool search = true;
		while(search){
			camera.refresh(target);
			if (camera.isPall) {
				camera.where(target);
				if (target == BALL) {
					switch (camera.dir) {
						case FORWARD:
							std::cout << "bforward" << std::endl;
							manager->moveRobot(0, 20, 0);
							break;
						case LEFT:
							std::cout << "bleft" << std::endl;
							timeout = 0;
							manager->moveRobot(0, 0, -10);
							break;
						case RIGHT:
							std::cout << "bright" << std::endl;
							timeout = 0;
							manager->moveRobot(0, 0, 10);
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
					}
				else {		//goal search
					switch (camera.dir) {
						case FORWARD:
							std::cout << "gforward" << std::endl;
							manager->moveRobot(0, 20, 0);
							//manager.shootCoil();
							//search = false;
							break;
						case LEFT:
							std::cout << "gleft" << std::endl;
							timeout = 0;
							manager->moveRobot(90, 10, -11);
							break;
						case RIGHT:
							std::cout << "gright" << std::endl;
							timeout = 0;
							manager->moveRobot(270, 10, 11);
							break;
						case STOP:
							std::cout << "gstop" << std::endl;
							timeout = 0;
							manager->moveRobot(0, 0, 0);
							if (!movingCloserToGoal) {
								manager->shootCoil();
							} else std::cout << "noshoot" << std::endl;
							movingCloserToGoal = false;
							if (target==BALL) target=GOAL;
							else target=BALL;
							search = false;
							break;
						}
				}
			}
			else{
				if (target == BALL) {
					if (ballTimeout < 300){
						ballTimeout += 1;
					} else {
						target = GOAL;
						movingCloserToGoal = true;
						camera.maxGoalDist -= 50;
					}
				}
				if (timeout < 5000) {
					timeout = timeout+1;
					manager->moveRobot(0, 0, -10);
				} else {
					runs = 0;
					search = 0;
					timeout = 0;
				}
            
			}
			//std::cout << timeout << std::endl;
		}
		//std::cout << "runs "<<runs << std::endl;
		runs = runs-1;
	}
    
    
    
    return 0;
}
