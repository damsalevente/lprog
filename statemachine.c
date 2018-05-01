#include <math.h>
#include "statemachine.h"
//actions
void actionRunMachine(Machine *machine)
{
    time (&machine->startTime);
}
void actionStopMachine (Machine *machine){
    time (&machine->finishTime);
    //machine->resourceMined += difftime(machine->finishTime,machine->startTime)*machine->power;   //Power = amount/seconds
}

//end of actions

void initStateMachine()
{
    StateMachine[Idle][Start].nextState = Mining;
    StateMachine[Idle][Start].action = actionRunMachine;

    StateMachine[Mining][Stop].nextState=Idle;
    StateMachine[Mining][Stop].action=actionStopMachine;

    StateMachine[Idle][Stop].nextState=Idle;
    StateMachine[Idle][Stop].action=NULL;

    StateMachine[Mining][Start].nextState=Mining;
    StateMachine[Mining][Start].action=NULL;
}
void createMachineById(Machine *curr,machineType type){
    if(type==0){
        curr->ID = uid;
        curr->type=type;
        curr->power=10;
        curr->powerConsumption=1;
        curr->resourceMined=0;
        
    }
    if(type == 1){
        curr->ID = uid;
        curr->type=type;
        curr->power=23;
        curr->powerConsumption=10;
        curr->resourceMined=0;
    }
    if(type == 2){
        curr->ID = uid;
        curr->type=type;
        curr->power=23;
        curr->powerConsumption=60;
        curr->resourceMined=0;
    }
    else{
        printf("There is no machine with that id\n");
    }
    uid++;
}