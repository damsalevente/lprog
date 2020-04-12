#include "player.h"

#define MAX_PLAYER_NUM 30
/* 
List of functions : 
void updateGame(Game *g); 
void addMachine(Player *p , machineType machineID);
void createGame(Game *g);
void tradeResource(Player *p, Game *g,double amount);
void powerBill(Player * p ,Game * g);
void addResource(Player *p, double amount);
void createGame(Game *g);
void sellMachine(Player *p,int machineIndex);
void addPlayer(Player *p,int *size,int name);
void updateForPlayer(Game *g,Player *p);
void handleMessage(Player *p,int machineIndex,eventName whathappened);
void getInfo(Player *p, Game *g);
*/
/*
 Global init, this will be the case in the main program too 
 Game -> this holds all the envoriment information.
 players -> all clients 
 when i delete one, i set everything to -1 
*/
Game game;
Player players[MAX_PLAYER_NUM];
int playernum = 0;
int main()
{
    createGame(&game);
    printf("Test one : Check game default values:\n");
    printf("%f\n%f\n%f\n%d\n", game.difficulty,
           game.powerCost,
           game.conversionRate,
           game.totalMiners);
    printf("Test 2 : Check update:\n");
    for (int i = 0; i < 10; i++)
    {
        updateGame(&game);
        printf("Phase %d:\n", i);
        printf("%f\n%f\n%f\n%d\n", game.difficulty,
               game.powerCost,
               game.conversionRate,
               game.totalMiners);
    }
    printf("Test 3 : Player functions:\n");
    addPlayer(players,&playernum,1);
    addPlayer(players,&playernum,2);
    
}