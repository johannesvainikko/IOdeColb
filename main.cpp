#include "object.hpp"
#include "PictureManager.hpp"
#include "RobotManager.hpp"
#include <unistd.h>

int main(int argC, char *argV[]){

    int target = BALL;
	RobotManager manager;
	
    PictureManager camera;
    
    if (argC>1){
        if (strcmp(argV[1], "-cmp")==0) {
            camera.init(YELLOW, manager);
        }
        if (strcmp(argV[1], "-BLUE")==0) {
			manager.initSerial();
            camera.init(BLUE, manager);
        }
    } else {
		manager.initSerial();
		camera.init(YELLOW, manager);
	}
    
    
    
    int timeout = 0;
    
    int runs = 3;
    
    while (runs > 0){
		bool search = true;
		while(search){
			camera.refresh(target);
			if (camera.isPall) {
				camera.where(target);
				if (target == BALL) {
					switch (camera.dir) {
						case FORWARD:
							manager.moveRobot(0, 20, 0);
							break;
						case LEFT:
							timeout = 0;
							manager.moveRobot(0, 0, -10);
							break;
						case RIGHT:
							timeout = 0;
							manager.moveRobot(0, 0, 10);
							break;
						case STOP:
						timeout = 0;
							manager.moveRobot(0, 20, 0);
							usleep(500000);
							if (target==BALL) target=GOAL;
							else target=BALL;
							break; //switch to goal search
						}
					}
				else {		//goal search
					switch (camera.dir) {
						case FORWARD:
							manager.moveRobot(0, 0, 0);
							manager.shootCoil();
							search = false;
							break;
						case LEFT:
							timeout = 0;
							manager.moveRobot(90, 10, -11);
							break;
						case RIGHT:
							timeout = 0;
							manager.moveRobot(270, 10, 11);
							break;
						case STOP:
							timeout = 0;
							manager.moveRobot(0, 0, 0);
							manager.shootCoil();
							if (target==BALL) target=GOAL;
							else target=BALL;
							search = false;
							break;
						}
				}
			}
			else{
				if (timeout < 500) {
					timeout = timeout+1;
					manager.moveRobot(0, 0, -10);
				} else {
					runs = 0;
					search = 0;
					timeout = 0;
				}
            
			}
			std::cout << timeout << std::endl;
		}
		//std::cout << "runs "<<runs << std::endl;
		runs = runs-1;
	}
    
    
    
    return 0;
}
