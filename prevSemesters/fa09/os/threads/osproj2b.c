#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int *numbers;
void *sequence( void * ); // the thread

int main( int argc, char *argv[] )
{
  /* local variables */
  pthread_t tid; //the thread identifier
  pthread_attr_t attr; //set of thread attributes

  if(argc < 2)
  {
	  fprintf(stderr,"usage: a.out <integer value>\n");
	  return -1;
  }
  if(atoi(argv[1]) < 0)
  {
	  fprintf(stderr,"%d must be >= 0\n", atoi(argv[1]));
	  return -1;
  }

  //allocate memory for global pointer
  numbers = malloc(atoi(argv[1])*sizeof(int)); 

  //get the default attributes
  pthread_attr_init( &attr ); 
  //create the thread
  pthread_create( &tid, &attr, sequence, argv[1]); 
  //wait for the thread to exit
  pthread_join( tid, NULL ); 

  //print results
  printf("Fibonnaci sequence: ");

  printf("%d", numbers[0]); //print 1st one without comma at beginning

  int i=1;
  while(i < atoi(argv[1]))
  {
	  printf(", %d", numbers[i]); //print remaining values
	  i++;
  }

  /* return to calling environment */
  printf("\n");
  free(numbers);
  return 0;
}


void *sequence( void *param )
{

  int upper = atoi(param);

  //fib numbers
  numbers[0]=0;
  numbers[1]=1;

  int i=2;
  while(i < upper)
  {
	  numbers[i] = numbers[i-2]+numbers[i-1];
	  i++;
  }


  pthread_exit(0);
}

