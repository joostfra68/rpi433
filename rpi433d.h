#define MQ_NAME "/rpi433d"
#define MQ_MESSAGE_MAX_LENGTH 15  //  1 + 8 + 2 + 1 + 2
#define MQ_MAX_NUM_OF_MESSAGES 10
#define MQ_MODE (S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH)
#define MQ_MESSAGE_PRIORITY 0

// Pin out using wiringPi pin numbering scheme (15 = TxD / BCM GPIO 14, see https://projects.drogon.net/raspberry-pi/wiringpi/pins/)
// #define PIN_OUT 15
#define PIN_OUT 0

enum deviceType{ action, oldkaku, newkaku };  //  0, 1, 2

enum deviceCommand { off, on, dim };  //  0, 1, 2

