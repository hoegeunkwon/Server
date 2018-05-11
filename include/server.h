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

#include "shieldsql.h"

#define BUFF_SIZE 512
#define FIELD_SIZE 20
#define ACCESS_USER_SIZE 500
#define SERVER_PORT 55555

#define DEBUG
#ifdef DEBUG
#define LOG(x) do{x}while(0);
#else
#define LOG(x)
#endif

int accessUserNum;
UserInfo accessUserArr[ACCESS_USER_SIZE];
pthread_mutex_t mutex;

int createServer(int port);
void* connectClient(void* arg);
void disconnectClient(int clientSockFd);
void insertClientData(int clientSockFd, UserInfo* data);

void parentsClient(int clientSockFd, char* buff);
void childClient(int clientSockFd, char* buff);

int checkParentsID(char* id);
int checkParentsPW(char* id, char* pw, UserInfo* data);
int joinParents(char* id, char* pw);
int getRandGroupid(MYSQL** db, MYSQL_RES** res);

int checkChildLogin(char* name, char* groupid, UserInfo* data);
int checkChildName(char* name, char* groupid);
int joinChild(char* name, char* groupid);

int sendMsg(int fd, char* msg, int msgSize);
int getFindClientFd(char* name, int groupid);

int fileMediation(unsigned int fileSize, int toFd, int fromFd);


void testDisplay(UserInfo* data);

//int sendMsg(int, char*, char*, int);
//int msgParsing(Msg*, const char*);

#endif
