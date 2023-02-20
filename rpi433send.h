/* 
 * rpi433send application to operate wireless switches with a RaspberryPi
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

// Pin out using wiringPi pin numbering scheme (0 = TxD / BCM GPIO 17 / Hdr 11, see https://wiringpi.com)
#define PIN_OUT 0

enum deviceType{ action, oldkaku, newkaku, elro, blokker };  //  0, 1, 2, 3, 4

enum deviceCommand { off, on, dim };  //  0, 1, 2

