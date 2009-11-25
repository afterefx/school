//Christopher Carlisle
#ifndef _BUFFER_H_DEFINED_
#define _BUFFER_H_DEFINED_

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef int buffer_item;
#define bool int
#define false 0 
#define true 1

#define BUFFER_SIZE 5
buffer_item global_buffer[BUFFER_SIZE];
unsigned int bufferCount=0; //keeps track of number of items in buffer
unsigned int buffer_In_index=0; //index position for putting stuff in buffer
unsigned int buffer_Out_index=0; //index position for pulling stuff out of buffer

//function prototypes
bool buffer_insert_item( buffer_item item ); 
bool buffer_remove_item( buffer_item *item );
void buffer_print();
void buffer_initialize();

//Semphores and mutexs
sem_t bufferEmpty;
sem_t bufferFull;
//sem_init(&bufferEmpty, 0, BUFFER_SIZE ); //<---------- PROBLEM LINE
//sem_init( &bufferFull, 0, 0 ); //<-------------PROBLEM LINE
pthread_mutex_t mutex;
//pthread_mutex_init( &mutex, NULL ); //<-------------PROBLEM LINE


void buffer_initialize()
{
	int i;
	for(i=0 ; i < BUFFER_SIZE; i++)
	{
		global_buffer[i] = -1;  //sets all buffer positions to -1
	}
}

bool buffer_insert_item( buffer_item item )
{
	printf("Item: %d\n", item ); //debug print statement

	//put item into buffer
	global_buffer[buffer_In_index] = item; 

	//move buffer in index plus one in circular fashion
	buffer_In_index = (buffer_In_index + 1) % BUFFER_SIZE; 
	//cout << bufferCount << endl; //debug output buffercount 

	//increase buffer count since item was inserted
	bufferCount++; 

	return true;
}

bool buffer_remove_item( buffer_item *item )
{
	//Grab item from buffer
	*item = global_buffer[buffer_Out_index];

	//Move out to next index position in circular fashion
	buffer_Out_index = ( buffer_Out_index + 1) % BUFFER_SIZE;

	//Decrease number of items in buffer
	bufferCount--;

	return true;
}

void buffer_print()
{
	int i;
	//line 1
	printf("(buffers occupied: %d)\n", bufferCount);

	//line 2
	printf( "buffers: ");

	   
	for(i=0; i < BUFFER_SIZE; i++)
		printf( " %d ", (uint) global_buffer[i] ); //print #'s in buffer
	printf("\n"); //start a new line

	// Third line of output. puts dashes under numbers
	printf( "        ");
	for(i=0; i < BUFFER_SIZE; i++)
		printf( " ----");
	printf("\n");

	// Fourth line of output. Shows position of in & out indexes
	printf( "         ");
	for(i=0; i < BUFFER_SIZE; i++)
	{
		printf( " ");

		if(buffer_In_index == i || buffer_Out_index == i)
		{
			if( bufferCount == BUFFER_SIZE )
				printf( "WR");
			else if( bufferCount == 0 )
				printf( "RW");
			else if( buffer_In_index == i )
				printf( "W ");
			else
				printf( "R ");

			printf( "  ");
		}
		else
			printf( "    ");
	}
	printf("\n");


}

#endif // _BUFFER_H_DEFINED_

