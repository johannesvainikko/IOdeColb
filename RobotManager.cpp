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
    coilPort = engines[3];
	
	/*for (int i = 0; i < 90; i+=30) {
		moveRobot(i, 50, 0);
		usleep(500000);
	}*/

	//moveRobot(90, 10, -11);
	//usleep(3000000);
	//moveRobot(270, 10, 11);
	//usleep(3000000);
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
	static int ports[4];
	ports[0] = engines[0];
	ports[1] = engines[1];
	ports[2] = engines[2];
	ports[3] = coilPort;
    closePorts(ports);
}

void RobotManager::shootCoil() {
	cCoil(coilPort);
	usleep(200000);
	sCoil(coilPort);
}

void RobotManager::moveRobot(float angle, float speed, int rotSpd) {
	// This formula for wheel speeds is copied from team Firestarter
	angle = angle * RAD_PER_DEG;
	int speed2 = speed * cos(5.0 * PI / 6.0 - angle) + rotSpd;    
    int speed1 = speed * cos(PI / 6.0 - angle) + rotSpd;
    int speed0 = speed * cos(3.0 * PI / 2.0 - angle) + rotSpd;
    
    
    int speeds[3] = {speed0, speed1, speed2};
    
    for(int i = 0; i < 3; i++){
        setSpeedForEng(engines[i], speeds[i]);
    } 
    
}
