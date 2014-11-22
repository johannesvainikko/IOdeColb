#ifndef ROBOTMANAGER_HPP_INCLUDED
#define ROBOTMANAGER_HPP_INCLUDED

#define PI 3.14159265
#define RAD_PER_DEG 2*PI/360
#define MAXITER 5

class RobotManager {
	
	
	
	private:
	int engines[3];
	int engSpeeds[3];
	int prevEngSpeeds[3];
	int iter;
	bool sCheck;
	int coilPort;
	

	
	public:
	bool hasSerial;
	void initSerial();
	void moveRobot(float angle, float speed, int rotSpeed);
	void turnRobot(int spd);
	void shootCoil();
	
	// These poll for switch state, wait and return state
	bool getSwitch(int nr);
	
	// Switch state reading broken up to separete write and read for speed 
	void sendSwitch(int nr);
	bool readSwitch(int nr);
	
	void runDribler();
	void stopDribler();
	
	void checkSpeeds();
	
	~RobotManager();
};


#endif // ROBOTMANAGER_HPP_INCLUDED
