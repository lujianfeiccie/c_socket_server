#ifndef OTHERTOOL_H
#define OTHERTOOL_H
int getIntFromBytes(unsigned char b[],int startIndex,int size);
void getBytesFromInt(unsigned char* b,int num);
void getKeyFromBytes(int* key, unsigned char b[]);
void getMouseFromBytes(int* x,int *y, unsigned char b[]);
#endif
