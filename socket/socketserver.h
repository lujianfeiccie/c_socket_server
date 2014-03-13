#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>



int socketlisten(int inputfd,int port,void (*cleanupInput) (int inputfd));
void parse(void* lp);

extern inline int getCurrentRotation();
extern inline int isIdle();
extern void setIdle(int i);

#endif
