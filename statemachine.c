#include <math.h>
#include <time.h>
typedef enum { Wait,
               Run,
               Reload,
               Write,
               Read } state;
typedef enum { Event1,
               Event2,
               Event3,
               Event4
} eventName;

typedef struct
{
    int ID;
    float power;
    float runningTime;
} Machine;
typedef struct elem
{
    state nextState;
    void (*action)(Machine *machine);
} elem;
//actions
void actionRunMachine(Machine *machine)
{
    machine->power += 10;
}
//end of actions
typedef elem StateMachine[Read + 1][Event4 + 1];
void initStateMachine(StateMachine *sm)
{
    sm[Wait][Event1]->nextState = Run;
    sm[Wait][Event1]->action = actionRunMachine;
}