#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int threadNumber;
    int lBound;
    int uBound;
} bounds;

void *prime(void *number);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main(int argc, char **argv)
{
    int rc1;
    pthread_t *currThread;
    bounds * cliBounds;
    int currentNumber;

    //check for correct number of arguments
    if(argc != 4)
    {
        printf("There are not enough args on the cli\n");
        return -1;
    }

    //read in args
    int lowerBound = atoi(argv[1]);
    int upperBound = atoi(argv[2]);
    int numThreads = atoi(argv[3]);
    int difference = (upperBound-lowerBound)+1;
    int range = (difference/numThreads)-1;
    int balance = difference%numThreads;

    if(lowerBound >= upperBound)
    {
        printf("The upper bound should be greater than the lower bound.\n");
        return -1;
    }

    if(numThreads < 1)
    {
        printf("The number of threads must be greater than or equal to one.\n");
        return -1;
    }

    //debug
    printf("lower: %d\n", lowerBound);
    printf("upper: %d\n", upperBound);
    printf("threads: %d\n", numThreads);
    printf("numbers to check: %d\n", difference);
    printf("distribute: %d\n", range);
    printf("mod: %d\n", (difference%numThreads));

    //increment num of threads by one
    numThreads++;

    cliBounds = (bounds*) malloc((sizeof(bounds)*numThreads));

    //create array of threads
    pthread_t threads[numThreads];

    //split the load evenly
    //get mod and the number that mod receives is the number of threads that
    //will receive one extra number to process

    /* Create independent threads each of which will execute functionC */
    currentNumber=lowerBound;
    int i=0;
    for(i; i < numThreads-1; i++)
    {
        currThread = &threads[i];
        cliBounds[i].threadNumber = i;
        cliBounds[i].lBound = currentNumber;
        cliBounds[i].uBound = currentNumber+range;
        currentNumber+=range;
        if (currentNumber > upperBound)
        {
            currentNumber = upperBound;
            cliBounds[i].uBound = currentNumber;
        }

        if(balance != 0)
        {
            cliBounds[i].uBound++;
            balance--;
            currentNumber++;
        }

        currentNumber++;
    }

    i=0;
    for(i; i < numThreads-1; i++)
    {

        if( (rc1=pthread_create( currThread, NULL, &prime, &cliBounds[i])) )
        {
            printf("Thread creation failed: %d\n", rc1);
        }

        /* Wait till threads are complete before main continues. Unless we  */
        /* wait we run the risk of executing an exit which will terminate   */
        /* the process and all threads before the threads have completed.   */
        pthread_join( *currThread, NULL);
    }

    exit(0);
}

void *prime(void *number)
{
    bounds *bob =  (bounds*)number;
    int lower=(*bob).lBound;
    int upper=(*bob).uBound;
    int amount = (upper-lower)+1;
    int boolPrime[amount];
    counter++;
    printf("==== Thread %d ====\n", (*bob).threadNumber);
    printf("lower Number: %d\n",lower);
    printf("upper Number: %d\n",upper);
    printf("number of numbers: %d\n",amount);

    //for each number in range check to see if prime
    int x=lower;
    int y;
    for(x; x <= upper; x++)
    {
        y=2;
        //printf("y=%d and x=%d\n",y,x);
    pthread_mutex_lock( &mutex1 );
        while(y < x)
        {
            //printf("%d < %d && %d mod %d !=0 \n", y,x,x,y);
            if(x%y ==0)
                break;
            else
                y++;
        }
    pthread_mutex_unlock( &mutex1 );

        //printf("checking result\n");
        if(y == x)
        {
            boolPrime[x] = 1;
            printf("%d is prime\n", x);
        }
        else
        {
            boolPrime[x] = 0;
            printf("%d is NOT prime\n", x);
        }

    }

    pthread_exit(NULL);
}

