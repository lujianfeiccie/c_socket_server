#include <stdio.h>
#include <string.h>
#include "event/input.h"
#include "socket/socketserver.h"
#include <pthread.h>
#define LEFT_CLICK 0x01
#define RIGHT_CLICK 0x04
#define MID_CLICK 0x02
#define OTHER 0x03

#define KEYBOARD_METHOD 2
#define MOUSE_METHOD 3
#define PLAN1
#define SOCKET

int main(int argc,char** args)
{
 initInput();
 //printf("inputfd:%d\n",inputfd);
 socketlisten(inputfd,6666,cleanupInput);
 cleanupInput(inputfd);
 return 0;
}
