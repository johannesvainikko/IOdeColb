#include "RobotManager.hpp"
#include "serial.hpp"
#include <iostream>
#include <unistd.h>
#include <cmath>


void RobotManager::initSerial() {
	int *ports;
	ports = scanPorts();
	for(int i = 0; i < 3; i++){
        engines[i] = ports[i];
    } 
	
	/*for (int i = 0; i < 90; i+=30) {
		moveRobot(i, 50, 0);
		usleep(500000);
	}*/

	
	moveRobot(0, 20, 0);
	usleep(500000);
	moveRobot(180, 20, 0);
	usleep(500000);
	moveRobot(0, 0, 20);
	usleep(500000);
	moveRobot(0, 0, -20);
	usleep(500000);
	moveRobot(0, 0, 0);
	usleep(2000000);
	//closePorts(ports);
}


RobotManager::~RobotManager()
{
    closePorts(engines);
}

void RobotManager::moveRobot(float angle, float speed, int rotSpd) {
	// This formula for wheel speeds is copied from team Firestarter
	angle = angle * RAD_PER_DEG;
	int speed2 = speed * cos(5.0 * PI / 6.0 - angle) + rotSpd;    
    int speed0 = speed * cos(PI / 6.0 - angle) + rotSpd;
    int speed1 = speed * cos(3.0 * PI / 2.0 - angle) + rotSpd;
    
    
    int speeds[3] = {speed0, speed1, speed2};
    
    for(int i = 0; i < 3; i++){
        setSpeedForEng(engines[i], speeds[i]);
    } 
    
}
