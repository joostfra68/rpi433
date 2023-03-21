/*
 * rpi433queue application to operate wireless switches with a RaspberryPi
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

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <iostream>
#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

#include "rpi433d.h"

using namespace std;

//int main(int argc, char **argv)
int main(int argc, char *argv[])
{
    mqd_t mqd;
    int ret;
    char msg[MQ_MESSAGE_MAX_LENGTH];
    char addr[8];
    ssize_t msg_len;
//    string strMsg;
    string programname = argv[0];
    int intAdr1;
    int intAdr2;
    int intCmd;
    deviceCommand devCmd;
    string strCmd;
    char chrCmd;
    int intLev;
    string strAddr;
    int intAddrLen;

    if( argc < 4 ) 
    { // not enough arguments
        printf("Usage: %s address device state level (yet only for new KaKu type A) \n", argv[0]);
        printf("Example: %s 1234567 10 dim 5 \n", argv[0]);
        printf("Example: %s 1234567 10 [on|off] \n", argv[0]);
        exit(1);
    }
    else
    {
    	// Message = 1 + 8 + 3 + 1 + 2
        intAdr1 = atoi(argv[1]);
        intAdr2 = atoi(argv[2]);
        if (string(argv[3]) == "off") {
        	devCmd = off;  //  std::string(argv[1]) == "yes"
        	intCmd = 0;
        	chrCmd = '0';
        }
        if (string(argv[3]) == "on") {
        	devCmd = on;
        	intCmd = 1;
        	chrCmd = '1';
        }
        if (string(argv[3]) == "dim") {
        	devCmd = dim;
        	intCmd = 2;
        	chrCmd = '2';
        }
        strCmd = to_string(intCmd);
        if (argc > 4) intLev = atoi(argv[4]);
        else intLev = 0;

        // send msg to screen
        printf(" Send code: %d %d %s %d \n", intAdr1, intAdr2, argv[3], intLev);
        
        strAddr = to_string(intAdr1);
        intAddrLen = strAddr.length();
        sprintf(addr,"%d",intAdr1);
        msg[0] = '2';
        msg[1] = intAddrLen>=8?addr[intAddrLen-8]:'0';
        msg[2] = intAddrLen>=7?addr[intAddrLen-7]:'0';
        msg[3] = intAddrLen>=6?addr[intAddrLen-6]:'0';
        msg[4] = intAddrLen>=5?addr[intAddrLen-5]:'0';
        msg[5] = intAddrLen>=4?addr[intAddrLen-4]:'0';
        msg[6] = intAddrLen>=3?addr[intAddrLen-3]:'0';
        msg[7] = intAddrLen>=2?addr[intAddrLen-2]:'0';
        msg[8] = intAddrLen>=1?addr[intAddrLen-1]:'0';
        strAddr = to_string(intAdr2);
        sprintf(addr,"%d",intAdr2);
        intAddrLen = strAddr.length();
        msg[9] = intAddrLen>=3?addr[intAddrLen-3]:'0';
        msg[10] = intAddrLen>=2?addr[intAddrLen-2]:'0';
        msg[11] = intAddrLen>=1?addr[intAddrLen-1]:'0';
        msg[12] = chrCmd; // char(intCmd);
        strAddr = to_string(intLev);
        sprintf(addr,"%d",intLev);
        intAddrLen = strAddr.length();
        msg[13] = intAddrLen>=2?addr[intAddrLen-2]:'0';
        msg[14] = intAddrLen>=1?addr[intAddrLen-1]:'0';
        msg[15] = '\0';
//        printf("The message = %s \n", msg);

     	/* Open the message queue. Message queue is already created from a different process */
      	mqd = mq_open(MQ_NAME, O_WRONLY);
        if( mqd != (mqd_t)-1 )
       	{
//                printf(" Message Queue Opened\n");
             printf(" Sending message to rpi433d.... ");
             ret = mq_send(mqd, msg, MQ_MESSAGE_MAX_LENGTH, MQ_MESSAGE_PRIORITY);
             if(ret)
                 perror("Failed");
	          else
        	        printf("Done\n");
                 /* Close the message queue */
             ret = mq_close(mqd);
             if(ret)
                 perror(" Message queue close failed");
             else
                 printf(" Message Queue Closed\n");
       	}
        else
       	{
             perror(" Message queue open failed, is rpi433d running?");
             exit(4);
       	}
    }
    return 0;
}
