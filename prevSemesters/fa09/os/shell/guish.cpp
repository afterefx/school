//*********************************************************
//
// PUT YOUR NAME HERE!!!!
// Operating Systems
// Project #1: New Island Shell - Writing Your Own Shell
//
//*********************************************************


//*********************************************************
//
// Includes and Defines
//
//*********************************************************
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <cstdlib>
using namespace std;


void child(string , char ** );

//*********************************************************
//
// Extern Declarations
//
//*********************************************************
using namespace std;
extern "C"
{
  extern char **gettoks();
} 


//*********************************************************
//
// Main Function
//
//*********************************************************
int main( int argc, char *argv[] )
{
  /*// local variables
  int ii;
  */char **toks;/*
  int retval;

  // initialize local variables
  ii = 0;
  */toks = NULL;/*
  retval = 0;

  // main (infinite) loop
  while( true )
    {
      // get arguments
      toks = gettoks();

      if( toks[0] != NULL )
	{
	  // simple loop to echo all arguments
	  for( ii=0; toks[ii] != NULL; ii++ )
	    {
	      cout << "Argument " << ii << ": " << toks[ii] << endl;
	    }

	  if( !strcmp( toks[0], "exit" ))
	    break;
	}
    }*/
  pid_t pid;
  int stat;
  string command;

  while(true)
  {
  	cout << "guish: ";
  	cin >> command;
  	if(fork())
  		wait(&stat);
  	else
	{
		toks=gettoks();
  		child(command, toks);
	}
	cout << "Back" << endl;
  }

  // return to calling environment
  return 0;
}


void child(string command, char ** toks)
{
  char cwd[100];
  if(command != "hist")
  {
    getcwd(cwd,100);
    cout << "Command: " << command.c_str() << endl;
    cout << "Arguments: " << command.c_str() << endl;
    cout << "Toks: " << toks << endl;
    cout << "CWD: " << cwd << endl;
    cout << chdir(cwd) << endl;
    execvp(command.c_str(),toks);
	cout << "Returning" << endl;
  }
}

