#!/bin/bash
###############################################
# File clean.sh
# Version 0.01
# Cleanup all built files.
# 20230213 FJ 0.01 Initial Release
###############################################

echo "You are in directory" 
pwd
rm -f rpi433send rpi433d
rm -f rpi433receive rpi433queue

