#include "object.hpp"
#include "PictureManager.hpp"
#include "RobotManager.hpp"
#include <unistd.h>

int main(int argC, char *argV[]){
	
    bool skip=false;
    int target = BALL;
	RobotManager manager;
	if (!skip) manager.initSerial();
    PictureManager camera;
    
    if (argC>1){
        if (strcmp(argV[1], "-computer")==0) {
            skip=true;
        }
        if (strcmp(argV[1], "-BLUE")==0) {
            camera.init(BLUE);
        }
    } else {
		camera.init(YELLOW);
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
						if (!skip) manager.moveRobot(0, 20, 0);
						else std::cout << "forward" << std::endl;
						break;
					case LEFT:
						timeout = 0;
						if (!skip) manager.moveRobot(0, 0, -10);
						else std::cout << "left" << std::endl;
						break;
					case RIGHT:
						timeout = 0;
						if (!skip) manager.moveRobot(0, 0, 10);
						else std::cout << "right" << std::endl;
						break;
					case STOP:
					timeout = 0;
						if (!skip) {
							manager.moveRobot(0, 20, 0);
							usleep(500000);
						}
						else std::cout << "no movement, ball in dribbler, start searching for goal" << target << std::endl;
						if (target==BALL) target=GOAL;
						else target=BALL;
						break; //switch to goal search
                    }
				}
            else {		//goal search
				switch (camera.dir) {
					case FORWARD:
						if (!skip) {
							manager.moveRobot(0, 0, 0);
							manager.shootCoil();
						}
						else std::cout << "shoot, goal forward" << std::endl;
						search = false;
						break;
					case LEFT:
						timeout = 0;
						if (!skip) manager.moveRobot(90, 10, -11);
						else std::cout << "left with ball" << std::endl;
						break;
					case RIGHT:
						timeout = 0;
						if (!skip) manager.moveRobot(270, 10, 11);
						else std::cout << "right with ball" << std::endl;
						break;
					case STOP:
						timeout = 0;
						if (!skip) {
							manager.moveRobot(0, 0, 0);
							manager.shootCoil();
						}
						else std::cout << "goal close enough, shoot" << target << std::endl;
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
				if (!skip) manager.moveRobot(0, 0, -10);
				else std::cout << "left/search" << std::endl;
			} else {
				runs = 0;
				search = 0;
				timeout = 0;
			}
            
        }
    }
    runs = runs-1;
	}
    
    
    
    return 0;
}
