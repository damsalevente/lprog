#include "player.h"
#include <math.h>
#include <stdio.h>
// Add one new machine to the player 
void addMachine(Player *p , int machineID){
    if(p->numMachines==MAX_MACHINES){
        printf("Couldn't add more machines to him\n");
    }
    else{
        Machine machine;
        createMachineById(&machine,machineID);  //new machine added
        p->money-=machineID*1.734+500;          //money removed
        p->machines[p->numMachines]=machine;
        p->numMachines++;
    }
}
void tradeResource(Player *p, Game *g,double amount){
    p->money+=amount * g->conversionRate;   //get the money
    p->resource-=amount;                    //remove the resource
}
void powerBill(Player * p ,Game * g,double amount){
    p->money -= amount*g->powerCost;
}
void addResource(Player *p, double amount){
    p->resource += amount;    //get the resource from all of the machines
}
void updateGame(Game *g){
    time_t mytime = time(0);
    struct tm * timeinfo  = localtime(&mytime);
    int ran = rand() % (55 + 1 -35) + 35;
    g->conversionRate += 0.25*g->conversionRate*sin(2*timeinfo->tm_sec);
    g->difficulty += ran/100;
    g->powerCost =g->powerCost+log(g->totalMiners+1);
}
void createGame(Game *g){
    g->conversionRate=1.25;
    g->difficulty=1;
    g->powerCost=1;
    g->totalMiners=0;
}