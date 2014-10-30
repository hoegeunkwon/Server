#include "./../include/server.h"
#include "./../include/shieldsql.h"

/*
   int main()
   {
   MYSQL* db = 0;
   MYSQL_RES* res = 0;
   UserInfo data;

   printf("%p\n", db);
   connectDB(&db, "localhost", "root", "jjssm", "shield");
   printf("%p\n", db);
   printf("number: %llu\n", getQueryDataNum(&db, "select * from userlist", &res));

   display(&res, &data);

   return 0;
   }
 */

int main(int argc, char** argv)
{
	int serverSockFd;
	int clientSockFd;
	struct sockaddr_in clientAddr;
	unsigned int addrLen;
	char addrBuff[20];
	pthread_t tId;

	
	if(pthread_mutex_init(&mutex, 0)) {
		perror("mutex");
		exit(0);
	} else {
		puts("Mutex Setting");
	}

	srand(time(0)); // Rand Port를 생성하려고

	serverSockFd = createServer(SERVER_PORT);
	puts("Waiting Client.....");

	while(1) {
		addrLen = sizeof(clientAddr);
		clientSockFd = accept(serverSockFd, (struct sockaddr*)&clientAddr, &addrLen);

		if(clientSockFd == -1) {
			perror("accept");
		}

		inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, addrBuff, sizeof(addrBuff));
		printf("Connected IP: %s\n", addrBuff);

		pthread_mutex_lock(&mutex);
		accessUserArr[accessUserNum++].fd = clientSockFd;
		
		LOG(
			int i;
			for(i=0; i<accessUserNum; i++) {
				printf("%d ", accessUserArr[i].fd);
			}
			puts("");
		)

		pthread_mutex_unlock(&mutex);

		if(pthread_create(&tId, 0, connectClient, &clientSockFd) != 0) {
			perror("thread create");
		}
		//pthread_detach(tId);    // 독립적인 자원관리
	}

	close(serverSockFd);

	return 0;
}
