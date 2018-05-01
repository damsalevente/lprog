#include <math.h>
#include <time.h>
#include <stdio.h>

typedef enum { Idle,
               Mining
             } state;
typedef enum { Start,
               Stop
} eventName;
typedef enum machineType {
    Weak,
    Medium,
    Heavy
}machineType;
typedef struct
{
    machineType type;
    int ID; // machine id
    double power;    // amount/tick
    double powerConsumption;  // fogyaszt. (W)
    double resourceMined;
    time_t startTime;   //power * elapsedtime will give the amount of resource mined.
    time_t finishTime;
} Machine;
typedef struct event_{
    eventName event;
    time_t timeStamp;
}event_;
typedef struct eventNode{
    event_ val;
    struct eventNode *next;
}eventNode;
typedef struct elem
{
    state nextState;
    void (*action)(Machine *machine);
} elem;
elem StateMachine[Mining + 1][Stop + 1];

//actions
void actionRunMachine(Machine *machine);
void actionStopMachine(Machine *machine);
//end of actions
void initStateMachine();
// create an event
void createMachineById(Machine *curr,machineType type);