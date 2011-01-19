Name: Christopher Carlisle

Contents: makefile 	Used to compile the program
	  main.cpp 	Main portion of program
	  buffer.h 	Functions to interact with the buffer
	  stats.h 	Class to keep track of stats

Solution: For this program I first read in the command
 	  line arguments using a function. I load these
	  into a global struct and use them to initialize
	  an instance of my stats class. I then initialize
	  my buffer with the stats sent in via the cli.
	  The threads are then created for the number of
	  producers and the number of consumers. The threads
	  start running producing and consuming items. After the
	  main function has created all of the threads it sleeps
	  for an user specified amount of time and then after that
	  changes a global flag to stop the simulation from running.
	  The threads are joined back to the main program.
	  As they are joined the stats are stored and added into
	  the instance of the class. Once all threads have been
	  joined the stats are printed out and the program ends.
