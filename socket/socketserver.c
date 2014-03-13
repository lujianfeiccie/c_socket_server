#include "socketserver.h"
#include "../util/othertool.h"
#include "../event/input.h"
#include <pthread.h>

#define FLAG_HEADER_1 0x0a
#define FLAG_HEADER_2 0x86
#define FLAG_MOUSE    0x12
#define FLAG_KEYBOARD 0x13
#define FLAG_EXIT     0xff

#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

#define MAXSIZE 1024
#define PRINTF  0

typedef struct
{
	int inputfd;
	int connfd;
	int listenfd;
	void (*cleanupInput) (int inputfd);
}*LP_DATA, DATA;


int rotation = 0,idle = 0;
inline int getCurrentRotation()
{
    return rotation;;
}

inline int isIdle()
{
    return idle;
}

void setIdle(int i)
{
   idle = i;
}

int socketlisten(int inputfd, int port, void (*cleanupInput)(int inputfd))
 {
    //����һ��socket��ַ�ṹserver_addr,���������internet��ַ, �˿�
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); //��һ���ڴ���������ȫ������Ϊ0
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

    //��������internet����Э��(TCP)socket,��server_socket���������socket
    int server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }

   //printf("Create Socket ok!\n");
   int opt =1;
   setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    //��socket��socket��ַ�ṹ��ϵ����
    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", port);
        exit(1);
    }
    //printf("Server Bind Port:%d ok!\n", port);

    //server_socket���ڼ���
    if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!");
        exit(1);
    }
    printf("Server Listen ......\n");
    while (1) //��������Ҫһֱ����
    {
        //����ͻ��˵�socket��ַ�ṹclient_addr
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

        //����һ����server_socket�����socket��һ������
        //���û����������,�͵ȴ�������������--����accept����������
        //accept��������һ���µ�socket,���socket(new_server_socket)����ͬ���ӵ��Ŀͻ���ͨ��
        //new_server_socket�����˷������Ϳͻ���֮���һ��ͨ��ͨ��
        //accept���������ӵ��Ŀͻ�����Ϣ��д���ͻ��˵�socket��ַ�ṹclient_addr��
        //printf("Server begin accept ...........!\n");
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            printf("Server Accept Failed!\n");
            break;
        }

        printf("Server Accept new one!\n");
        LP_DATA data = malloc(sizeof(DATA));
		data->connfd = new_server_socket;
		data->inputfd = inputfd;
		data->listenfd = server_socket;
		data->cleanupInput = cleanupInput;

        //Create a thread
		pthread_t id;
		int err = 0;
        err = pthread_create(&id, NULL, (void*) parse, (void*) data);
		if( err != 0 )
		{
            printf("can't create thread:%s\n",strerror(err));
        }
#if PRINTF
         printf("create thread ok: %s\n",strerror(err));
#endif
    }
        //�رռ����õ�socket
    close(server_socket);
	return 0;
}



//�����ַ���
int send_str(int sock,const char *str)
{
    int byte;
    int len;
    len = strlen(str);
    byte = send_byte(sock,str,len);
    if(byte < 0)
        return -1;
    return byte;
}

//����len���ֽ�
int send_byte(int sock,char *buf,int len)
{
    int rc;
    int byte;
    for(byte = 0;byte <len ; byte += rc)
    {
       rc = send(sock,buf + byte,len - byte,MSG_NOSIGNAL);
       if(rc < 0 )
       {
          byte = -1;
          break;
       }
    }
    return byte;
}


void mouseEvent(int nCount,unsigned char phoneDirection,unsigned char mouseStatus,int x,int y,int width,int height)
{
   if(0x01 == mouseStatus) //����
  {
#if PRINTF
      printf("point:%d  phoneDir:%d down,x:%d y:%d\n",nCount,phoneDirection,x,y);
#endif
      ptrEvent(1,x,y,width,height);
   }
   else if(0x00 == mouseStatus )//̧��
   {
#if PRINTF
      printf("point:%d phoneDir:%d up,x:%d y:%d\n",nCount,phoneDirection,x,y);
#endif
      ptrEvent(0,x,y,width,height);
   }

}

