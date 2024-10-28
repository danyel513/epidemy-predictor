#ifndef EPIDEMY_H
#define EPIDEMY_H

// defined debug purpose variables
#define DEBUG

// used libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// predefined data
#define INFECTED_DURATION 14 // duration of the sickness
#define IMMUNE_DURATION 30 // after this the persone can get infected again
#define SUSCEPTIBLE_DURATION 0 // signals that a person is predisposed to get infected

// variable data
extern int MAX_X_COORD, MAX_Y_COORD; // maximal coordinates given on the first row of the file
extern int TOTAL_SIMULATION_TIME; // simulation duration -> 1st argv
extern char INPUT_FILE_NAME[100]; // input file name -> 2nd argv
extern int THREAD_NUMBER; // number of threads used -> 3rd argv

// define STATUS enum type
typedef enum Status
{
    INFECTED,
    SUSCEPTIBLE,
    IMMUNE
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
    int infectionCounter; // how many times the person got infected over the simulation
    int time; // decremented every time the status does not change from imune or infected
} Person_t;

// serial functions

// parallel functions

// general use functions
Person_t* readData(int *n); // reads data from the file -> returns the Person array and the array size (as parameter)
void printPersonArray(Person_t* personArray, int numOfPersons); // prints array data
void checkArguments(int argc, char *argv[]); // checks and saves args
void errorHandler(void); // prints appropiate message for error
void movePerson(Person_t *p); // moves a person with one unit
void computeFutureStatus(Person_t *p, int n, int index); // finds the next status of every person status
void updateStatus(Person_t *p, int n);

#endif
