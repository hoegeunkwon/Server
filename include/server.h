#ifndef _SERVER
#define _SERVER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "./../include/shieldsql.h"

#define BUFF_SIZE 512
#define NAME_SIZE 20
#define ACCESS_USER_SIZE 100
#define SERVER_PORT 5555

#define DEBUG
#ifdef DEBUG
#define LOG(x) do{x}while(0);
#else
#define LOG(x)
#endif

int accessUserNum;
UserInfo accessUserArr[ACCESS_USER_SIZE];
pthread_mutex_t mutex;

int createServer(int);
void* connectClient(void*);
//int sendMsg(int, char*, char*, int);
//int msgParsing(Msg*, const char*);

#endif
