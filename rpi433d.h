/*
 * rpi433d daemon application to operate wireless switches with a RaspberryPi
 * rpi433queue sends commands to rpi433d to process them in background
 * Version 1.0.0 (20230220) By Frank Joosten https://frankjoosten.nl
 *
 * Makes use of:
 *
 * RemoteSwitch library v2.0.0 made by Randy Simons http://randysimons.nl
 * See RemoteSwitchSender.h for details.
 *
 * NewRemoteTransmitter library v1.2.0 (20140128) made by Randy Simons http://randysimons.nl/
 * See NewRemoteTransmitter.h for details.
 *
 * License: GPLv3. See license.txt
 */

#define MQ_NAME "/rpi433d"
#define MQ_MESSAGE_MAX_LENGTH 16  //  1 + 8 + 3 + 1 + 2
#define MQ_MAX_NUM_OF_MESSAGES 10
#define MQ_MODE (S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH)
#define MQ_MESSAGE_PRIORITY 0

// Pin out using wiringPi pin numbering scheme (15 = TxD / BCM GPIO 14, see https://projects.drogon.net/raspberry-pi/wiringpi/pins/)
// #define PIN_OUT 15
#define PIN_OUT 0

enum deviceType{ action, oldkaku, newkaku };  //  0, 1, 2

enum deviceCommand { off, on, dim };  //  0, 1, 2

