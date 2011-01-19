/********************************************************************
||Name:       Christopher Carlisle
||COURSE:     COSC 4328
||Lab #:      8
*******************************************************************/
#include <iostream>
using namespace std;
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//function prototype
void myDisplay();
void myReshape(int, int);
void main_menu(int);
void myinit();

//global vars
const int rows = 6;
const int cols = 6; 
int start[2] = {2,1};
int finish[2] = {4,4};
bool toggle=0;
char cstrDirection[15];
/**************
//up 	0
//right 1
//down 	2
//left 	3
**************/
int direction = 1; //base 0 and % value by 4

/*********************************
  0 no wall
  1 wall
  2 starting point (not yet implemented)
  3 finishing point (not yet implemented)
**********************************/
int map[rows][cols] =  {{1,1,1,1,1,1}, 
			{1,1,1,1,1,1},
		       	{1,0,0,0,0,1},
		       	{1,1,1,1,0,1},
		       	{1,1,1,1,0,1},
		       	{1,1,1,1,1,1}};

GLfloat light0_pos[]={1.8,3.5,0.5,1.0}; //closer top right

GLsizei wh = 500, ww = 500; // initial window size 
int winWidth=ww;
int winHeight=wh;

/* rehaping routine called whenever window is resized or moved */

/************************************************
|| myinit()
|| Purpose: setup the camera
************************************************/
void myinit()
{
	//show in terminal map
	for(int r=0; r < rows; r++)
	{
		for(int c=0; c < cols; c++)
			printf("%d,",map[r][c]); 
		printf("\n");
	}

	//setup projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity(); 
        glOrtho(0.0, rows, 0.0, cols, -4.0, 4.0);
	//gluPerspective( 45.0, 1.0, -3.0, 20.0 ); 	
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); 

	/* set clear color to black */ 
        glClearColor(0.0, 0.0, 0.0, 1.0);

	//try to rotate for first person camera
	//glTranslatef(4,0,0);
	//glRotatef(30.0, 0.0, 0.0, 1.0);
	glRotatef(40.0, 1.0, 0.0, 0.0);
}


/*
//void displayString(double x, double y, char *str)
//{
	//glColor3f(1.0,1.0,1.0); //set text color to white
    	//glDisable(GL_LIGHTING); //disable lighting
	//glPushMatrix(); //store comments on the side
	//glLoadIdentity(); //reset the matrix
	//glRasterPos3d(x,y,1.2); 
	//while (*str)
	//{
		//glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
		//str++;
	//}
    	//glEnable(GL_LIGHTING); //enable lighting
	//glPopMatrix();
//}*/

/************************************************
|| display()
|| Purpose: places cubes
************************************************/
void display()
{


	//move the light around
	light0_pos[0]=start[1]+.5;
	light0_pos[1]=(start[0] * -1)+5;
       	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos); //position light 0

	/*
	//print direction
	//switch(direction)
	//{
		//case 0: 
			//sprintf(cstrDirection, "Up");
			//break;
		//case 1: 
			//sprintf(cstrDirection, "Right");
			//break;
		//case 2: 
			//sprintf(cstrDirection, "Down");
			//break;
		//case 3: 
			//sprintf(cstrDirection, "Left");
			//break;
		//default:
			//sprintf(cstrDirection, "??");
			//break;
	//}
	//displayString(4.0,4.0,cstrDirection); //draw a string in the //viewport*/


	//maze
	glPushMatrix();
	glTranslatef(-0.5,5.5,0.0);
	glPushMatrix();
       	int r,c;

    	glClear(GL_COLOR_BUFFER_BIT);

    	//make map
    	glPushMatrix();
    	for(r=0; r < rows; r++)
    	{
		for(c=0; c < cols; c++)
		{
			glTranslatef(1.0,0.0,0.0);
			if(map[r][c] == 1)
				glutSolidCube(1);
		}
			glTranslatef(-6.0,-1.0,0.0);
    	}
    	glPopMatrix();

	//put starting cube
	glPushMatrix();
	for(r=0;r < start[0]; r++)
		glTranslatef(0.0,-1.0,0.0);
	for(c=0;c < start[1]+1; c++)
		glTranslatef(1.0,0.0,0.0);
	glutSolidCube(.5);
    	glPopMatrix();
    	glPopMatrix();

	//Get rid of initial push
    	glPopMatrix();
    	glutSwapBuffers();
}

