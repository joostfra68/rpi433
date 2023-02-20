#!/bin/bash
###############################################
# File build.sh
# Version 0.01
# Build all executable files.
# 20230213 FJ 0.01 Initial Release
###############################################

echo "You are in directory" 
pwd
g++ -o rpi433d rpi433d.cpp -I/usr/local/include  -L/usr/local/lib -lwiringPi -lrt
g++ -o rpi433queue rpi433queue.cpp -lrt
ln -s rpi433queue kaku
ln -s rpi433queue action
g++ -o rpi433send rpi433send.cpp -I/usr/local/include  -L/usr/local/lib -lwiringPi -lrt
g++ -o rpi433receive rpi433receive.cpp -I/usr/local/include  -L/usr/local/lib -lwiringPi -lrt

