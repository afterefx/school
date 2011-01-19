###########################################################
#
# Simple Makefile for Operating Systems Project 1
# project1
#
###########################################################
.SUFFIXES: .h .c .cpp .l .o

project1: project1.yy.o project1.o
	g++ project1.yy.o project1.o -o project1 -lfl

project1.o: project1.cpp
	g++ -c project1.cpp

project1.yy.o: project1.yy.c
	gcc -c project1.yy.c

project1.yy.c: project1.l
	flex -oproject1.yy.c project1.l

clean:
	/bin/rm -f *.o *~ core project1 project1.yy.c
