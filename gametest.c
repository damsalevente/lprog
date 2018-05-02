#include "player.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#define MAX_PLAYERS 5

//Global variables for all threads
Player players[MAX_PLAYERS];
int playernum = 0;
Game game;

void timer_handler(int sig)
{

    for (int i = 0; i < playernum; i++)
    {
        updateForPlayer(&game, &players[i]);
        updateGame(&game);
        printf("Game updated");
    }

    printf("Happened\n");
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

    printf("Conversion rate before:%lf\n", g.conversionRate);
    printf("P money before init : %lf\n", p.money);
    addPlayer(&p, "Yolo janos");
    printf("%lf\n", p.powerConsumption);
    addMachine(&p, Weak);
    printf("ONe don\n");
    addMachine(&p, Weak);
    addMachine(&p, Heavy);
    players[0] = p;
    playernum++;
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
    //disable timer
    setitimer(ITIMER_REAL, &tval, NULL);

    while (1)
    {

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
            handleMessage(&players[0], 0, Start);
        }
        if (cmd == 6)
        {
            handleMessage(&players[0], 0, Stop);
        }

        return 0;
    }
}