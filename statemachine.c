#include <math.h>
#include "statemachine.h"
//actions
void actionRunMachine(Machine *machine)
{
    time (&machine->startTime);
}
void actionStopMachine (Machine *machine){
    time (&machine->finishTime);
    machine->resourceMined += difftime(machine->finishTime,machine->startTime)*machine->power;   //Power = amount/updatetime
    printf("Resource mined: %lf",machine->resourceMined);
}
void ok(Machine * machine)
{
    return;
}
//end of actions

void initStateMachine()
{
    StateMachine[Idle][Start].nextState = Mining;
    StateMachine[Idle][Start].action = actionRunMachine;

    StateMachine[Mining][Stop].nextState=Idle;
    StateMachine[Mining][Stop].action=actionStopMachine;

    StateMachine[Idle][Stop].nextState=Idle;
    StateMachine[Idle][Stop].action=ok;

    StateMachine[Mining][Start].nextState=Mining;
    StateMachine[Mining][Start].action=ok;
}
void createMachineById(Machine *curr,machineType type){
    if(type == Weak){
        curr->ID = 0;
        curr->type=type;
        curr->power=10;
        curr->powerConsumption=(double)1/60;
        curr->resourceMined=0;
        return;
        
    }
    if(type == Medium){
        curr->ID = 0;
        curr->type=type;
        curr->power=23;
        curr->powerConsumption=(double)10/60;   // /10minute
        curr->resourceMined=0;
        return;
    }
    if(type == Heavy){
        curr->ID = 0;
        curr->type=type;
        curr->power=23;
        curr->powerConsumption=1;
        curr->resourceMined=0;
        return;
    }
    else{
      printf("Couldn't find that type,sorry\n");
    }
}