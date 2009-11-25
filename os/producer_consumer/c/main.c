//Christopher Carlisle
#include <stdlib.h>
#include "buffer.h"
#include "stats.h"
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

//Structs
typedef struct _ARGS //struct to hold args from command line
{
	int simLength;
	int maxThreadSleep;
	int numProducers;
	int numConsumers;
	bool outputBufferSnapshots;
}ARGS;

//Global args
ARGS global_args; //create an instance of arguments struct
bool runSim = true; //flag to indicate that the simluation should stop when false

//function prototype
void arguments(int, char  **);
bool checkPrime(buffer_item);
void *producer(void * param);
void *consumer(void * param);

int main( int argc, char *argv[] )
{
	int i;
	//Get command line arguments
	arguments(argc, argv);
	
	//Store simulation statistics
	Stats(global_args.simLength, global_args.maxThreadSleep,
			global_args.numProducers, global_args.numConsumers,
			BUFFER_SIZE);

	//Initialize the buffer
	buffer_initialize();

	//========= Threads =========
       	pthread_t tidP[global_args.numProducers]; //producer thread identifier array
       	pthread_t tidC[global_args.numConsumers]; //consumer thread identifier array
       	pthread_attr_t attr; //set of thread attributes
  	pthread_attr_init( &attr ); //get the default attributes
	
	printf("Starting Threads...\n");
	
	//Create producer thread(s)
	for(i=0; i < global_args.numProducers; i++)
	{
  		pthread_create( &tidP[i], &attr, producer, NULL);
	}

  	//Create consumer thread(s)
	for(i=0; i < global_args.numConsumers; i++)
	{
  		pthread_create( &tidC[i], &attr, consumer, NULL);
	}
	
  	//Sleep
	sleep(global_args.simLength);

	//Signal Threads to quit
	runSim = false;

	//Used to accept array from threads
	int *tempStats;

  	//Join Threads
	//cout << "Seg fault here may be caused by no mutex/semaphore\n"; //debug statement here
	for(i=0; i < global_args.numProducers; i++)
	{
		//Store stats for each thread
  		tempStats = (int*) pthread_join( tidP[i], NULL );

		totalThreadNumProduced[i] = tempStats[0];
		numTimesBufferFull += tempStats[1];

	}
	for(i=0; i < global_args.numConsumers; i++)
	{
		//Store stats for each thread
  		tempStats = (int*) pthread_join( tidC[i], NULL );

		totalThreadNumConsumed[i] = tempStats[0];
		numTimesBufferEmpty += tempStats[1];
	}
	
	//Grab number of items remaining in buffer
	numItemsRemaining = bufferCount;

  	//Display Statistics
	print();

  	//Exit
	return 0;
}

void arguments(int argc, char  *argv[])
{
	if(argc < 6)
	{
		cerr << "5 arguments are needed. The arguments should be in the following order:\n  simulation length\n  max thread sleep\n  number of producers\n  number of consumers\n yes or no for outputting buffer snapshots\n";
		exit(-1);
	}

	global_args.simLength = atoi(argv[1]);
	global_args.maxThreadSleep = atoi(argv[2]);
	global_args.numProducers = atoi(argv[3]);
	global_args.numConsumers = atoi(argv[4]);
	
	if(strcmp(argv[5],"yes") == 0)
		global_args.outputBufferSnapshots = 1;
	else if(strcmp(argv[5],"no") == 0)
		global_args.outputBufferSnapshots = 0;
	else
	{
		cerr << "The last argument must be 'yes' or 'no'\n";
		exit(-1);
	}


}

bool checkPrime(buffer_item in)
{
	int lim = in/2;
	int i;
	int result;

	for(i=2; i<=lim; i++)
	{
		result = in % i;
		if(result == 0)
			return false;
	}

	return true;

}

void *producer(void * param)
{
	//Variables
	int * stats = new int[2]; //used to return stats
	int time; //holds random number for time to sleep
	stats[0] = 0; //holds # of items produced
	stats[1] = 0; //holds # of times buffer is full
	buffer_item bItem; // this is my item

	while(runSim)
	{
		//Sleep for a random amount of time.
		time = rand();
		time = time % global_args.maxThreadSleep;

		sleep(time);

		//Generate a random number
		bItem = random();
		//bItem = bItem % 99;
		printf("Producer item: %d\n", bItem);

		//Check to see if buffer is full
		if(bufferCount == BUFFER_SIZE)
		{
		 printf("All buffers full.  Producer %d waits.\n",
				 pthread_self());
			stats[1]++;
		}

		//Wait
		sem_wait( &bufferEmpty );

		//Lock
		pthread_mutex_lock( &mutex );

		//Produce - CRITICAL SECTION
		if(runSim && buffer_insert_item(bItem))
		{
			printf("Producer %d writes %d\n", pthread_self(), bItem);

			stats[0]++; //add # of items produced

			if(global_args.outputBufferSnapshots) //if snapshots enabled
				buffer_print(); //print snapshots
		}

		//unlock
		pthread_mutex_unlock( &mutex );

		//signal
		sem_post( &bufferEmpty );

	}

	pthread_exit( (void*)stats ); //return stats array
}	

void *consumer(void * param)
{
	//Variables
	buffer_item bItem; //bItem to store received item
	int time; //int to hold random time to sleep
	int * stats = new int[2]; //array explain on next two lines
	stats[0] = 0; //holds # of items consumed
	stats[1] =0; //holds # of times buffer was empty

	while(runSim)
	{
		//Sleep for a random amount of time
		time = rand();
		time = time % global_args.maxThreadSleep;
		sleep(time);

		//Check if buffers are empty
		if(bufferCount == 0)
		{
			printf("All buffers empty.  Consumer %d waits.\n", pthread_self());
			stats[1]++; //add one time buffer was empty
		}

		//Check semaphore
		sem_wait( &bufferFull );

		//Lock
		pthread_mutex_lock( &mutex );

		//Consume
		if(runSim && buffer_remove_item( &bItem)) //consumes in if statement
		{
			//line 1
			printf("Consumer %d reads %d", pthread_self(), bItem); 

			stats[0]++; //increase # of items consumed

			if(checkPrime(bItem)) //find if # was prime
				printf("   * * * PRIME * * *");
			printf("\n");

			//if snapshots are enabled
			if(global_args.outputBufferSnapshots)
				buffer_print();
		}


		//Unlock
		pthread_mutex_unlock( &mutex );

		//Signal
		sem_post( &bufferFull );

	}

	pthread_exit( (void*)stats ); //return stats for this thread

}
