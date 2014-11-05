#ifndef ROBOTMANAGER_HPP_INCLUDED
#define ROBOTMANAGER_HPP_INCLUDED

#define PI 3.14159265
#define RAD_PER_DEG 2*PI/360

class RobotManager {
	
	
	
	private:
	int engines[3];
	int coilPort;
	
	public:
	
	void initSerial();
	void moveRobot(float angle, float speed, int rotSpeed);
	void turnRobot(int spd);
	void shootCoil();
	
	~RobotManager();
};


#endif // ROBOTMANAGER_HPP_INCLUDED