/*********************************************
||myKey() function
||Purpose:  Q will close the window
|| 	    y rotate 
|| 	    u undo rotate 
|| 	    r go to beginning of maze
|| 	    t toggle the view of the maze
*********************************************/
void myKey(unsigned char key, int x, int y)
{
	if( key == 'Q' | key == 'q')
		exit(0); //Quit the program
	if( key == 'Y' | key == 'y') //rotate
	{
		glPushMatrix();
        	glRotatef(.5,0,1,0);
		glutPostRedisplay();
		
	}
	if( key == 'R' | key == 'r')//go back to beginning
	{
		start[0]=2;
		start[1]=1;
		glutPostRedisplay();
	}
	if( key == 'U' | key == 'u') //undo rotate
	{
		glPopMatrix();
		glutPostRedisplay();
	}
	if( key == 'T' | key == 't') //toggle view
	{
		//gluLookAt
		if(toggle)
		{
			glRotatef(-30.0, 1.0, 0.0, 0.0);
			//glTranslatef(0.0,0.0,-4.0);
			toggle != toggle;
		}
		else
		{
			glRotatef(30.0, 1.0, 0.0, 0.0);
			//glTranslatef(0.0,0.0,4.0);
			toggle != toggle;
		}
		glutPostRedisplay();
		
	}
}

/*********************************************
||enableLighting() function
||Purpose: Enables a light that can be moved
*********************************************/
void enableLighting()
{
    GLfloat diffuse0[]={1.0, 1.0, 1.0, 1.0}; //Light diffuse
    GLfloat ambient0[]={0.6, 0.6, 0.6, 1.0};
    GLfloat specular0[]={1.0,1.0,1.0,1.0};
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING); //enable lighting

    //Light 0
    glEnable(GL_LIGHT0); //enable light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0); //ambient light 0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0); //diffuse light 0
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0); //specular light 0
}

/*********************************************
||specialKeys() function
||Purpose: Allows you to use the arrow keys to move
|| 	   the cube through the maze
*********************************************/
void specialKeys(int key, int x, int y) 
{
		if(key == GLUT_KEY_UP) //move foward 
		{
			switch (direction)
			{
				case 0:
					//-1 from 0
					if(map[start[0]-1][start[1]] == 0)
						start[0] -= 1;
					break;
				case 1:
					//+1 to 1
					if(map[start[0]][start[1]+1] == 0)
						start[1] += 1;
					break;
				case 2: 
					//+1 to 0
					if(map[start[0]+1][start[1]] == 0)
						start[0] += 1;
					break;
				case 3:
					//-1 from 1
					if(map[start[0]][start[1]-1] == 0)
						start[1] -= 1;
					break;
				default:
					break;
			}

			glutPostRedisplay();
		}

		if(key ==  GLUT_KEY_DOWN) //move back
		{
			switch (direction)
			{
				case 0:
					//+1 to 0
					if(map[start[0]+1][start[1]] == 0)
						start[0] += 1;
					break;
				case 1:
					//-1 from 1
					if(map[start[0]][start[1]-1] == 0)
						start[1] -= 1;
					break;
				case 2: 
					//-1 from 0
					if(map[start[0]-1][start[1]] == 0)
						start[0] -= 1;
					break;
				case 3:
					//+1 to 1
					if(map[start[0]][start[1]+1] == 0)
						start[1] += 1;
					break;
				default:
					break;
			}

			glutPostRedisplay();
		}
		if(key == GLUT_KEY_RIGHT)
		{
			direction= (direction + 1) % 4;
			glutPostRedisplay();
		}
		if(key == GLUT_KEY_LEFT)
		{
			if(direction > 0)
				direction= (direction - 1) % 4;
			else
				direction = 3;
			glutPostRedisplay();
		}

} 

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Maze");
    myinit();
    glutDisplayFunc(display);
    enableLighting(); //setup lights
    glutKeyboardFunc(myKey); //Keyboard events
    glutSpecialFunc(specialKeys); //Special keyboard events
    glutMainLoop();
}
