#include "player.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#define MAX_PLAYERS 5

//Global variables for all threads
Player players[MAX_PLAYERS];
int playernum = 0;
Game game;
int update_f = 0;
void timer_handler(int sig)
{
    updateGame(&game);  // Game is read only
    update_f = 1;
    //printf("Happened\n");
}

int get_command(char massage[])
{
    if (strncmp(massage, "get", 3) == 0)
    {
        printf("the message is get\n");
        return 1;
    }
    else if (strncmp(massage, "sell", 4) == 0)
    {
        printf("the message is sell\n");

        return 2;
    }
    else if (strncmp(massage, "add", 3) == 0)
    {
        printf("the message is add\n");

        return 3;
    }
    else if (strncmp(massage, "trade", 5) == 0)
    {
        printf("the message is trade\n");

        return 4;
    }
    else if (strncmp(massage, "stop", 4) == 0)
    {
        printf("the message is stop\n");

        return 5;
    }
    else if (strncmp(massage, "start", 5) == 0)
    {
        printf("the message is start\n");

        return 6;
    }
    else
        return 0;
}
int main()
{
    elem e;
    Game g;
    initStateMachine();
    createGame(&g);
    Player p;
    addPlayer(&p,"Peter");
    addMachine(&p,Weak);
    players[playernum++]=p;
    getInfo(&players[0],&game);
    for (int i = 0; i < p.numMachines; i++)
    {
        printf("%lf\n", p.machines[i].power);
    }
    eventName eName = Start;
    //Timer
    struct itimerval tval;
    timerclear(&tval.it_interval); /* zero interval means no reset of timer */
    timerclear(&tval.it_value);
    tval.it_value.tv_sec = 1;
    tval.it_interval.tv_sec = 1;
    //Handler setup
    signal(SIGALRM, timer_handler);
    //Start timer

    while (1)
    {

        if(update_f)
        {
            for(int i=0 ; i<playernum;i++)
            {
                printf("Update for player :%d \n",i);
                updateForPlayer(&game,&players[i]);
            }
            update_f = 0;
        }
        //setitimer(ITIMER_REAL,&tval,NULL);  //újra felhúzzuk.
        char buffer[26];
        struct tm *tm_info;
        fgets(buffer, 26, stdin);
        int cmd = get_command(buffer);
        if (cmd == 1)
        {

            for (int i = 0; i < playernum; i++)
            {
                printf("Player number %d", i);
                getInfo(&players[i], &g);
            }
        }
        if (cmd == 2)
        {
            sellMachine(&players[0]);
        }
        if (cmd == 3)
        {
            getInfo(&players[0], &g);
        }
        if (cmd == 5)
        {
            handleMessage(&players[0], 0, Stop);
            printf("Resource mined: %lf\n",players[0].machines[0].resourceMined);

        }
        if (cmd == 6)
        {
            handleMessage(&players[0], 0, Start);
        }

    }
    return 0;
}