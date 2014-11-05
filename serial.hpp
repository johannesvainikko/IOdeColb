
#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED
#include <string>

int getIDn(int port);
int sendAsString(std::string task, int port);
int* scanPorts();
void closePorts(int ports[]);
void setSpeedForEng(int eng, int speed);

void pCoil(int port);
void cCoil(int port);
void sCoil(int port);

#endif // SERIAL_H_INCLUDED
