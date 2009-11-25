//Christopher Carlisle
#ifndef STATS_H
#define STATS_H

#include <stdlib.h>


//vars
int simulationTime; //holds simulation time
int maxThreadSleepTime; //holds max sleep time for threads
int numProducerThreads; //holds # of producer threads
int numConsumerThreads; //holds # of consumer threads
int sizeOfBuffer; //holds size of the buffer
int totalNumConsumed; //gets # of items consumed
int totalNumProduced; //gets # of items produced
int *totalThreadNumProduced; //holds # of items produced in array for each thread
int *totalThreadNumConsumed; //holds # of items consumed in array for each thread
int numTimesBufferFull; //total # of times the buffer was full
int numTimesBufferEmpty; //total # of times the buffer was empty
int numItemsRemaining; //total # of items that were left in the buffer

//functions prototypes
void print(); //does the output at the end
void Stats(int, int, int, int, int); //Constructor to set all the stuff we get from cli

void Stats(int _simTime, int _maxSleep, int _numProducerThreads, int _numConsumerThreads, int _bufferSize)
{
	//Vars
	simulationTime = _simTime; 
	maxThreadSleepTime = _maxSleep;
	numProducerThreads = _numProducerThreads;
	numConsumerThreads = _numConsumerThreads;
	sizeOfBuffer = _bufferSize;

	//Allocate memory
	totalThreadNumProduced = new int[_numProducerThreads];
	totalThreadNumConsumed = new int[_numConsumerThreads];

}

void print()
{

	printf( "PRODUCER / CONSUMER SIMULATION COMPLETE\n");
	printf( "=======================================\n");
	printf( "Simulation Time:      %d\n", simulationTime);
	printf( "Maximum Thread Sleep Time:      %d\n", maxThreadSleepTime);
	printf( "Number of Producer Threads:        %d\n", numProducerThreads);
	printf( "Number of Consumer Threads:        %d\n", numConsumerThreads );
	printf( "Size of Buffer:       %d\n", sizeOfBuffer );
	
	//Producer threads
	int totalProd = 0;
	int i;

	//-----sum up the amount of items produced by all threads
	for(i=0; i < numProducerThreads; i++)
		totalProd += totalThreadNumProduced[i];

	//-----display total found above
	printf( "Total Number of Items Produced:       %d\n", totalProd );

	//-----for each producer thread show # of items produced
	for(i=0; i < numProducerThreads; i++) 
	{
		printf("   Thread ");
		printf("%d:    ", (i+1) );
		printf("%d\n", totalThreadNumProduced[i]);
	}

	printf("\n"); 

	//Consumer threads
	int totalCons = 0;

	//------sum up the amount of items consumed by all threads
	for(i=0; i < numConsumerThreads; i++)
		totalCons += totalThreadNumConsumed[i];

	//------display total # of items consumed
	printf("Total Number of Items Consumed:  %d\n", totalCons );

	//-----for each consumer thread show # of items produced
	for(i=0; i < numConsumerThreads; i++)
	{
	       	printf( "   Thread " );
		printf("%d:    ", (i+numProducerThreads+1) );
	       	printf("%d", totalThreadNumConsumed[i] );
	}

	printf("\n"); 

	printf("Number Of Items Remaining in Buffer:    %d\n", numItemsRemaining);
	printf("Number Of Times Buffer Was Full:    %d\n", numTimesBufferFull);
	printf("Number Of Times Buffer Was Empty:    %d\n", numTimesBufferEmpty);

}
/*
=======================================
Simulation Time:			30
Maximum Thread Sleep Time:		3
Number of Producer Threads:		2
Number of Consumer Threads:		2
Size of Buffer:				5

Total Number of Items Produced:		50000
  Thread 1:				30000
  Thread 2:				20000

Total Number of Items Consumed:		60000	
  Thread 3:				40000
  Thread 4:				20000

Number Of Items Remaining in Buffer:    0
Number Of Times Buffer Was Full:	3
Number Of Times Buffer Was Empty:	4
		
*/
#endif
