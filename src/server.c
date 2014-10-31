#include "./../include/server.h"

int createServer(int port)
{
	int sockFd;
	struct sockaddr_in serverAddr;

	if( (sockFd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
		perror("socket");
		exit(0);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(port);

	if(bind(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("bind");
		exit(0);
	}

	if(listen(sockFd, 5) < 0) {
		perror("listen");
		exit(0);
	}

	return sockFd;
}

void* connectClient(void* arg)
{
	int clientSockFd = *((int*)arg);
	char buff[BUFF_SIZE];

	// check parents or child
	read(clientSockFd, buff, BUFF_SIZE);
	if(strcmp(buff, "parents") == 0) {
		parentsClient(clientSockFd, buff);
	} else if(strcmp(buff, "child") == 0) {
		childClient(clientSockFd, buff);
	}

	disconnectClient(clientSockFd);

	return 0;
}

void disconnectClient(int clientSockFd)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i=0; i<accessUserNum; i++) {
		if(accessUserArr[i].fd == clientSockFd) {
			accessUserArr[i] = accessUserArr[accessUserNum-1];
			--accessUserNum;
			close(clientSockFd);
			break;
		}
	}
	printf("DisConnected Now AccessUserNum : %d\n", accessUserNum);
	LOG(
		int i;
		for(i=0; i<accessUserNum; i++) {
			printf("%d ", accessUserArr[i].fd);
		}
		puts("");
	)
	pthread_mutex_unlock(&mutex);
}

void insertClientData(int clientSockFd, UserInfo* data)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i=0; i<accessUserNum; i++) {
		if(accessUserArr[i].fd == clientSockFd) {
			memcpy(accessUserArr+i, data, sizeof(UserInfo));
			testDisplay(accessUserArr+i);
			break;
		}
	}
	pthread_mutex_unlock(&mutex);
}

void parentsClient(int clientSockFd, char* buff)
{
	int buffSize;
	char* p;
	char sendBuff[BUFF_SIZE];
	UserInfo data;

	puts("parents fun");
	// join parents check
	while( (buffSize = read(clientSockFd, buff, BUFF_SIZE)) != 0) {
		char tmp[FIELD_SIZE];
		char id[FIELD_SIZE];
		char pw[FIELD_SIZE];

		p = strtok(buff, " "); if(p == 0) continue;
		strcpy(tmp, p);
		p = strtok(NULL, " "); if(p == 0) continue;
		strcpy(id, p);
		p = strtok(NULL, " "); if(p == 0) continue;
		strcpy(pw, p);

		printf("%s %s %s\n", tmp, id, pw);

		if(strcmp(tmp, "login") == 0) {		// login msg
			if(checkParentsPW(id, pw, &data) == TRUE) {
				puts("Login Success");
				data.fd = clientSockFd; 
				//testDisplay(&data);
				insertClientData(clientSockFd, &data);
				sprintf(sendBuff, "groupid %d", data.groupid);
				sendMsg(clientSockFd, sendBuff, BUFF_SIZE);
				break;
			} else {
				puts("Login Fail");
			}
		}
		
		printf("%s %s %s\n", tmp, id, pw);
		if(strcmp(tmp, "join") == 0) {	// join msg
			if(checkParentsID(id) == FALSE) {	// possible join
				puts("Possible join");
				if(joinParents(id, pw) == TRUE) {
					puts("Success Join");
				} else {
					puts("Not Join");
				}
			} else {
				puts("Join Fail");
			}
		}
	}

	// after login		// toName|fromName|msg
	while( (buffSize = read(clientSockFd, buff, BUFF_SIZE)) != 0) {
		int toFd;
		char cmd[FIELD_SIZE];
		char toName[FIELD_SIZE];
		char msg[BUFF_SIZE];

		p = strtok(buff, "|"); if(p == 0) continue;
		strcpy(cmd, p);
		p = strtok(NULL, "|"); if(p == 0) continue;
		strcpy(toName, p);
		p = strtok(NULL, "|"); if(p == 0) continue;
		strcpy(msg, p);

		toFd = getFindClientFd(toName, data.groupid);
		
		if(strcmp(cmd, "msg") == 0) {
			sprintf(sendBuff, "%s|%s|%s", cmd, data.name, msg);
			sendMsg(toFd, sendBuff, BUFF_SIZE);
		} else if(strcmp(cmd, "file") == 0) {
			sprintf(sendBuff, "%s|%s|%s", cmd, data.name, msg);
			sendMsg(toFd, sendBuff, BUFF_SIZE);
		}
	}

}

