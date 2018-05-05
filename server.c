#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "player.h"
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

#define PORT 4444
int flag = 0;
void timer_handler()
{
	printf("Happened\n");
	flag = 1;
}

int main()
{
	//Game init
	Game g;
	//Player
	Player players [1024];
	int playernum = 0;
	initStateMachine();
	//from config file
	createGame(&g);
	//Timer
	struct itimerval tval;
	timerclear(&tval.it_interval); /* zero interval means no reset of timer */
	timerclear(&tval.it_value);
	tval.it_value.tv_sec = 1;
	tval.it_interval.tv_sec = 1;
	//Handler setup
	signal(SIGALRM, timer_handler);
	//Start timer
	setitimer(ITIMER_REAL, &tval, NULL);
	//Socket
	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

	if (listen(sockfd, 10) == 0)
	{
		printf("[+]Listening....\n");
	}
	else
	{
		printf("[-]Error in binding.\n");
	}

	while (1)
	{
		newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
		if (newSocket < 0)
		{
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if ((childpid = fork()) == 0)
		{
			close(sockfd);

			while (1)
			{
				recv(newSocket, buffer, 1024, 0);
				printf("DEBUG: buffer:\n %s",buffer);
				if (strcmp(buffer, ":exit") == 0)
				{
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}
				else
				{
					if (strcmp(buffer, ":start") == 0)
					{
						printf("Creating player from %s:%d\n Name will be :%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port),ntohs(newAddr.sin_port));
						char * msg ="Player created";
						addPlayer(players,&playernum,ntohs(newAddr.sin_port));
						send(newSocket,msg,strlen(msg),0);
					}
					else{
						send(newSocket,buffer,strlen(buffer),0);
					}
					// end of game logic
					bzero(buffer, sizeof(buffer));
				}
			}
		}
	}

	close(newSocket);

	return 0;
}