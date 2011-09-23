main: mainA mainB

mainA: lexA.yy.c
	gcc lexA.yy.c -o mainA

lexA.yy.c: proj2accc.l
	flex -o lexA.yy.c proj2accc.l

mainB: lexB.yy.c
	gcc lexB.yy.c -o mainB

lexB.yy.c: proj2bccc.l
	flex -o lexB.yy.c proj2bccc.l

clean:
	rm lexB.yy.c lexA.yy.c mainA mainB
