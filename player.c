#include "player.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Add one new machine to the player 
void addMachine(Player *p , machineType machineID){
    if(p->numMachines==MAX_MACHINES){
        printf("Couldn't add more machines to him\n");
    }
    else{
        Machine machine;
        createMachineById(&machine,machineID);  //new machine added
        p->money-=machineID*1.734+500;          //money removed
        p->machines[p->numMachines++]=machine;
        if(p->money<0){
            printf("Broke\n");
        }
    }
}
void tradeResource(Player *p, Game *g,double amount){
    p->money+=amount * g->conversionRate;   //get the money
    p->resource-=amount;                    //remove the resource
}
void powerBill(Player * p ,Game * g){
    p->money -= p->powerConsumption*g->powerCost;
    if(p->money<0){
            printf("Broke\n");
        }
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
//need to check before add new player
void addPlayer(Player *p,int *size,int name)
{
    p[*size].money = 3000;
    p[*size].numMachines=0;
    p[*size].name = name;
    (*size)++;
    printf("newsize: %d",*size);
    
}
/* Called every X sec */
void updateForPlayer(Game *g,Player *p)
{
    for(int i=0;i<p->numMachines;i++)
    {
        if(p->machineStates[i]==Mining)
        {
            p->powerConsumption+=p->machines[i].powerConsumption;   //Every time they run, so we add amount/updatetime
        }
        if(p->machines[i].resourceMined != 0){
            p->resource+= p->machines[i].resourceMined;     // Could've written better but oh man
            p->machines[i].resourceMined = 0;
        }
    }
}
void handleMessage(Player *p,int machineIndex,eventName whathappened)
{
    elem e;
    e = StateMachine[p->machineStates[machineIndex]][whathappened];
    e.action(&p->machines[machineIndex]);
    p->machineStates[machineIndex]=e.nextState;
}
void sellMachine(Player *p,int machineIndex)
{
    p->money += p->machines[p->numMachines].type*1.28+2000;
    p->numMachines --;  //delete last
}
void getInfo(Player *p,Game *g){
    printf("Players machines\n");
    printf("Money:%lf,Id: %d,Power cons:%lf,Resource: %lf",p->money,p->name,p->powerConsumption,p->resource);
    for(int i = 0;i<p->numMachines;i++){
        printf("State : %d\n",p->machineStates[i]);
        printf("PARAMETERS\n");
        printf("Power :%lf\n",p->machines[i].power);
        printf("PowerConsumption :%lf\n",p->machines[i].powerConsumption);
        printf("Resource mined :%lf\n",p->machines[i].resourceMined);
        printf("ID :%d\n",p->machines[i].ID);
    }
}