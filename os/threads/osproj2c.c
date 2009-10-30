#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

//Function prototypes
char lowerLetter(char input);
void *vows( void *param );
void *cons( void *param );

//Global var
int argCount;
int wordNumber=1;

int main( int argc, char *argv[] )
{
  //set number of args
  argCount = argc-1;

  /* local variables */
  pthread_t tid1, tid2; //the thread identifier
  pthread_attr_t attr; //set of thread attributes


  //get the default attributes
  pthread_attr_init( &attr );
  //create thread 1
  pthread_create( &tid1, &attr, vows, (void *)argv);
  //create thread 2
  pthread_create( &tid2, &attr, cons, (void *)argv);

  //wait for threads
  pthread_join( tid1, NULL );
  pthread_join( tid2, NULL );

  /* return to calling environment */
  printf("\n");
  return 0;
}


void *vows( void *param )
{  
  char **output;
  
  while(wordNumber < argCount)
  {
	  output = (char**)param;
	  if(lowerLetter(output[wordNumber][0]) == 'a' || lowerLetter(output[wordNumber][0]) == 'e' || lowerLetter(output[wordNumber][0]) == 'i' || lowerLetter(output[wordNumber][0]) == 'o' || lowerLetter(output[wordNumber][0]) == 'u')
	  {
		  printf("vow: %s\n", output[wordNumber]);
		  wordNumber++;
	  }
	  else
		  sched_yield();

  }

  pthread_exit(0);
}

void *cons( void *param )
{  
  char **output;
  int i=1;
  
  while(wordNumber < argCount)
  {
	  output = (char**)param;
	  if(lowerLetter(output[wordNumber][0]) != 'a' && lowerLetter(output[wordNumber][0]) != 'e' && lowerLetter(output[wordNumber][0]) != 'i' && lowerLetter(output[wordNumber][0]) != 'o' && lowerLetter(output[wordNumber][0]) != 'u')
	  {
		  printf("cons: %s\n", output[wordNumber]);
		  wordNumber++;
	  }
	  else
		  sched_yield();

  }

  pthread_exit(0);
}


char lowerLetter(char input)
{
	if(input >= 65 && input <= 90)
		input += 32;
	return input;
}
