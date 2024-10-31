#include "epidemy.h"

// importing the extern variables used in the parallel version
Person_t *persons = NULL;
int numberOfPersons = 0;
pthread_barrier_t movingBarrier;

void start_simulation(Person_t *p, int n)
{
    // initialize barriers
    pthread_barrier_init(&movingBarrier, NULL, THREAD_NUMBER);

    // check if the number of persons can be diveded equally among the number of threads
    unsigned int rest_flag = 0;
    if(n % THREAD_NUMBER != 0)
        rest_flag = n % THREAD_NUMBER;

    // declared the thread handles
    pthread_t* thread_handles = malloc(THREAD_NUMBER * sizeof(pthread_t));
    int* tid = malloc(THREAD_NUMBER * sizeof(int));

    // start giving threads persons to move around
    for (int thread = 0; thread < THREAD_NUMBER; thread++)
    {
        tid[thread] = thread;
        pthread_create(&thread_handles[thread], NULL, movePersonsParallel, &tid[thread]);
    }
    // added a barrier to make sure that all the people moved around and we can compute the next status

    // after all the threads are done computing the persons moving procedure -> change the status
    for (int thread = 0; thread < THREAD_NUMBER; thread++)
    {
        tid[thread] = thread;
        pthread_create(&thread_handles[thread], NULL, movePersonsParallel, &tid[thread]);
    }

    // stop the threads
    for (int thread = 0; thread < THREAD_NUMBER; thread++)
        pthread_join(thread_handles[thread], NULL);

    // cleaning dynamic memory and destrying barriers
    pthread_barrier_destroy(&movingBarrier);
    free(thread_handles);
    free(tid);
}

int main(int argc, char **argv)
{
    // check the arguments passed to the program
    checkArguments(argc, argv);

    // read input data
    persons = readData(&numberOfPersons);

    // start the simulation
    start_simulation(persons, numberOfPersons);

    // free dynamic alocated memory
    free(persons);
    return 0;
}