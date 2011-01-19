#include <pthread.h>
#include <iostream>
#include <cstring>
using namespace std;

/****************************************************************
//Write a program that accepts a phrase of unspecified length on the command line. For example:

//prompt% vowcon Operating Systems Class at TAMUCC

//The main() in this program has to create two threads running functions (vow and cons). The threads should take turns printing the respective words of the phrase supplied on the command line. The vow thread should print all the words that start with a vowel and the cons thread should print all words starting with a consonant. Note that the main thread (running main()) should not print anything itself – the output should be supplied by the threads that it creates. The order of words in the phrase should not be changed in the printout. Your program should work for any phrase of any reasonable length, not just the one given in the example. The output of your program should look similar to the following

//prompt% vowcon Operating Systems Class at TAMUCC
//vow: Operating
//cons: Systems
//cons: Class
//vow: at
//cons: TAMUCC

//In this part you are *not allowed* to use synchronization primitives such as mutexes for thread coordination. You can use sched_yield() to relinquish control of the CPU. You will have to investigate some of the other pthread functions available to you for this project. Look for examples online.

//The name of this program must be osproj2c.c or osproj2c.cpp.
****************************************************************/



#define MAX_THREADS 512
char tolower(char);

void *sequence( void * );

int numSequence;
int hits[ MAX_THREADS ];


int main( int argc, char *argv[] )
{
  /* local variables */
  int retval;
  //pthread_t p_threads[MAX_THREADS];
  //pthread_attr_t attr;


  //fib numbers
  int n1=0, n2=1, result;

  /* initialize local variables */
  retval = 0;


  //pthread_attr_init( &attr );
  //pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );

  /* parse command line arguments into sample points and number of threads */
  numSequence = atoi(argv[1]);

  //pthread_create( &p_threads[ 12 ], &attr, sequence, (void *) &hits[12] );

  //pthread_join( p_threads[ 12 ], NULL );

  char *output;
  int count;
  int i=1;
  while(i < argc)
  {
	  output = argv[i];
	  if(tolower(output[0]) == 'a' || tolower(output[0]) == 'e' || tolower(output[0]) == 'i' || tolower(output[0]) == 'o' || tolower(output[0]) == 'u')
		  printf("vow: %s\n", output);
	  else
		  printf("cons: %s\n", output);
	  i++;
	  
  }

  /* return to calling environment */
  printf("\n");
  return( retval );
}


void *sequence( void *s )
{

  pthread_exit(0);
}

char tolower(char input)
{
	if(input >= 65 && input <= 90)
		input += 32;
	return input;
}
