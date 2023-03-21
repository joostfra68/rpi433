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

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <iostream>
#include <string.h>
#include <string>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include "RemoteSwitch.cpp"
#include "NewRemoteTransmitter.cpp"
#include "rpi433d.h"

using namespace std;

//int intStop;

void send433mhz(deviceType devicetype, int address, int device, deviceCommand command, int value)
{

	switch (devicetype)
    {
        case newkaku:
        {
        	NewRemoteTransmitter transmitter(address, PIN_OUT, 260, 3);
        	if (device == 0)
        	{
    			printf (" device=newkaku address1=%d address2=%d command=%s\n", address, device, command==on?"on":"off");
        		transmitter.sendGroup(command == on);
        	}
        	else
        	{
        		if (command != dim)
        		{
        			printf (" device=newkaku address1=%d address2=%d command=%s\n", address, device, command==on?"on":"off");
        			transmitter.sendUnit(device, command == on);
        		}
        		else
        		{
        			printf (" device=newkaku address1=%d address2=%d command=dim value=%d\n", address, device, value);
        			transmitter.sendDim(device, value);
        		}
        	}
        }
        break;
        case oldkaku:
        {
            printf (" device=oldkaku address1=%c address2=%d command=%s\n", address, device, command==on?"on":"off");
            KaKuSwitch kaKuSwitch(PIN_OUT);
            kaKuSwitch.sendSignal(address, device, command == on);
        }
        break;
        case action:
        {
            printf (" device=action address1=%d address2=%d command=%s\n", address, device, command==on?"on":"off");
            ActionSwitch actionSwitch(PIN_OUT);
            actionSwitch.sendSignal(address, device, command == on);
        }
        break;
    }
}

// Define the function to be called when ctrl-c (SIGINT) is sent to process
//void signal_callback_handler(int signum) {
//   cout << "Caught signal " << signum << endl;
   // Terminate program
   //exit(signum);
//   intStop = 1;
//   printf("Daemon stopped, exit after next message.");
//}

int main()
{
    mqd_t mqd;
    struct mq_attr attr;
    int ret;
    char msg[MQ_MESSAGE_MAX_LENGTH];
    char mesg[MQ_MESSAGE_MAX_LENGTH];
    ssize_t msg_len;
    string strMesg;
    string strAddr;
    int intDevt;
    int intAddr;
    int intUnit;
    int intComm;
    int intLevl;

//        intStop = 0;
//        signal(SIGINT, signal_callback_handler);

    // load wiringPi
    if(wiringPiSetup() == -1)
    {
        printf("WiringPi setup failed. Maybe you haven't installed it yet?");
        exit(1);
    }
                            
    // setup pin and make it low (otherwise transmitter will block other 433 mhz transmitters like remotes)
    pinMode(PIN_OUT, OUTPUT);
    digitalWrite(PIN_OUT, LOW);
 
    /* Form the queue attributes */
    attr.mq_flags = 0; /* i.e mq_send will be block if message queue is full */
    attr.mq_maxmsg = MQ_MAX_NUM_OF_MESSAGES;
    attr.mq_msgsize = MQ_MESSAGE_MAX_LENGTH;
    attr.mq_curmsgs = 0; /* mq_curmsgs is dont care */

    /* Create message queue */
    mqd = mq_open(MQ_NAME, O_RDONLY | O_CREAT, MQ_MODE, &attr);
    if( mqd != (mqd_t)-1 )
    {
        printf(" Message Queue Opened\n");

        while (1)
//        while (intStop == 0)
        {
            printf(" Receiving message .... \n");
            msg_len = mq_receive(mqd, msg, MQ_MESSAGE_MAX_LENGTH, NULL);
            if(msg_len < 11)
            {
                printf(" Failed");
            }
            else
            {
                printf(" Successfully received %d bytes, ", (int)msg_len);
                strMesg = msg;
                printf(" Msg %s ", strMesg.c_str());
                strAddr = strMesg.substr(0, 1);
                intDevt = atoi(strAddr.c_str());
                printf(" Devt %s ", strAddr.c_str());
                strAddr = strMesg.substr(1, 8);
                intAddr = atoi(strAddr.c_str());
                printf(" Addr %s ", strAddr.c_str());
                strAddr = strMesg.substr(9, 3);
                intUnit = atoi(strAddr.c_str());
                printf(" Unit %s ", strAddr.c_str());
                strAddr = strMesg.substr(12, 1);
                intComm = atoi(strAddr.c_str());
                printf(" Comm %s ", strAddr.c_str());
                strAddr = strMesg.substr(13, 2);
                printf(" Lev %s \n", strAddr.c_str());
                intLevl = atoi(strAddr.c_str());
                if (msg_len > 11) send433mhz((deviceType) intDevt, intAddr, intUnit, (deviceCommand)  intComm, intLevl);
            }
        }
                
        /* Close the message queue */
        ret = mq_close(mqd);
        if(ret) perror(" Message queue close failed");
        else printf(" Message Queue Closed\n");
        
        ret = mq_unlink(MQ_NAME);
        if(ret) perror(" Message queue unlink failed");
        else printf(" Message Queue unlinked\n");
    }
    else
    {
        perror(" Message queue open failed ");
    }

    return 0;
}

