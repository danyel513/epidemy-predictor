#ifndef EPIDEMY_H
#define EPIDEMY_H

// used libraries
#include<stdio.h>
#include<stdlib.h>

// predefined data
#define INFECTED_DURATION 2 // duration of the sickness
#define IMMUNE_DURATION 2 // after this the persone can get infected again

// variable data
int MAX_X_COORD, MAX_Y_COORD; // maximal coordinates given on the first row of the file
int TOTAL_SIMULATION_TIME; // simulation duration -> 1st argv
char InputFileName[]; // input file name -> 2nd argv
int ThreadNumber; // number of threads used -> 3rd argv

// define STATUS enum type
typedef enum Status
{
    INFECTED,
    SUSCEPTIBLE,
    IMUNE
} Status_t;

// define DIRECTION emun type
typedef enum Direction
{
    N,
    S,
    E,
    W
} Direction_t;

// define the PERSON structure
typedef struct Person
{
    long personID; // identification
    int x, y; // initial coordinates ---> 0 < x,y < maxX,maxY
    Status_t currentStatus; //  = initial health status -> can be 0 or 1
    Status_t futureStatus; // health status after epidemy "step"
    Direction_t movementDirection; // movement pattern
    int amplitude; // movement steps -> smaller than the size of the area
    int infectionCounter = 0;
} Person_t;

// serial functions

// parallel functions


#endif