void childClient(int clientSockFd, char* buff)
{
	int buffSize;
	char* p;
	char sendBuff[BUFF_SIZE];
	UserInfo data;

	puts("child fun");
	// join child check
	while( (buffSize = read(clientSockFd, buff, BUFF_SIZE)) != 0) {
		char tmp[FIELD_SIZE];
		char name[FIELD_SIZE];
		char groupid[FIELD_SIZE];

		p = strtok(buff, " "); if(p == 0) continue;
		strcpy(tmp, p);
		p = strtok(NULL, " "); if(p == 0) continue;
		strcpy(name, p);
		p = strtok(NULL, " "); if(p == 0) continue;
		strcpy(groupid, p);

		printf("%s %s %s\n", tmp, name, groupid);

		if(strcmp(tmp, "login") == 0) {		// login msg
			if(checkChildLogin(name, groupid, &data) == TRUE) {
				puts("Login Success");
				data.fd = clientSockFd; 
				//testDisplay(&data);
				insertClientData(clientSockFd, &data);
				break;
			} else {
				puts("Login Fail");
			}
		}
		
		if(strcmp(tmp, "join") == 0) {	// join msg
			if(checkChildName(name, groupid) == FALSE) {	// possible join
				puts("Possible join");
				if(joinChild(name, groupid) == TRUE) {
					puts("Success Join");
					sendMsg(clientSockFd, "Success Join", BUFF_SIZE);
				} else {
					puts("Not Join");
				}
			} else {
				puts("Join Fail");
			}
		}
	}

	// after login
	while( (buffSize = read(clientSockFd, buff, BUFF_SIZE)) != 0) {
		int toFd;
		char cmd[FIELD_SIZE];
		char toName[FIELD_SIZE];
		char msg[BUFF_SIZE];

		p = strtok(buff, "|"); if(p == 0) continue;
		strcpy(cmd, p);
		p = strtok(NULL, "|"); if(p == 0) continue;
		strcpy(toName, p);
		p = strtok(NULL, "|"); if(p == 0) continue;
		strcpy(msg, p);

		toFd = getFindClientFd(toName, data.groupid);

		sprintf(sendBuff, "%s|%s|%s", cmd, data.name, msg);
		sendMsg(toFd, sendBuff, BUFF_SIZE);

		if(strcmp(cmd, "file") == 0) {
			p = strtok(msg, " ");
			p = strtok(NULL, " ");
			printf("file size: %d\n", atoi(p));
			fileMediation(atoi(p), toFd, clientSockFd);
		}
	}
}

int fileMediation(unsigned int fileSize, int toFd, int fromFd)
{
	unsigned int buffSize = 0;
	unsigned int totalSize = 0;
	char fileBuff[BUFF_SIZE];

	puts("fileMediation start");
	while( (buffSize = read(fromFd, fileBuff, BUFF_SIZE)) != 0) {
		write(toFd, fileBuff, buffSize);
		totalSize += buffSize;

		if(fileSize == totalSize) break;
	}
	puts("fileMediation end");

	return 0;
}

int checkParentsID(char* id)
{
	MYSQL* db;
	MYSQL_RES* res;
	char queryBuff[256];
	
	sprintf(queryBuff, "select * from userlist where id like '%s'", id);
	connectDB(&db, "localhost", "root", "jjssm", "shield");
	if( getQueryDataNum(&db, queryBuff, &res) == 0) {
		return FALSE;
	}

	closeDB(&db, &res);

	return TRUE;
}

