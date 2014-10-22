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

// if not join user 
// set user of db
// 중복검사
// else
// server memory upload

	

	return 0;
}

void disconnectClient(int clientSockFd)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i=0; i<accessUserNum; i++) {
		if(accessUserArr[i].fd == clientSockFd) {
			accessUserArr[i] = accessUserArr[accessUserNum-1];
			accessUserNum--;
			break;
		}
	}
	printf("DisConnected Now AccessUserNum : %d\n", accessUserNum);
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
	UserInfo data;

	puts("parents");
	// join user check
	while(buffSize = read(clientSockFd, buff, BUFF_SIZE)) {
		char tmp[FIELD_SIZE];
		char id[FIELD_SIZE];
		char pw[FIELD_SIZE];


		//printf("%s.\n", buff);

		p = strtok(buff, " "); if(p == 0) continue;
		strcpy(tmp, p);
		p = strtok(NULL, " "); if(p == 0) continue;
		strcpy(id, p);
		p = strtok(NULL, " "); if(p == 0) continue;
		strcpy(pw, p);


		printf("%s %s %s\n", tmp, id, pw);

		if(strcmp(tmp, "login") == 0) {		// login msg
			if(checkUserPW(id, pw, &data) == TRUE) {
				puts("Login Success");
				data.fd = clientSockFd; 
				//testDisplay(&data);
				insertClientData(clientSockFd, &data);
				break;
			} else {
				puts("Login Fail");
			}
		}
		
		printf("%s %s %s\n", tmp, id, pw);
		if(strcmp(tmp, "join") == 0) {	// join msg
			puts("in join");

			if(checkUserID(id) == FALSE) {	// possible join
				puts("possible join");
				if(joinParents(id, pw) == TRUE) {
					puts("Success Join");
				} else {
					puts("not Join");
				}
			}
		}
	}

	while(buffSize = read(clientSockFd, buff, BUFF_SIZE)) {

	}

}

void childClient(int clientSockFd, char* buff)
{
	puts("child");

}

int checkUserID(char* id)
{
	MYSQL* db;
	MYSQL_RES* res;
	char queryBuff[50];
	
	sprintf(queryBuff, "select * from userlist where id like '%s'", id);
	connectDB(&db, "localhost", "root", "jjssm", "shield");
	if( getQueryDataNum(&db, queryBuff, &res) == 0) {
		return FALSE;
	}

	closeDB(&db, &res);

	return TRUE;
}

int checkUserPW(char* id, char* pw, UserInfo* data)
{
	int i;
	MYSQL* db;
	MYSQL_RES* res;
	char queryBuff[50];
	
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
	MYSQL* db;
	char queryBuff[128];

	puts("joinParents fun");
	
	sprintf(queryBuff, 
	"insert into userlist(id, pw, name, perm, groupid, fd) values('%s', '%s', '부모', 'parents', 54321, -1)", id, pw);
	printf("%s\n", queryBuff);

	connectDB(&db, "localhost", "root", "jjssm", "shield");
	if(sendQuery(&db, queryBuff) == FALSE) {
		return FALSE;
	}

	closeDB(&db, 0);

	return TRUE;
}









void testDisplay(UserInfo* data)
{
	printf("%10s %10s %10s %10s %10s %10s\n",
			"id", "pw", "name", "perm", "groupid", "fd");
	printf("%10s %10s %10s %10s %10d %10d\n", 
	data->id, data->pw, data->name, data->perm, data->groupid, data->fd);
}
