#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include "player.h"

#define PORT 4444
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
void *UpdateThread();
Game game_parameters;
int flag = 0;
int main()
{

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[512];
	int len = 0;
	char rec_buffer[512];
	pthread_t update_thread;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	if (pthread_create(&update_thread, NULL, &UpdateThread, &clientSocket) != 0)
		perror("Thread creation");
	else
		pthread_detach(update_thread);

	while (1)
	{
		if (flag == 1)
		{
			// UPDATE  game parameters
			send(clientSocket, ":getinfo", 9, 0);
			if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0)
			{
				printf("Error\n");
			}
			else
			{
				//puts(buffer);
				sscanf(buffer, "Difficulty:%f\nConversion rate:%f\n Powercost: %f\n Number of miners:%d", &game_parameters.difficulty, &game_parameters.conversionRate, &game_parameters.powerCost, &game_parameters.totalMiners);
			}
			flag = 0 ;
		}
		//pthread_mutex_lock(&mutex1);
		printf("Client: \t");
		gets(buffer);
		printf("you entered: %s\n", buffer);
		send(clientSocket, buffer, strlen(buffer), 0);
		if (strcmp(buffer, ":exit") == 0)
		{
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		len = recv(clientSocket, rec_buffer, sizeof(rec_buffer), 0);
		rec_buffer[len] = '\0';
		if (len < 0)
		{
			printf("[-]Error in receiving data.\n");
		}
		else
		{
			printf(" Difficulty:%2.2lf\n Conversion rate:%2.2lf\n Powercost: %2.2lf\n Number of miners: %d\n", game_parameters.difficulty, game_parameters.conversionRate, game_parameters.powerCost, game_parameters.totalMiners);
			printf("Server: %s\n", rec_buffer);
		}
		//pthread_mutex_unlock(&mutex1);
		memset(rec_buffer, 0, 512);

	}

	return 0;
}

void *UpdateThread(void *arg)
{
	//char gamebuf[1024];
	//int clientSocket = *(int *)arg;
	while (1)
	{
		/*pthread_mutex_lock(&mutex1);
		send(clientSocket, ":getinfo", 9, 0);
		if (recv(clientSocket, gamebuf, sizeof(gamebuf), 0) < 0)
		{
			printf("Error\n");
		}
		else
		{
			//puts(gamebuf);
			sscanf(gamebuf, "Difficulty:%f\nConversion rate:%f\n Powercost: %f\n Number of miners:%d", &game_parameters.difficulty, &game_parameters.conversionRate, &game_parameters.powerCost, &game_parameters.totalMiners);
		}
		pthread_mutex_unlock(&mutex1);*/
		flag = 1;	// it will triggers the main loop to perform a :getinfo 
		sleep(10);
	}
}