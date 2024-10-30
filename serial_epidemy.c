#include "epidemy.h"

// starting simultation
void start_simulation(Person_t* personArray, int n)
{
    while(TOTAL_SIMULATION_TIME--)
    {
        // moving people around
        for(int i=0; i<n; i++)
        {
            movePerson(&personArray[i]);
        }

        // define their status - check for infections and immunity gaining
        for(int i=0; i<n; i++)
        {
            computeFutureStatus(personArray, n, i);
        }

        // update current status
        updateStatus(personArray, n);

        // for debug purpose only: check that people got infected
#ifdef DEBUG
        printPersonArray(personArray, n);
#endif
    }
}

int main(int argc, char *argv[])
{
    // check and save arguments
    checkArguments(argc, argv);

    // read person array from the file and save the max coords from file
    Person_t *personArray = NULL;
    int n;
    personArray = readData(&n);

// for debug purpose only: print the data read from file - check for
#ifdef DEBUG
    printPersonArray(personArray, n);
#endif

    start_simulation(personArray, n);

    writeData(personArray, n, 0);

    free(personArray);
    return 0;
}
