# put this file in the directory with your source code
# to convert lab1.c into the executable lab1, type "make lab1"

XLIBS = -lX11 -lXext -lXmu -lXt -lXi 
GL_LIBS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lm $(XLIBS)
LIB_DEP = $(LIBDIR)/$(GL_LIB) $(LIBDIR)/$(GLU_LIB) $(LIBDIR)/$(GLUT_LIB)
CC = gcc

##### RULES #####

.SUFFIXES:
.SUFFIXES: .c

#.c: $(LIB_DEP)
#$(CC) $(CFLAGS) $< $(GL_LIBS) -o $@
main: skeleton Xfm $(LIB_DEP)
	gcc skeleton.o Xfm.o -o skeleton $(GL_LIBS)

skeleton: skeleton.c Xfm.h
	gcc skeleton.c -c -o skeleton.o

Xfm: Xfm.h Xfm.c
	gcc Xfm.c -c -o Xfm.o

clean:
	/bin/rm *.o skeleton
