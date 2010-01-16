# put this file in the directory with your source code
# to convert lab1.c into the executable lab1, type "make lab1"

XLIBS = -lX11 -lXext -lXmu -lXt -lXi 
GL_LIBS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lm $(XLIBS)
LIB_DEP = $(LIBDIR)/$(GL_LIB) $(LIBDIR)/$(GLU_LIB) $(LIBDIR)/$(GLUT_LIB)
CC = gcc

##### RULES #####

.SUFFIXES:
.SUFFIXES: .c

.c: $(LIB_DEP)
	$(CC) $(CFLAGS) $< $(GL_LIBS) -o $@
