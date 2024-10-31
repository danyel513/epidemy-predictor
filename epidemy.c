#include "epidemy.h"

// variable data
int MAX_X_COORD, MAX_Y_COORD; // maximal coordinates given on the first row of the file
int TOTAL_SIMULATION_TIME; // simulation duration -> 1st argv
char INPUT_FILE_NAME[100] = ""; // input file name -> 2nd argv
int THREAD_NUMBER; // number of threads used -> 3rd argv

// function: errorHandler() -> prints appropiate error message and stops the program
void errorHandler(void)
{
    perror(strerror(errno)); // print the right error message having the "errno" code
    exit(EXIT_FAILURE);
}

// function: checkArguments() -> checks if arguments respect conditions
void checkArguments(const int argc, char *argv[])
{
    if (argc != 4) // ./exe TOTAL_SIMULATION_TIME InputFileName ThreadNumber
    {
        printf("Invalid arguments! \n");
        exit(EXIT_FAILURE);
    }

    // read and check for correct arguments

    char *endptr;

    TOTAL_SIMULATION_TIME = (int) strtol(argv[1], &endptr, 10); // casting to integer
    if (*endptr != '\0' || TOTAL_SIMULATION_TIME <= 0)
    {
        printf("Invalid TOTAL SIMULATION TIME! \n");
        exit(EXIT_FAILURE);
    }

    THREAD_NUMBER = (int) strtol(argv[3], &endptr, 10); // casting to integer
    if (*endptr != '\0' || THREAD_NUMBER <= 0)
    {
        printf("Invalid THREADS NUMBER! \n");
        exit(EXIT_FAILURE);
    }

    strcpy(INPUT_FILE_NAME, argv[2]); // copy the name
    if(strcmp(INPUT_FILE_NAME, "") == 0)
    {
        printf("Invalid INPUT FILE NAME! \n");
        exit(EXIT_FAILURE);
    }

// debug purpose: test that the values were saved properly
#ifdef DEBUG
    printf("Saved values from arguments: %d %s %d \n", TOTAL_SIMULATION_TIME, INPUT_FILE_NAME, THREAD_NUMBER);
#endif
}

