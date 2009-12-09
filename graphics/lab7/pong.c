//Christopher Carlisle
#define MAX_NUM_PARTICLES 1
#define INITIAL_NUM_PARTICLES 1
#define INITIAL_SPEED 1.8

typedef int bool;
#define TRUE 1
#define FALSE 0

#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//function prototype
void myDisplay();
void timer(int);
void myReshape(int, int);
void main_menu(int);
void collision(int);
float forces(int, int);
void myinit();

//global vars
int num_particles; // number of particles 
GLfloat leftPaddlePOS=0.0;
GLfloat rightPaddlePOS=0.0;


// ======= particle struct ==========
typedef struct particle
{ 
     int color;
     float position[3];
     float velocity[3];
     float mass;
} particle;

particle particles[MAX_NUM_PARTICLES]; //particle system 

// initial state of particle system 
int present_time;
int last_time;
int num_particles = INITIAL_NUM_PARTICLES;
float speed = INITIAL_SPEED;
bool gravity = FALSE; // gravity off 
bool elastic = TRUE; // restitution off
bool repulsion = FALSE; // repulsion off 
float coef = 1.0; // perfectly elastic collisions 
float d2[MAX_NUM_PARTICLES][MAX_NUM_PARTICLES]; // array for interparticle distances 

GLsizei wh = 500, ww = 500; // initial window size 

GLfloat colors[8][3]={{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
    {1.0, 1.0, 1.0}};

/* rehaping routine called whenever window is resized or moved */

void myinit()
{
        int  i, j;
	
	/* adjust clipping box */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity(); 
        glOrtho(-2.0, 2.0, -2.0, 2.0, -4.0, 4.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); 
        gluLookAt(1.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0);


 	//set up particles with random locations and velocities 
        for(i=0; i<num_particles; i++) 
        {
            particles[i].mass = 1.0;
            particles[i].color = i%8; //circular rotation of colors

	    //random position and random velocity
            for(j=0; j<3; j++)
            {
                particles[i].position[j] = 2.0*((float) rand()/RAND_MAX)-1.0;
                particles[i].velocity[j] = speed*2.0*((float) rand()/RAND_MAX)-1.0;
            }
        }

	/* set clear color to grey */ 
        glClearColor(0.0, 0.0, 0.0, 1.0);
}

//timer functoin to perform animation
void timer(int foo)
{
    int i, j, k;
    float dt;
    present_time = glutGet(GLUT_ELAPSED_TIME);
    dt = 0.001*(present_time -  last_time);
    for(i=0; i<num_particles; i++) 
    {
       for(j=0; j<3; j++)
       {
           particles[i].position[j]+=dt*particles[i].velocity[j];    
           particles[i].velocity[j]+=dt*forces(i,j)/particles[i].mass;
       }
        collision(i);
    }
    if(repulsion) for(i=0;i<num_particles;i++) for(k=0;k<i;k++) 
    {
            d2[i][k] = 0.0;
            for(j=0;j<3;j++) d2[i][k]+= (particles[i].position[j]-
              particles[k].position[j])*(particles[i].position[j]-
              particles[k].position[j]);
            d2[k][i]=d2[i][k];
    }
    last_time = present_time;
    glutPostRedisplay();
    glutTimerFunc(50, timer, 1 );
    
}

/************************************************
|| forces()
|| Purpose: applies gravity and repulsion
************************************************/
float forces(int i, int j)
{
   int k;
   float force = 0.0;
   if(gravity&&j==1) force = -1.0; /* simple gravity */
   if(repulsion) for(k=0; k<num_particles; k++)  /* repulsive force */
   {
      if(k!=i) force+= 0.001*(particles[i].position[j]-particles[k].position[j])/(0.001+d2[i][k]);
   }
   return(force);
}

/************************************************
|| leftPadCollision()
|| Purpose: checks for collesion with left paddle
************************************************/
bool leftPadCollision()
{
	//variables
	GLfloat distance;
	GLfloat paddleTop;
	GLfloat paddleBottom;
	GLfloat cubePOS = particles[0].position[2];

	//Calculations
	distance = 1.0 - cubePOS;
	paddleTop = leftPaddlePOS + 0.2;
	paddleBottom = leftPaddlePOS - 0.2;

	//Decide what to do
	if(distance < 0.55 && particles[0].position[1] < paddleTop && particles[0].position[1] > paddleBottom && cubePOS < 1.0 && cubePOS > -1.0)
		return TRUE; //bounce
	return FALSE; //keep going
}

/************************************************
|| rightPadCollision()
|| Purpose: checks for collesion with right paddle
************************************************/
bool rightPadCollision()
{
	//variables
	GLfloat distance;
	GLfloat paddleTop;
	GLfloat paddleBottom;
	GLfloat cubePOS = particles[0].position[2];

	//Calculations
	distance = cubePOS + 1.0;
	paddleTop = rightPaddlePOS + 0.2;
	paddleBottom = rightPaddlePOS - 0.2;

	//Decide what to do
	if(distance < 0.55 && particles[0].position[1] < paddleTop && particles[0].position[1] > paddleBottom && cubePOS < 1.0 && cubePOS > -1.0)
		return TRUE; //bounce
	return FALSE; //keep going
}

/************************************************
|| collision()
|| Purpose: checks for collisions with walls & 
|| 	    paddles
************************************************/
void collision(int n)
{
/* tests for collisions against cube and reflect cubes if necessary */
     int i;

     for (i=0; i<3; i++) 
     {
	     //x&y
	     if(i != 2 ) 
	     { 
		     if(particles[n].position[i]>=0.95) 
		     {
			     particles[n].velocity[i] = -coef*particles[n].velocity[i];
			     particles[n].position[i] = 0.95-coef*(particles[n].position[i]-0.95);
		     }
		     if(particles[n].position[i]<=-0.95) 
		     {
			     particles[n].velocity[i] = -coef*particles[n].velocity[i];
			     particles[n].position[i] = -0.95-coef*(particles[n].position[i]+0.95);
		     }
	     }

	     //z
	     else
	     {
		     //paddle collision
		     if(leftPadCollision() || rightPadCollision())
		     {		     
			     if(particles[n].position[i]>=0.85) 
			     {
				     particles[n].velocity[i] = -coef*particles[n].velocity[i];
				     particles[n].position[i] = 0.85-coef*(particles[n].position[i]-0.85);
			     }
			     if(particles[n].position[i]<=-0.85) 
			     {
				     particles[n].velocity[i] = -coef*particles[n].velocity[i];
				     particles[n].position[i] = -0.85-coef*(particles[n].position[i]+0.85);
			     }
		     }
	     }
     }
}

/************************************************
|| display()
|| Purpose: places cubes
************************************************/
void display()
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT);

    //ball
    glColor3f(1.0,1.0,1.0); //change the color
    glPushMatrix();
    glTranslatef(particles[0].position[0],particles[0].position[1],particles[0].position[2]); //place a cube
    glutSolidCube(.1);
    glPopMatrix();

    //left paddle
    glPushMatrix();
    glTranslatef(0.0,leftPaddlePOS,1.0);
    glScalef(4, 0.8, 0.5);
    glutWireCube(.5);
    glPopMatrix();

    //right paddle
    glPushMatrix();
    glTranslatef(0.0,rightPaddlePOS,-1.0);
    glScalef(4, 0.8, 0.5);
    glutWireCube(.5);
    glPopMatrix();

    //outline
    glPushMatrix();
    glScalef(1, 1, 2.5);
    glutWireCube(2); 
    glPopMatrix();
    glutSwapBuffers();
}

