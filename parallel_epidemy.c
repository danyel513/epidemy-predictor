#include "epidemy.h"

void start_parallel_simulation(void)
{
    // initialize barriers
    if (pthread_barrier_init(&movingBarrier, NULL, THREAD_NUMBER) != 0
        || pthread_barrier_init(&statusBarrier, NULL, THREAD_NUMBER) != 0)
    {
        errorHandler();
    }

    // declared the thread handles
    pthread_t* thread_handles = (pthread_t*) malloc(THREAD_NUMBER * sizeof(pthread_t));
    int *tid = (int*) malloc(THREAD_NUMBER * sizeof(int));

    int time = TOTAL_SIMULATION_TIME;
    while (time--)
    {
        // start giving threads tasks to accomplish
        for (int thread = 0; thread < THREAD_NUMBER; thread++)
        {
            tid[thread] = thread;
            pthread_create(&thread_handles[thread], NULL, threadTask, &tid[thread]);
        }

        // stop the threads
        for (int thread = 0; thread < THREAD_NUMBER; thread++)
            pthread_join(thread_handles[thread], NULL);

// debug purpose only: check the data after every iteration
#ifdef DEBUG
        printPersonArray(personsArr, numberOfPersons);
#endif
    }

    // cleaning dynamic memory and destrying barriers
    if (pthread_barrier_destroy(&movingBarrier) != 0
        || pthread_barrier_destroy(&statusBarrier) != 0)
    {
        errorHandler();
    }
    free(thread_handles);
    free(tid);

}

int main(int argc, char **argv)
{
    // check the arguments passed to the program
    checkArguments(argc, argv);

    // read input data - this time we initialize the data in an extern mode, without using the actual adress
    personsArr = readData(&numberOfPersons);

// debug purpose only: check the data read from the file
#ifdef DEBUG

    printPersonArray(personsArr, numberOfPersons);

#endif

// measure the runtime of the parallel algorithm
#ifdef PARALLEL_MEASUREMENTS

    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

#endif

    start_parallel_simulation();

#ifdef PARALLEL_MEASUREMENTS

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (double) (finish.tv_sec - start.tv_sec);
    elapsed += (double) (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printStats(elapsed, numberOfPersons);

#endif

    // print data in the file
    writeData(personsArr, numberOfPersons, 1);

    // free the dynamic alocated memory
    free(personsArr);
    return 0;
}