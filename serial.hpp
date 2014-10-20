
#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED
#include <string>

int getIDn(int port);
int sendAsString(std::string task, int port);
int* scanPorts();
void closePorts(int ports[]);
void setSpeedForEng(int eng, int speed);

#endif // SERIAL_H_INCLUDED