int checkParentsPW(char* id, char* pw, UserInfo* data)
{
	MYSQL* db;
	MYSQL_RES* res;
	char queryBuff[256];
	
	sprintf(queryBuff, "select * from userlist where id like '%s'", id);
	connectDB(&db, "localhost", "root", "jjssm", "shield");
	if( getQueryDataNum(&db, queryBuff, &res ) == 0) {
		return FALSE;
	}

	getQueryDataRow(&res, data);
	if(strcmp(data->pw, pw) == 0) {
		return TRUE;
	}

	closeDB(&db, &res);

	return FALSE;
}

int joinParents(char* id, char* pw)
{
	int randGroupid;
	MYSQL* db;
	MYSQL_RES* res;
	char queryBuff[256];
	
	connectDB(&db, "localhost", "root", "jjssm", "shield");

	randGroupid = getRandGroupid(&db, &res);

	sprintf(queryBuff, 
	"insert into userlist(id, pw, name, perm, groupid, fd) values('%s', '%s', '부모', 'parents', %d, -1)", 
	id, pw, randGroupid);

	printf("%s\n", queryBuff);

	if(sendQuery(&db, queryBuff) == FALSE) {
		return FALSE;
	}

	closeDB(&db, &res);

	return TRUE;
}

int getRandGroupid(MYSQL** db, MYSQL_RES** res)
{
	int randid = 0;
	char queryBuff[256];

	while(randid == 0) {
		randid = (rand() % 10000) + (((rand() % 9)+1) * 10000);

		sprintf(queryBuff, "select * from userlist where groupid like %d", randid);

		if(getQueryDataNum(db, queryBuff, res) == 0) {
			break;
		}

		randid = 0;
	}

	return randid;
}

int checkChildLogin(char* name, char* groupid, UserInfo* data)
{
	MYSQL* db;
	MYSQL_RES* res;
	char queryBuff[256];
	
	//sprintf(queryBuff, "select * from userlist where groupid like %s", groupid);
	sprintf(queryBuff, "select * from userlist where name like '%s' and groupid like '%s'", name, groupid);
	printf("%s\n", queryBuff);
	
	connectDB(&db, "localhost", "root", "jjssm", "shield");
	if( getQueryDataNum(&db, queryBuff, &res ) == 0) {
		return FALSE;
	}

	getQueryDataRow(&res, data);

	closeDB(&db, &res);

	return TRUE;
}

int checkChildName(char* name, char* groupid)
{
	MYSQL* db;
	MYSQL_RES* res;
	char queryBuff[256];
	
	sprintf(queryBuff, "select * from userlist where name like '%s' and groupid like '%s'", name, groupid);
	connectDB(&db, "localhost", "root", "jjssm", "shield");
	if( getQueryDataNum(&db, queryBuff, &res) == 0) {
		return FALSE;
	}

	closeDB(&db, &res);

	return TRUE;
}

int joinChild(char* name, char* groupid)
{
	MYSQL* db;
	char queryBuff[256];
	
	connectDB(&db, "localhost", "root", "jjssm", "shield");

	sprintf(queryBuff, 
	"insert into userlist(id, pw, name, perm, groupid, fd) values('', '', '%s', 'child', %d, -1)", 
	name, atoi(groupid));

	printf("%s\n", queryBuff);

	if(sendQuery(&db, queryBuff) == FALSE) {
		return FALSE;
	}

	closeDB(&db, 0);

	return TRUE;
}


int sendMsg(int fd, char* msg, int msgSize)
{
	int a;
	
	if(fd == -1) return FALSE;

	a = write(fd, msg, msgSize);
	printf("send msg %s - msgSize %d\n", msg, a);

	return TRUE;
}

int getFindClientFd(char* name, int groupid)
{
	int fd = -1;
	int i;
	
	pthread_mutex_lock(&mutex);
	for(i=0; i<accessUserNum; i++) {
		if(accessUserArr[i].groupid == groupid && (strcmp(accessUserArr[i].name, name) == 0)) {
			fd = accessUserArr[i].fd;
			break;
		}
	}
	pthread_mutex_unlock(&mutex);

	return fd;
}








void testDisplay(UserInfo* data)
{
	printf("%10s %10s %10s %10s %10s %10s\n",
			"id", "pw", "name", "perm", "groupid", "fd");
	printf("%10s %10s %10s %10s %10d %10d\n", 
	data->id, data->pw, data->name, data->perm, data->groupid, data->fd);
}
