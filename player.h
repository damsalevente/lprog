#define _USE_MATH_DEFINES // needed to have definition of M_LOG2E 

#include "statemachine.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define MAX_MACHINES 256

typedef struct Game{
    float difficulty;
    float powerCost;
    float conversionRate;
    int totalMiners;
}Game;

typedef struct Player{
    float money;
    double resource;
    double powerConsumption;
    char name [256];
    Machine machines [MAX_MACHINES];
    int numMachines;
}Player;
//for every machine, calculate the Price*consumption/elapsed time
/*
$hashTime = ((float) $difficulty) * (pow(2.0, 32) / ($hashRate * 1000.0)) ;
$powerCostPerYear = 365.25 * 24.0 * $powerConsumption / 1000.0 * $electricityRate;
$blocksPerYear =  (365.25 * 24.0 * 3600.0) / $hashTime ;
$coinsPerYear = $blockCoins * $blocksPerYear;
$revenuePerYear = $coinsPerYear * $conversionRate;
$profitPerYear = $revenuePerYear - $powerCostPerYear;
$netProfit1st = $revenuePerYear - $costHardware - $powerCostPerYear;
if ($profitPerYear <= 0) $breakEvenTime = -1;
else $breakEvenTime = $costHardware / ($profitPerYear / (365.25 * 24.0 * 3600.0));
*/
void calculateMoney(Game *g,Player *p);
/*
Difficulty : Monthly increases by : 35% to 55%
ConversionRate : Sine function
PowerCost :
*/
void updateGame(Game *g);
void addMachine(Player *p , int machineID);
void createGame(Game *g);
void tradeResource(Player *p, Game *g,double amount);
void powerBill(Player * p ,Game * g,double amount);
void addResource(Player *p, double amount);
void createGame(Game *g);

