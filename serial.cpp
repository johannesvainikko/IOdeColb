
#include <iostream>
#include <fstream>

#include <cassert>
#include <cstdint>
#include <sstream>
#include <fcntl.h>

#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#inlcude <windows.h>
#endif

#include <string.h>
#include "rs232.h"
#include "serial.hpp"

/// Based on code from team Robosome

using namespace std;


void mySleep(int sleepMs)
{
#ifdef LINUX
    usleep(sleepMs * 1000000);   // usleep takes sleep time in us
#endif
#ifdef WINDOWS
    Sleep(sleepMs);
#endif
}



int* scanPorts() {
	static int ports[4];
    for (int i=24;i<28;i++) {
        int k =-1;
        do {
            cout<<"port "<<i;
            k=getIDn(i);
            
                if (k >= 0) {

					//int t = -1;
					//t = 
					sendAsString( "fs1", i);
                
					usleep(100000);
					//RS232_CloseComport(i);

            }
            if (k<=3 && k>=0){
                ports[k]=i;
            }
            /*if (k == 255) {
				ports[3] = k;
			}*/

            

            usleep(100000);
        } while (k==-1);
        
    
    }
    RS232_OpenComport(ports[0], 115200);
    RS232_OpenComport(ports[1], 115200);
    RS232_OpenComport(ports[2], 115200);
    RS232_OpenComport(ports[3], 115200);
    
    return ports;
}


void closePorts(int ports[]) {
	sendAsString( "sd0", ports[0]);
	sendAsString( "sd0", ports[1]);
	sendAsString( "sd0", ports[2]);
	sleep(0.2);
	sendAsString( "fs1", ports[0]);
	sendAsString( "fs1", ports[1]);
	sendAsString( "fs1", ports[2]);
	sleep(0.2);
	RS232_CloseComport(ports[0]);
	RS232_CloseComport(ports[1]);
	RS232_CloseComport(ports[2]);
	RS232_CloseComport(ports[3]);
}

int getIDn(int port){
    unsigned char out[256];
    const char * in = "?\n";
    unsigned char * in2;
    stringstream ss;
    in2 = (unsigned char *) in;
    RS232_OpenComport(port, 115200);
    RS232_SendBuf(port, in2, 3);
    for (int i=0;i<10000;i++){
        RS232_PollComport(port,out,sizeof(out));
        ss.str("");
        ss<<out;
        string saadud=ss.str();
        std::size_t founded=saadud.find("id:");
        if (founded != string::npos){
            ss.str("");
            ss<<saadud.at(founded+3);
            int id=stoi(ss.str());
            cout<<": mootor leitud: id "<<id<<endl;
            //RS232_CloseComport(port);
            return id;
        }
        //sleep(1);
    }
    RS232_CloseComport(port);
    cout<<" tyhi port"<<endl;
    return -1;
}



int sendAsString(std::string task, int port){
    unsigned char * taskIn= (unsigned char *)task.c_str();
    stringstream ss;
    ss<<taskIn<<endl;
    //cout<<taskIn<<endl;
    //unsigned char taskOut[sizeof(taskIn)+1];
    //strncpy(taskOut,taskIn,sizeof(taskIn));
    //taskOut[sizeof(taskOut)]='\n';
    //cout<<taskOut;
    //usleep(1000);

    unsigned char end[1]={'\n'};
    std::size_t founded=task.find("-");
    if (founded != string::npos){
        RS232_SendBuf(port, taskIn, sizeof(taskIn));
    }
    else {
        RS232_SendBuf(port, taskIn, sizeof(taskIn));
    }
    RS232_SendBuf(port, end, 1);
    return 0;
}



void setSpeedForEng(int eng, int speed) {
	stringstream motorStream;
	string motorString;
	 motorStream << "sd" << speed << endl;
	 motorString = motorStream.str();
	 sendAsString(motorString,eng);
	 motorStream.str("");
}



void sendSpeedCheckForEng(int port){
    RS232_cputs(port, "s\n");
    
}

int readSpeedForEng(int port) {
	unsigned char answer[20] = {0};
    RS232_PollComport(port, answer, 100);
    stringstream ss;
    int i = 4;
    while (i < 20) {
		ss << answer[i];
		i+= 1;
		if (answer[i] == '>') break;
	}
    cout << "engCheck "<< ss.str() << endl;
    //usleep(1000000);
    return stoi(ss.str());
}



void cCoil(int port){
    //const char * in = "c\n";
    //unsigned char * in2;
    //in2 = (unsigned char *) in;
    //RS232_SendBuf(port, in2, 2);
    
    stringstream cStream;
	string cString;
	 cStream << "c" << endl;
	 cString = cStream.str();
	 sendAsString(cString, port);
	 cStream.str("");
}

void pCoil(int port){
    //const char * in = "p\n";
    //unsigned char * in2;
    //in2 = (unsigned char *) in;
    //RS232_SendBuf(coil[0], in2, 2);
    
    stringstream cStream;
	string cString;
	 cStream << "p" << endl;
	 cString = cStream.str();
	 sendAsString(cString, port);
	 cStream.str("");
}

void sCoil(int port){	 
	 
	 sendAsString( "fs0", port);
	 usleep(100000);
	 sendAsString( "c", port);
	 usleep(700000);
	 sendAsString( "k2000", port);
	 usleep(100000);
	 sendAsString( "fs1", port);
	 usleep(100000);
}

bool readPin(int port) {
	sendAsString("gb", port);
	unsigned char out[256];
	stringstream ss;
    for (int i=0;i<10000;i++){
        RS232_PollComport(port,out,sizeof(out));
        ss.str("");
        ss<<out;
        string saadud=ss.str();
        std::size_t founded=saadud.find("b:");
        if (founded != string::npos){
            ss.str("");
            ss<<saadud.at(founded+2);
            int id=stoi(ss.str());
            //cout<<"pall tribleris "<<id<<endl;
            if (id==1) return true;
            else return false;
        }
    }
    cout<<"tyhi port"<<endl;
    return -1;
}


