/* 
 * rpi433send application to operate wireless switches with a RaspberryPi
 * Version 1.2.1 (20230220) By Frank Joosten https://frankjoosten.nl 
 *
 * Makes use of:
 * 
 * NewRemoteSwitch library v1.2.1 (20230220) modified by Frank Joosten https://frankjoosten.nl
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
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
// #include "RemoteSwitch.cpp"      //  for old devices
#include "NewRemoteTransmitter.cpp"
#include "rpi433send.h"

using namespace std;

void sendcode(deviceType devtyp, int address, int device, deviceCommand command, int value)
{
    // Leave deviceType in for future possibilities, with old devices ;-)
    switch (devtyp)
    {
        case newkaku:
        {
        NewRemoteTransmitter transmitter(address, PIN_OUT, 260, 3);
        if (device == 0)
	{
            transmitter.sendGroup(command == on);
        }
        else
        {
            if (command != dim)
            {
//                printf ("device=newkaku address1=%d address2=%d command=%s\n", address, device, command==on?"on":"off");
                printf ("Code: address1=%d address2=%d command=%s\n", address, device, command==on?"on":"off");
                transmitter.sendUnit(device, command == on);
            }
            else
            {
//                printf ("device=newkaku address1=%d address2=%d command=dim value=%d\n", address, device, value);
                printf ("Code: address1=%d address2=%d command=dim value=%d\n", address, device, value);
                transmitter.sendDim(device, value);
            }
        }
        }
        break;
/*        case oldkaku:       //  for old devices
        {
            printf ("device=oldkaku address1=%c address2=%d command=%s\n", address, device, command==on?"on":"off");
            KaKuSwitch kaKuSwitch(PIN_OUT);
            kaKuSwitch.sendSignal(address, device, command == on);
        }
        break;
        case elro:
        {
            printf ("device=elro address1=%d address2=%d command=%s\n", address, device, command==on?"on":"off");
            ElroSwitch elroSwitch(PIN_OUT);
            elroSwitch.sendSignal(address, device, command == on);
        }
        break;
        case action:
        {
            printf ("device=action address1=%d address2=%d command=%s\n", address, device, command==on?"on":"off");
            ActionSwitch actionSwitch(PIN_OUT);
            actionSwitch.sendSignal(address, device, command == on);
        }
        break;
        case blokker:
        {
            printf ("device=blokker address1=%d command=%s\n", device, command==on?"on":"off");
            BlokkerSwitch blokkerSwitch(PIN_OUT);
            blokkerSwitch.sendSignal(device, command == on);
        }
        break;  */
    }
}

int main(int argc, char *argv[])
{
    int intAdr1;
    int intAdr2;
    int intCmd;
    deviceCommand devCmd;
    int intLev;
    
    // load wiringPi
    if(wiringPiSetup() == -1)
    {
        printf("WiringPi setup failed.");
        exit(1);
    }
                            
    // setup pin and make it low (otherwise transmitter will block other 433 mhz transmitters like remotes)
    pinMode(PIN_OUT, OUTPUT);
    digitalWrite(PIN_OUT, LOW);
         
    if( argc < 4 ) 
    { // not enough arguments
        printf("Usage: %s address device state level (yet only for new KaKu type A) \n", argv[0]);
        printf("Example: %s 1234567 10 dim 5 \n", argv[0]);
        printf("Example: %s 1234567 10 [on|off] \n", argv[0]);
        exit(1);
    }
    else
    {
        intAdr1 = atoi(argv[1]);
        intAdr2 = atoi(argv[2]);
        intCmd = atoi(argv[3]);
        if (string(argv[3]) == "on") devCmd = on;  //  std::string(argv[1]) == "yes"
        if (string(argv[3]) == "off") devCmd = off;
        if (string(argv[3]) == "dim") devCmd = dim;
        if (argc > 4) intLev = atoi(argv[4]);
        else intLev = 0;
        printf("Send code: %d %d %s %d \n", intAdr1, intAdr2, argv[3], intLev);
        sendcode((deviceType) 2, intAdr1, intAdr2, devCmd, intLev);
    }
    return 0;
}

