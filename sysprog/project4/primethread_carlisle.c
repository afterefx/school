/*
 * Christopher Carlisle
 * COSC 4348
 * HW 4
 * 4/25/2011
 * ccarlisle1@islander.tamucc.edu
 * Katangur
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int threadNumber;
    int lBound;
    int uBound;
} bounds;

//function prototypes
void *prime(void *number);
void *sort();
int compare (const void * a, const void * b);

//globals
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;
FILE * fstream;
FILE * sorted;
int * list;

int main(int argc, char **argv)
{
    int rc1;
    pthread_t *currThread;
    bounds * cliBounds;
    int currentNumber;

    //open file
    fstream = fopen("results_carlisle.dat", "w+");
    if(fstream == NULL)
    {
        fprintf(stderr, "Cannot open data file!\n");
        exit(1);
    }


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
    //printf("lower: %d\n", lowerBound);
    //printf("upper: %d\n", upperBound);
    //printf("threads: %d\n", numThreads);
    //printf("numbers to check: %d\n", difference);
    //printf("distribute: %d\n", range);
    //printf("mod: %d\n", (difference%numThreads));

    //increment num of threads by one
    numThreads++;

    cliBounds = (bounds*) malloc((sizeof(bounds)*numThreads));

    //create array of threads
    pthread_t threads[numThreads];

    //balance load
    currentNumber=lowerBound;
    int i=0;
    int stoppingPoint = numThreads-1;
    for(i; i < stoppingPoint; i++)
    {
        cliBounds[i].threadNumber = i+1;
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

    //create each thread
    for(i=0; i < stoppingPoint; i++)
    {
        currThread = &threads[i];

        if( (rc1=pthread_create( currThread, NULL, &prime, &cliBounds[i])) )
            printf("Thread creation failed: %d\n", rc1);

        /* Wait till threads are complete before main continues. Unless we  */
        /* wait we run the risk of executing an exit which will terminate   */
        /* the process and all threads before the threads have completed.   */
    }

    //join each thread
    for(i=0; i < stoppingPoint; i++)
    {
        currThread = &threads[i];
        pthread_join( *currThread, NULL);
    }

    //sort the file
    if( (rc1=pthread_create( &threads[stoppingPoint], NULL, &sort, NULL)) )
        printf("Thread creation failed: %d\n", rc1);

    //join last created thread
    pthread_join( threads[stoppingPoint], NULL);


    //print results
    for(i=0; i < counter; i++)
    {
        printf("Prime %d = %d\n", (i+1), list[i]);
    }

    //dealloc
    free(cliBounds);

    //close files
    fclose(fstream);
    fclose(sorted);

    //exit
    exit(0);
}

void *prime(void *number)
{
    bounds *bob =  (bounds*)number;
    int lower=(*bob).lBound;
    int upper=(*bob).uBound;
    int amount = (upper-lower)+1;
    int boolPrime[amount];
    //if((*bob).threadNumber == 1)
        //sleep(6);
    //printf("==== Thread %d ====\n", (*bob).threadNumber);
    //printf("lower Number: %d\n",lower);
    //printf("upper Number: %d\n",upper);
    //printf("number of numbers: %d\n",amount);

    //for each number in range check to see if prime
    int x=lower;
    int y;
    for(x; x <= upper; x++)
    {
        //printf("loop number %d\n", ((x-lower)+1));
        y=2;
        while(y < x)
        {
            if(x%y ==0)
            {
                //printf("%d < %d && %d mod %d == 0\n", y,x,x,y);
                break;
            }
            else
            {
                //printf("%d < %d && %d mod %d !=0 \n", y,x,x,y);
                y++;
            }
        }

        //printf("checking result\n");
        if(y == x)
        {
            boolPrime[x-lower] = 1;
            pthread_mutex_lock( &mutex1 );
            fprintf(fstream,"%d\n", x);
            pthread_mutex_unlock( &mutex1 );
            counter++;
        }
        else
            boolPrime[x-lower] = 0;

    }

    pthread_exit(NULL);
}

void *sort()
{
    //int list[counter];
    list = (int*) malloc((sizeof(int)*counter));
    rewind(fstream);

    sorted = fopen("sorted_carlisle.dat", "w+");
    if(fstream == NULL)
    {
        fprintf(stderr, "Cannot open data file!\n");
        exit(1);
    }

    //read from file
    int i=0;
    while(!feof(fstream))
    {
        fscanf (fstream, "%d", &list[i]);
        i++;
    }

    qsort(list, counter, sizeof(int), compare);

    for(i=0; i < counter; i++)
    { fprintf(sorted, "%d\n", list[i]); }

}

int compare (const void * a, const void * b)
{ return ( *(int*)a - *(int*)b ); }
