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
	int i;
	int clientSockFd = *((int*)arg);
	int destFd;
	char addrBuff[20];
	char buff[BUFF_SIZE];
	int buffSize;
	pthread_t tId;

// join user check
// if not join user 
// set user of db
// 중복검사
// else
// server memory upload

/*
	// setUserName
	while((buffSize = read(clientSockFd, buff, BUFF_SIZE))) {
		if(strlen(buff) >= 15) {
			continue;
		}

		if(strcmp(buff, "other") == 0) {
			printf("other: %s\n", buff);
			break;
		}

		pthread_mutex_lock(&mutex);
		for(i=0; i<clientInfoArrSize; i++) {
			if(clientInfoArr[i].sockFd == clientSockFd) {
				memcpy(clientInfoArr[i].name, buff, NAME_SIZE);
				break;
			}
		}
		pthread_mutex_unlock(&mutex);
		break;
	}

	while((buffSize = read(clientSockFd, buff, BUFF_SIZE))) {
		printf("socket Num: %d\t%s.\n", clientSockFd, buff);

		if(msgParsing(&msgInfo, buff) == -1) {
			puts("No Parsing");
			continue;
		}


		// 이부분에서 소속을 체크한다.
		if(strcmp(bottleName, msgInfo.company) == 0) {                      // 같은 소속.
			puts("Same");
			if( (destFd = getClientFd(msgInfo.to)) == 0) {
				puts("그런사람없음.");
			} else {
				if(msgInfo.file) {
					printf("파일이다.\n");
					FileMsg fileMsg;
					fileMsg.sockFd = clientSockFd;
					memcpy(&fileMsg.msg, &msgInfo, sizeof(Msg));

					if(pthread_create(&tId, 0, sameCompanyFile, &fileMsg) != 0) {
						perror("thread create");
					}
					pthread_detach(tId);    // 독립적인 자원관리
				} else {
					printf("send: %s\n", buff);
					sendMsg(destFd, 0, buff, buffSize);
				}
			}
		} else {                                                            // 다른 소속.
			puts("Not Same");
			if(getCompanyAddr(msgInfo.company, addrBuff) == -1) {
				puts("Not Other Bottle");
				continue;
			}
			printf("addr: %s.\n", addrBuff);
			if(sendMsg(0, addrBuff, buff, buffSize) == -1) {
				puts("메시지전송 실패");
			}
		}
	}

	pthread_mutex_lock(&mutex);
	for(i=0; i<clientInfoArrSize; i++) {
		if(clientInfoArr[i].sockFd == clientSockFd) {
			clientInfoArr[i] = clientInfoArr[clientInfoArrSize-1];
			clientInfoArrSize--;
			break;
		}
	}
	pthread_mutex_unlock(&mutex);

	close(clientSockFd);

	//printf("No.%d  Cnt = %d\n", clientSockFd, cnt);
	//printf("No.%d Client Out....\n", clientSockFd);
	printf("_____Total Client : ( %d )______\n", clientInfoArrSize);

	*/
	return 0;
}

