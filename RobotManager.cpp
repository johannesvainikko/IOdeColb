#include "RobotManager.hpp"
#include "serial.hpp"
#include <iostream>
#include <unistd.h>
#include <cmath>


void RobotManager::initSerial() {
	std::cout << "initSerial()"<< std::endl;
	
	int *ports;
	ports = scanPorts();
	for(int i = 0; i < 4; i++){
        engines[i] = ports[i];
    }
    coilPort = engines[3];
    
    hasSerial = true;
    
    //sCoil(coilPort);
	
	
	
	
	/*for (int i = 0; i < 90; i+=30) {
		moveRobot(i, 50, 0);
		usleep(500000);
	}*/

	//moveRobot(90, 10, -11);
	//usleep(3000000);
	//moveRobot(270, 10, 11);
	//usleep(3000000);
	//moveRobot(0, 20, 0);
	//usleep(1000000);
	//moveRobot(180, 20, 0);
	//usleep(1000000);
	//moveRobot(0, 0, 20);
	//usleep(1000000);
	//moveRobot(0, 0, -20);
	//usleep(1000000);
	
	
	moveRobot(0, 0, 0);
	usleep(200000);
	//closePorts(ports);
	
	
	
}

RobotManager::~RobotManager()
{	std::cout << "~RobotManager()"<< std::endl;
	if (hasSerial) {
		static int ports[4];
		ports[0] = engines[0];
		ports[1] = engines[1];
		ports[2] = engines[2];
		ports[3] = coilPort;
		closePorts(ports);
	}
	
}

void RobotManager::shootCoil() {
	if (hasSerial) sCoil(coilPort);
}

void RobotManager::moveRobot(float angle, float speed, int rotSpd) {
	//if (iter < MAXITER) iter += 1;
	//else checkSpeeds();
	// This formula for wheel speeds is copied from team Firestarter
	angle = angle * RAD_PER_DEG;
	int speed2 = speed * cos(5.0 * PI / 6.0 - angle) + rotSpd;    
    int speed1 = speed * cos(PI / 6.0 - angle) + rotSpd;
    int speed0 = speed * cos(3.0 * PI / 2.0 - angle) + rotSpd;
    
    
    int speeds[3] = {speed0, speed1, speed2};
    
    //std::cout<<"spd -> " << speed0 << ":" << speed1<< ":" <<speed2 << std::endl;
    if (hasSerial) {
		for(int i = 0; i < 3; i++){
       
			setSpeedForEng(engines[i], speeds[i]);
			engSpeeds[i] = speeds[i];
		} 
    } else {
			std::cout<<"spd -> " << speed0 << ":" << speed1<< ":" <<speed2 << std::endl;
	}
}


void RobotManager::checkSpeeds() {
	if (!sCheck) {
		sendSpeedCheckForEng(engines[0]);
		//usleep(1000000);
		//readSpeedCheckForEng(engines[0]);
		sCheck = true;
	}
	else {
		int spd = readSpeedForEng(engines[0]);
		std::cout << "read spd:" << spd << std::endl;
		sCheck = false;
	}
}





bool RobotManager::readSwitch1() {
	if (hasSerial)  return readPin(engines[0]);
	else return true;
}
