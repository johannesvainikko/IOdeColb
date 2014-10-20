IO de Colb
========

Robotex 2014

ThreshTest build:
g++ threshTest.cpp  -o threshTest `pkg-config --cflags --libs opencv`

--------------------

Main program compile command (2 warnings on Ubuntu 14.04)
sudo g++ -std=c++11 -Wall main.cpp globals.cpp pall.cpp pildiT66tlus.cpp RobotManager.cpp serial.cpp rs232.c -o test `pkg-config --cflags --libs opencv`