/*********************************************
||myKey() function
||Purpose:  Q will close the window
|| 	    Spacebar will restart the game
|| 	    w,a can be used to move the left
|| 	        up and down
|| 	    r,t rotate the view of the game
*********************************************/
void myKey(unsigned char key, int x, int y)
{

	if( key == 'Q' | key == 'q')
		exit(0); //Quit the program

	if( key == 'R' | key == 'r')
	{
		glPushMatrix();
        	glRotatef(5,0,0,1);
		glutPostRedisplay();
		
	}
	if( key == 'T' | key == 't')
	{
		glPushMatrix();
        	glRotatef(5,0,1,0);
		glutPostRedisplay();
		
	}

	if( key == 'U' | key == 'u')
	{
		glPopMatrix(); //undo rotates
		glutPostRedisplay();
	}

	if( key == ' ' )
	{
		//reset game
		if( particles[0].position[2] > 1.0 || particles[0].position[2] < -1.0)
		{
			particles[0].position[1] = 0.0;
			particles[0].position[2] = 0.0;
			glutPostRedisplay();
		}

	}

	if(key == 'w' | key == 'W') //left paddle up
	{

		if(leftPaddlePOS < 0.8)
		{
			leftPaddlePOS += 0.1;
			glutPostRedisplay();
		}
	}

	if(key ==  's' | key == 'S') //left paddle down
	{
		if(leftPaddlePOS > -0.8)
		{
			leftPaddlePOS -= 0.1;
			glutPostRedisplay();
		}
	}


}

/*********************************************
||specialKeys() function
||Purpose: Allows you to use the arrow keys to move
|| 	   the right paddle up and down
*********************************************/
void specialKeys(int key, int x, int y) 
{
		if(key == GLUT_KEY_UP)
		{
			if(rightPaddlePOS < 0.8)
			{
				rightPaddlePOS += 0.1;
				glutPostRedisplay();
			}
		}

		if(key ==  GLUT_KEY_DOWN)
		{
			if(rightPaddlePOS > -0.8)
			{
				rightPaddlePOS -= 0.1;
				glutPostRedisplay();
			}
		}

} 

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Pong");
    myinit();
    glutDisplayFunc(display);
    glutTimerFunc(50, timer, 1 );
    glutKeyboardFunc(myKey); //Keyboard events
    glutSpecialFunc(specialKeys); //Special keyboard events
    glutMainLoop();
}