void parse(void* lp)
{
    printf("new thread client connected!\n");

	LP_DATA data = (LP_DATA)lp;
    unsigned char buffer[MAXSIZE];
	int n;
	int i,j,k;
	int m = 1;

    //���ͻ���һ����ϢConnectOk
    unsigned char *pClientMsg = "ConnectOk";
    send_str(data->connfd,pClientMsg);
	while (m == 1)
    {
		n = recv(data->connfd, buffer, MAXSIZE, 0);
        if(n>0)
        {
                buffer[n] = '\0';
#if PRINTF
                         printf("recv msg from client,len:%d\n", n);
                         printf("data16:");
                         for(j=0;j<n;j++)
                         {
                           printf("0x%.2x ",buffer[j]);
                         }
                         printf("\n");
#endif


          //��ͷ(1 Bytes)	������(1Byte)	���ݳ���(1Byte)	��������10-30	У���1Byte
          if(n >= 4)
          {
               int m;
               int sum = 0; //����У���
               for(m=0; m < n; m++)
               {
                  if(m == 0)
                  {
                     if(buffer[m] != FLAG_HEADER_1) //0x0a
                     {
                        break;
                     }
                  }

                  if(m >= 1 && m<= n-2)
                  {
                     sum += buffer[m];//�����У���
                  }
               }
#if PRINTF
               printf("Recv check sum:0x%.2x\n",buffer[n-1]);
               printf("Calc check sum:0x%.2x\n",sum);
#endif

               //�ж�У����Ƿ���ȷ
               if((unsigned char)sum != buffer[n-1])
               {

#if PRINTF
         printf("data check fail!\n");
#endif

                  continue;
               }
               else
               {
                 //printf("data check ok!\n");
               }
        		switch (buffer[1])//������
                {
            		case FLAG_MOUSE: //����¼�
                        {

                           unsigned char  phoneDirection=  buffer[3];//�ֻ�����
                           int width  =  buffer[4]  << 8| buffer[5];
                           int height =  buffer[6]  << 8| buffer[7];
                           if (0 == phoneDirection)  //0: 0��,   ����
                           {
                              rotation = 0;
                           }
                           else if (1 == phoneDirection)  //1: 90��,  ����
                           {
                                rotation = 90;
                           }
                           else if(2 == phoneDirection)  //2: 180��, ����
                           {
                                rotation = 180;
                           }
                           else if(3 == phoneDirection)   //3: 270��, ����
                           {
                                rotation = 270;
        			       }
                           //printf("phoneDirection:%d width:%d  height:%d\n ",phoneDirection,width,height);

                           //�жϼ�������
                           int  nCount = 0;
                           if(buffer[2] == 10)  //1��
                           {
                               nCount = 1;

                               //���״̬������:  0x01 ̧��:  0x00
                               unsigned char mouseStatus = buffer[8];//���״̬
                               int x  =  buffer[9] << 8| buffer[10];
                               int y  =  buffer[11] << 8| buffer[12];
                               mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);
                           }
                           else if(buffer[2] == 15) //2��
                          {
                               nCount = 2;

                               //���״̬������:  0x01 ̧��:  0x00
                               unsigned char mouseStatus = buffer[8];//���״̬
                               int x  =  buffer[9] << 8| buffer[10];
                               int y  =  buffer[11] << 8| buffer[12];
                               mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                               mouseStatus = buffer[13];//���״̬
                               x  =  buffer[14] << 8| buffer[15];
                               y  =  buffer[16] << 8| buffer[17];
                               mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                           }
                           else if(buffer[2] == 20) //3��
                           {
                                nCount = 3;

                                //���״̬������:  0x01 ̧��:  0x00
                                unsigned char mouseStatus = buffer[8];//���״̬
                                int x  =  buffer[9] << 8| buffer[10];
                                int y  =  buffer[11] << 8| buffer[12];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[13];//���״̬
                                x  =  buffer[14] << 8| buffer[15];
                                y  =  buffer[16] << 8| buffer[17];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[18];//���״̬
                                x  =  buffer[19] << 8| buffer[20];
                                y  =  buffer[21] << 8| buffer[22];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);
                           }
                           else if(buffer[2] == 25) //4��
                           {
                                nCount = 4;

                                //���״̬������:  0x01 ̧��:  0x00
                                unsigned char mouseStatus = buffer[8];//���״̬
                                int x  =  buffer[9] << 8| buffer[10];
                                int y  =  buffer[11] << 8| buffer[12];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[13];//���״̬
                                x  =  buffer[14] << 8| buffer[15];
                                y  =  buffer[16] << 8| buffer[17];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[18];//���״̬
                                x  =  buffer[19] << 8| buffer[20];
                                y  =  buffer[21] << 8| buffer[22];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[23];//���״̬
                                x  =  buffer[24] << 8| buffer[25];
                                y  =  buffer[26] << 8| buffer[27];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);
                           }
                           else if(buffer[2] == 30) //5��
                           {
                                nCount = 5;
                                //printf("mouse num:%d\n",nCount);

                                //���״̬������:  0x01 ̧��:  0x00
                                unsigned char mouseStatus = buffer[8];//���״̬
                                int x  =  buffer[9] << 8| buffer[10];
                                int y  =  buffer[11] << 8| buffer[12];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[13];//���״̬
                                x  =  buffer[14] << 8| buffer[15];
                                y  =  buffer[16] << 8| buffer[17];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[18];//���״̬
                                x  =  buffer[19] << 8| buffer[20];
                                y  =  buffer[21] << 8| buffer[22];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[23];//���״̬
                                x  =  buffer[24] << 8| buffer[25];
                                y  =  buffer[26] << 8| buffer[27];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);

                                mouseStatus = buffer[28];//���״̬
                                x  =  buffer[29] << 8| buffer[30];
                                y  =  buffer[31] << 8| buffer[32];
                                mouseEvent(nCount,phoneDirection,mouseStatus,x,y,width,height);
                           }
            			}
            			break;
            		case FLAG_KEYBOARD:
                        {
                            #if PRINTF
                			printf("keyboard enent give up!\n");
                            #endif
            		    }
            			break;
            		case FLAG_EXIT: //��������
                        {
                            #if PRINTF
                            printf("kill server,exit!!\n");
                            #endif

                    		if (data->connfd != -1)
                    			close(data->connfd);
                    		if (data->listenfd != -1)
                    			close(data->listenfd);
                    		data->cleanupInput(data->inputfd);
                    		exit(0);
            		     }
            			break;
        		     }
		  }
          else
          {
             #if PRINTF
             printf("n<4 give up!  n=%d \n",n);
             #endif
          }
	   }
        else
        {
            //printf("%d \n",n);
            if (data->connfd != -1)
            {
                close(data->connfd);
                m = 0;

              #if PRINTF
              printf("close data->connfd ok.\n",n);
              #endif

            }
        }
	}
}
