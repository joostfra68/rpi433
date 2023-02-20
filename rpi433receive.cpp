/* 
 * rpi433receive application to read wireless senders with a RaspberryPi
 * Pi needs to be idle for timing to work correctly
 * Version 1.0.0 (20230220) By Frank Joosten https://frankjoosten.nl 
 *
 * Makes use of:
 *
 * NewRemoteReceiver library v1.2.0 (20140128) made by Randy Simons http://randysimons.nl/
 * See NewRemoteReceiver.h for details.
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
#include "NewRemoteReceiver.cpp" 
// #include "rpi433receive.h"

// Callback function is called only when a valid code is received.
void showCode(NewRemoteCode receivedCode) {
  // Note: interrupts are disabled. You can re-enable them if needed.

  // Print the received code.
  printf("Addr %ld", receivedCode.address);

  if (receivedCode.groupBit) {
    printf(" group");
  } 
  else {
    printf(" unit %d", receivedCode.unit);
  }

  switch (receivedCode.switchType) {
    case NewRemoteCode::off:
      printf(" off");
      break;
    case NewRemoteCode::on:
      printf(" on");
      break;
    case NewRemoteCode::dim:
      printf(" dim");
      break;
  }

  if (receivedCode.dimLevelPresent) {
    printf(", dim level: %d", receivedCode.dimLevel);
  }

  printf(", period: %d us. \n", receivedCode.period);
}  

int main(int argc, char *argv[]) {

    printf("WiringPi setting up \n");
    // load wiringPi
    if(wiringPiSetup() == -1)
    {
        printf("WiringPi setup failed. \n");
        exit(1);
    } else {
        printf("WiringPi set up, listening for codes..... \n");
    }

  NewRemoteReceiver receiver(2, 2, showCode);

  // Main program loop
  while(1)  
  {
  // Toggle the LED
  //digitalWrite(0, !digitalRead(0));
  //delay(5);
  //  printf(".");
  //  usleep(1000);
    sleep(1);
  } 

  return 0;

}
