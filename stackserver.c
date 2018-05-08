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
void *UpdateThread()
{
    while (1)
    {
        printf("Game update started in thread\n");
        pthread_mutex_lock(&mutex1);
        updateGame(&game);
        game.totalMiners = 0;
        for (int i = 0; i < playernum; i++)
        {
            updateForPlayer(&game, &players[i]);
            game.totalMiners += players[i].numMachines;
        }
        pthread_mutex_unlock(&mutex1);
        printf("Game update finished\n");
        sleep(10);
    }
}

/* Find player in the array */
int findPlayer(int name)
{
    for (int i = 0; i < playernum; i++)
    {
        if (players[i].name == name)
        {
            return i;
        }
    }
    return -1;
}
/* Delete player from the array */
void removePlayer(int id)
{
    int position = findPlayer(id);
    if (position != -1)
    {
        for (int i = position - 1; i < playernum - 1; i++)
        {
            players[i] = players[i + 1];
        }
        playernum--;
    }
}
/* Gameupdate handler */
void timer_handler()
{
    printf("Game update started\n");
    pthread_mutex_lock(&mutex1);
    updateGame(&game);
    game.totalMiners = 0;
    for (int i = 0; i < playernum; i++)
    {
        updateForPlayer(&game, &players[i]);
        game.totalMiners += players[i].numMachines;
    }
    pthread_mutex_unlock(&mutex1);
}
/*--------------------------------------------------------------------*/
/*--- Child - echo servlet                                         ---*/
/*--------------------------------------------------------------------*/
void *Child(void *arg)
{
    char line[100];
    int bytes_read;
    int client = *(int *)arg;
    char s_buffer[100];
    pthread_mutex_lock(&mutex1);
    int id = uid++;
    addPlayer(players, &playernum, id);
    pthread_mutex_unlock(&mutex1);
    printf("Creating player. Name will be :%d\n", id);

    while (1)
    {
        s_buffer[0] = '\0';
        bytes_read = recv(client, line, sizeof(line), 0);
        line[bytes_read] = '\0';
        int idx = findPlayer(id);
        if (idx < 0)
        {
            PANIC("Player deleted\n");
        }
        if (strcmp(line, ":exit") == 0)
        {
            playernum--;
            printf("Disconnected\n");
            break;
        }
        else
        {
            if (strncmp(line, ":addmachine",11) == 0)
            {
                 char *cmd = strtok(line, ":");
                 int data = 0;
                cmd = strtok(NULL, ":");
                if(cmd)
                {
               data = atoi(cmd);
                if (data<3 && data >=0)
                {

                }
                else{
                    data = 0;
                }
                }
                printf("Trying to add a a new machine to player with id:%d", id);
                pthread_mutex_lock(&mutex1);
                addMachine(&players[idx], data);
                pthread_mutex_unlock(&mutex1);
                printf("Added machine %lf:", players[idx].machines[players[idx].numMachines - 1].power);
            }
            if (strncmp(line, ":start", 6) == 0)
            {
                char *cmd = strtok(line, ":");
                cmd = strtok(NULL, ":");
                if (cmd)
                {
                    int data = atoi(cmd);
                    if (data < players[idx].numMachines)
                    {
                        pthread_mutex_lock(&mutex1);
                        handleMessage(&players[idx], data, Start);
                        pthread_mutex_unlock(&mutex1);
                    }
                    else
                    {
                        printf("Failed to start machine: no index found\n");
                    }
                }
                else
                {
                    printf("Failed to start machine\n");
                }
            }
            if (strncmp(line, ":stop", 5) == 0)
            {
                char *cmd = strtok(line, ":");
                cmd = strtok(NULL, ":");
                if (cmd)
                {
                    int data = atoi(cmd);
                    if (data < players[idx].numMachines)
                    {
                        pthread_mutex_lock(&mutex1);
                        handleMessage(&players[idx], data, Stop);
                        pthread_mutex_unlock(&mutex1);
                        sprintf(s_buffer, "Machine %d stopped!", data);
                    }
                    else
                    {
                        sprintf(s_buffer, "Failed to stop machine: no index found");
                    }
                }
                else
                {
                    sprintf(s_buffer, "Failed to stop machine\n");
                }
            }
            if (strncmp(line, ":trade", 6) == 0)
            {
                char *cmd = strtok(line, ":");
                cmd = strtok(NULL, ":"); //Second chunk
                if (cmd)
                {
                    int amount = atoi(cmd);
                    if (amount < players[idx].resource)
                    {
                        pthread_mutex_lock(&mutex1);
                        tradeResource(&players[idx], &game, amount);
                        pthread_mutex_unlock(&mutex1);
                        sprintf(s_buffer, "Your money :%lf", players[idx].money);
                    }
                    else
                    {
                        /* The entered resource is not valid */
                        sprintf(s_buffer, "Invalid resource entered");
                    }
                }
            }
            if (strcmp(line, ":getinfo") == 0)
            {
                sprintf(s_buffer, "%lf;%lf;%lf;%d", game.difficulty, game.conversionRate, game.powerCost, game.totalMiners);
                pthread_mutex_lock(&mutex1);
                getInfo(&players[idx], &game);
                pthread_mutex_unlock(&mutex1);
                send(client, s_buffer, strlen(s_buffer), 0);
            }
            else
            {
                strcpy(s_buffer, line);
            }
            send(client, s_buffer, strlen(s_buffer), 0);
            memset(s_buffer, 0x00, 100);
        }
    }
    close(client);
    removePlayer(id);
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
    //setitimer(ITIMER_REAL, &tval, NULL);
    int sd;
    struct sockaddr_in addr;
    //updateThread
    pthread_t update_thread;

    if (pthread_create(&update_thread, NULL, &UpdateThread, 0) != 0)
        perror("Thread creation");
    else
        pthread_detach(update_thread);
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
