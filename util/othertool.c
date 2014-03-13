

#include "othertool.h"
int getIntFromBytes(unsigned char b[],int startIndex,int size){
 int result=0;
 int i;
 int endIndex = startIndex + size;
 for(i=startIndex;i<endIndex;i++){
   //result * 256
   result = ( result << 8 ) + b[i];
 }
 return result;
}
void getBytesFromInt(unsigned char *b,int num){
  b[0] = num>>24; //num / 2^24;
  b[1] = num>>16; //num / 2^16;
  b[2] = num>>8;  //num / 2^8;
  b[3] = (unsigned char)num;
}
void getKeyFromBytes(int* key, unsigned char b[]){
	*key = getIntFromBytes(b,3,4);
}
void getMouseFromBytes(int* x,int *y, unsigned char b[]){
	*x = getIntFromBytes(b,3,4);
	*y = getIntFromBytes(b,7,4);
}
