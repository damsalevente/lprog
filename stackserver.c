#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "player.h"
#include <sys/time.h>

void PANIC(char *msg);
#define PANIC(msg)   \
    {                \
        perror(msg); \
        exit(-1);    \
    }
/*--------------------------------------------------------------------*/
/*--- Global game variables                                        ---*/
/*--------------------------------------------------------------------*/
Game game;
Player players[5];
int playernum = 0;
int flag = 0;
int uid = 0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
/* Gameupdate handler */
void timer_handler()
{
    printf("Game update is happening.\n");
    updateGame(&game);
    for (int i = 0; i < playernum; i++)
    {
        printf("Update for player :%d \n", i);
        updateForPlayer(&game, &players[i]);
    }
    flag = 1;
}
/*--------------------------------------------------------------------*/
/*--- Child - echo servlet                                         ---*/
/*--------------------------------------------------------------------*/
void *Child(void *arg)
{
    char line[100];
    int bytes_read;
    int client = *(int *)arg;
    pthread_mutex_lock(&mutex1);

    int id = uid++;
    addPlayer(players, &playernum, id);
    printf("Creating player. Name will be :%d\n", id);
    pthread_mutex_unlock(&mutex1);

    while (1)
    {
        recv(client, line, 100, 0);
        printf("DEBUG: buffer:\n %s", line);
        if (strcmp(line, ":exit") == 0)
        {
            playernum--;
            printf("Disconnected\n");
            break;
        }
        else
        {
            if (strcmp(line, ":start") == 0)
            {
              
                send(client, line, strlen(line), 0);
            }
            if (strcmp(line, ":addmachine") == 0)
            {
                printf("Trying to add a a new machine to player with id:%d", id);
                pthread_mutex_lock(&mutex1);
                addMachine(&players[id], Weak);
                printf("Added machine %lf:", players[id].machines[players[id].numMachines-1].power);
                pthread_mutex_unlock(&mutex1);
            }
            if(strncmp(line,":start",6) == 0)
            {
                char *cmd;
                cmd = strtok(line," ");
                cmd = strtok(NULL," ");
                int machineNumber = atoi(cmd);
                handleMessage(&players[id],machineNumber,Start);
            }
            if(strncmp(line,":stop",5) == 0)
            {
                char *cmd;
                cmd = strtok(line," ");
                cmd = strtok(NULL," ");
                int machineNumber = atoi(cmd);
                handleMessage(&players[id],machineNumber,Stop);
            }
            if (strcmp(line, ":getinfo") == 0)
            {
                char msg[100];
                sprintf(msg, "Game info : %lf ; %lf;%lf;%d", game.difficulty, game.conversionRate, game.powerCost, game.totalMiners);
                getInfo(&players[id],&game);
                send(client, msg, strlen(msg), 0);
            }
            else
            {
                send(client, line, strlen(line), 0);
            }
            // end of game logic
            bzero(line, sizeof(line));
        }
    }
    close(client);
    return arg;
}

/*--------------------------------------------------------------------*/
/*--- main - setup server and await connections (no need to clean  ---*/
/*--- up after terminated children.                                ---*/
/*--------------------------------------------------------------------*/
int main(void)
{
    createGame(&game);
    initStateMachine();
    //Timer
    struct itimerval tval;
    timerclear(&tval.it_interval); /* zero interval means no reset of timer */
    timerclear(&tval.it_value);
    tval.it_value.tv_sec = 10;
    tval.it_interval.tv_sec = 10;
    //Handler setup
    signal(SIGALRM, timer_handler);
    //Start timer
    setitimer(ITIMER_REAL, &tval, NULL);
    int sd;
    struct sockaddr_in addr;

    if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        PANIC("Socket");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4444);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
        PANIC("Bind");
    if (listen(sd, 20) != 0)
        PANIC("Listen");
    while (1)
    {
        int client, addr_size = sizeof(addr);
        pthread_t child;

        client = accept(sd, (struct sockaddr *)&addr, &addr_size);
        printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        if (pthread_create(&child, NULL, Child, &client) != 0)
            perror("Thread creation");
        else
            pthread_detach(child); /* disassociate from parent */
    }
    return 0;
}
