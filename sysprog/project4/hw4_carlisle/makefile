main: parallelfilecopy_carlisleprog progprimethread_carlisle

########################
# Prime numbers program
########################
progprimethread_carlisle: primethread_carlisle.c
	gcc primethread_carlisle.c -lpthread -o primethread_carlisle

########################
# Parallel file copy
########################
parallelfilecopy_carlisleprog: parallelfilecopy_carlisle.o copyfilepass.o restart.o
	gcc parallelfilecopy_carlisle.o copyfilepass.o restart.o -lpthread -o parallelfilecopy_carlisle

parallelfilecopy_carlisle.o: parallelfilecopy_carlisle.c
	gcc parallelfilecopy_carlisle.c -c -o parallelfilecopy_carlisle.o

copyfilepass.o: copyfilepass.c
	gcc copyfilepass.c -c -o copyfilepass.o

restart.o: restart.c restart.h
	gcc restart.c -c -o restart.o


########################
# Clean stuff up
########################
clean: primethread_carlisleclean paralellfilecopyclean resultsclean sortedclean

primethread_carlisleclean:
	/bin/rm primethread_carlisle

paralellfilecopyclean:
	/bin/rm parallelfilecopy_carlisle
	/bin/rm *.o

resultsclean:
	/bin/rm results_carlisle.dat

sortedclean:
	/bin/rm sorted_carlisle.dat