// function: readData() -> reads and saves the data from the input file
Person_t* readData(int *n) // n reperesents the size of the array, it needs to be saved in the parameter, initialy it can be 0 or NULL
{
    // open file - reading only
    FILE *inputFile = fopen(INPUT_FILE_NAME, "r");
    if (inputFile == NULL) errorHandler();

    // read data:

    if (fscanf(inputFile, "%d %d", &MAX_X_COORD, &MAX_Y_COORD) != 2) errorHandler();

    if (fscanf(inputFile, "%d", n) != 1) errorHandler();

// debug purpose only: see values MAX_X_COORD, MAX_Y_VALUE and the number of people in file
#ifdef DEBUG
    printf("Saved values from file: max_x = %d, max_y = %d, n = %d \n", MAX_X_COORD, MAX_Y_COORD, *n);
#endif

    Person_t* p = (Person_t*) malloc(sizeof(Person_t) * (*n));
    if (p == NULL)
    {
        printf("Memory allocation error! Couldn't alocate enough memory for the array. \n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < (*n); i++)
    {
        int aux1, aux2; // used it for the enum content

        if (fscanf(inputFile, "%ld %d %d %d %d %d", &p[i].personID, &p[i].x, &p[i].y, &aux1, &aux2, &p[i].amplitude) != 6)
            errorHandler();

        // setting other data
        p[i].infectionCounter = 0;
        p[i].currentStatus = (Status_t) aux1;
        p[i].movementDirection = (Direction_t) aux2;

        // checking the input data integrity
        if(p[i].x > MAX_X_COORD) p[i].x = MAX_X_COORD;
        if(p[i].y > MAX_Y_COORD) p[i].y = MAX_Y_COORD;
        if(p[i].currentStatus > 1 || p[i].currentStatus < 0) p[i].currentStatus = 0;
        if(p[i].movementDirection > 3 || p[i].movementDirection < 0) p[i].movementDirection = 0;
        if((p[i].movementDirection == 0 || p[i].movementDirection == 1 ) && p[i].amplitude > MAX_Y_COORD) p[i].amplitude = MAX_Y_COORD;
        if((p[i].movementDirection == 2 || p[i].movementDirection == 3 ) && p[i].amplitude > MAX_X_COORD) p[i].amplitude = MAX_X_COORD;

        // setting the decrementing variable -> duration
        p[i].time = p[i].currentStatus ? SUSCEPTIBLE_DURATION : INFECTED_DURATION;
    }

    // close file
    if(fclose(inputFile) == -1)
    {
        perror(strerror(errno));
    }

    // returning the chunk of memory - free in the main program
    return p;
}

// function: printPersonArray() -> prints the array of Person_t
void printPersonArray(Person_t* p, int n)
{
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("%ld - x: %d - y: %d - %s - %d - %d \n", p[i].personID, p[i].x, p[i].y,
               !p[i].currentStatus ? "infected" : (p[i].currentStatus == 1 ? "susceptible" : "immune"),
               p[i].movementDirection, p[i].amplitude);
    }
}

// function: movePerson() -> moves the person with "amplitude" size in their own movement direction
void movePerson(Person_t *p)
{
    switch (p->movementDirection) // judging by the moving direction and the current position we determine the next position
    {
        case N:
            if(p->y + p->amplitude > MAX_Y_COORD)
            {
                p->movementDirection = S;
                p->y -= p->amplitude;
            }
            else
            {
                p->y += p->amplitude;
            }
            break;

        case S:
            if(p->y - p->amplitude < 0)
            {
                p->movementDirection = N;
                p->y += p->amplitude;
            }
            else
            {
                p->y -= p->amplitude;
            }
            break;

        case E:
            if(p->x + p->amplitude > MAX_X_COORD)
            {
                p->movementDirection = W;
                p->x -= p->amplitude;
            }
            else
            {
                p->x += p->amplitude;
            }
            break;

        case W:
            if(p->x - p->amplitude < 0)
            {
                p->movementDirection = E;
                p->x += p->amplitude;
            }
            else
            {
                p->x -= p->amplitude;
            }
            break;
        default: break;
    }
}

// function: computeFutureStatus() -> defines the future status of the every person after they moved around
void computeFutureStatus(Person_t *p, const int n, const int index)
{
    // case 1: the person is either immune or infected and the interval of time is not over
    if ((p[index].currentStatus == INFECTED || p[index].currentStatus == IMMUNE) && p[index].time > SUSCEPTIBLE_DURATION)
    {
        p[index].time -= p[index].amplitude;
        p[index].futureStatus = p[index].currentStatus;
        return;
    }

    // case 2: the person was infected and is about to get cured (immune)
    if (p[index].currentStatus == INFECTED && p[index].time <= SUSCEPTIBLE_DURATION)
    {
        p[index].time = IMMUNE_DURATION;
        p[index].futureStatus = IMMUNE;
        return;
    }

    // case 3: the person is susceptible to contact the desease
    for(int i = 0; i < n; i++)
    {
        if(p[index].x == p[i].x && p[index].y == p[i].y && p[i].currentStatus == INFECTED)
        {
            p[index].futureStatus = INFECTED;
            p[index].time = INFECTED_DURATION;
            p[index].infectionCounter ++;
            return;
        }
    }

    // case 4: the perse\on did not get in contact with any infected people
    p[index].futureStatus = SUSCEPTIBLE;
    p[index].time = SUSCEPTIBLE_DURATION;

}

// function: updateStatus() -> passes from the current status to the future status every person
void updateStatus(Person_t *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        p[i].currentStatus = p[i].futureStatus;
    }
}

// function: writeData() -> prints the person array in the output file
void writeData(Person_t *p, int n, unsigned int type)
{
    // open the specified file type = 0 => serial, type != 0 => parallel
    FILE *f;
    if (type) f = fopen("f_serial_out.txt", "w");
    else f = fopen("f_parallel_out.txt", "w");
    if(f == NULL)
    {
        errorHandler();
        return;
    }

    // print array
    for(int i=0; i<n; i++)
    {
        fprintf(f, "Person %ld has final_x = %d, final_y = %d, final_status = %s and was infected %d times. \n",
            p[i].personID, p[i].x, p[i].y,
            !p[i].currentStatus ? "infected" : (p[i].currentStatus == 1 ? "susceptible" : "immune"),
            p[i].infectionCounter);
    }
}

// function: printStats() -> prints the measurements obtained in the output file
void printStats(double time, int nrPers) // uses the global TOTAL_SIMULATION_TIME, THREAD_COUNT
{
    FILE *f;
    if(THREAD_NUMBER == 1) // choose the right file to print the stats
    {
        f = fopen("performance_serial.txt", "a");
    }
    else
    {
        f = fopen("performance_parallel.txt", "a");
    }

    if(f == NULL)
    {
        errorHandler();
        return;
    }

    fprintf(f, "-----------/------------ \n");
    fprintf(f, "Total time: %f seconds\n", time);
    fprintf(f, "Total number of persons: %d\n", nrPers);
    fprintf(f, "Total time of simulation: %d \n", TOTAL_SIMULATION_TIME);
    fprintf(f, "Total number of threads: %d \n", THREAD_NUMBER);
    fprintf(f, "-----------/------------ \n\n");
}