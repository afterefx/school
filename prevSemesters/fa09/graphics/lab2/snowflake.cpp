/********************************************************************
||Name:       Christopher Carlisle
||COURSE:     COSC 4328
||Lab #:     2 
||OBJECTIVE:   Draw a Koch Snowflake and make a second one interactive
|| 		plus a few more items
********************************************************************/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <cstdio>

struct iPOINT
{
	GLint x,y;
};

GLint n, 		//Holds number of iterations
    orig;	//Backup of original number of iterations passed in

bool drawSecondSnowflake=0; //false
iPOINT pTracker, pPassiveMouseLocation, 
       //pScreenSize, //
       pDragMouseStart; //where the mouse starts dragging from
const GLfloat pie = 3.141592653589793;

/* initial triangle */
GLfloat v[3][2]={{-0.5, -0.29}, {0.5, -0.29}, {0.0, 0.575}},//first snowflake
	w[3][2]={{-0.5, -0.29}, {0.5, -0.29}, {0.0, 0.575}};//second snowflake

/*********************************************
||moveSnowflakes() function
||Purpose: Move snowflake around the screen
||precondition: Current location of snowflake 
||postcondition: Snowflake is moved based on arguments sent in
*********************************************/
void moveSnowflakes(int horiz, int vert)
{
	int u;
	for(u=0;u<3;u++) //for each x of each snowflake
	{
		v[u][0] += (horiz/10.0); //move by .1 on the x axis
		w[u][0] += (horiz/-5.0); //move by -0.2 on the x axis
	}
	for(u=0;u<3;u++) //for each y of each snowflake
	{
		v[u][1] += (vert/10.0); //move by .1 on the y axis
		w[u][1] += (vert/-5.0); //move by -0.2 on the y axis
	}
}

/*********************************************
||drawLine() function
||Purpose: Receive 2 points and draw a line
*********************************************/
void drawLine( GLfloat *a, GLfloat *b )
{
       glVertex2fv(a);
       glVertex2fv(b);
}

/*********************************************
||degreesToRadians() function
||Purpose: Convert degrees into radians
*********************************************/
double degreesToRadians(int degrees)
{
	return (degrees * (pie / 180.0));
}

/*********************************************
||length() function
||Purpose: Return the length of a line segment between 2 points
*********************************************/
double length(GLfloat *a, GLfloat * b)
{
	GLfloat tempValueX, tempValueY, //temporary variables
		lengthOfSide; //hold the lenght of the current line

	//Length of L
	tempValueX = (a[0] - b[0]); //a[x]-b[x]
	tempValueY = (a[1] - b[1]); //a[y]-b[y]

	//Found abs() was not needed since numbers squared will be postive
	//anyway
	lengthOfSide = (tempValueX * tempValueX)+ (tempValueY * tempValueY);
	lengthOfSide = sqrt(lengthOfSide);

	return lengthOfSide;
}

/*********************************************
||divide_line() function
||Purpose: A line segment is sent in with the 
||	  number of degrees it is off the 
||	  plane and how many iterations are left
||precondition: A line segment is sent in
||postcondition: The line is split into 4 line 
|| 		 segments and each line is sent
|| 		 recursively if there are iterations
|| 		 left to execute.
*********************************************/
void divide_line(GLfloat *a, GLfloat *b, int degrees, int m)
{
	//Declarations
	double lengthOfSide, //holds the length of line (a,b)
		angle1, // holds the radians of -60
		angle2, // holds the radians of +60
		radians; //holds the radians for the degrees sent in
	GLfloat	r[2], s[2], t[2]; //new points to divide current line into 4


	if(m > 0)
	{
		//Find the length of the line
		lengthOfSide = length(a, b);

		//Convert degrees to radians
		radians = degreesToRadians(degrees); //original angle of line
		angle1 = degreesToRadians(-60+degrees); //turn to the right
		angle2 = degreesToRadians(60+degrees); //turn to the left

		//Math to create new points
		r[0] = a[0] + ((lengthOfSide/3.0) * cos(radians)); //find new x
		r[1] = a[1] + ((lengthOfSide/3.0) * sin(radians)); //find new y
		s[0] = r[0] + ((lengthOfSide/3.0) * cos(angle1)); //find new x
		s[1] = r[1] + ((lengthOfSide/3.0) * sin(angle1)); //find new y	
		t[0] = s[0] + ((lengthOfSide/3.0) * cos(angle2)); //find new x
		t[1] = s[1] + ((lengthOfSide/3.0) * sin(angle2)); //find new y	
	

		//Next iteration
		glColor3f(1.0,0.0,0.0); //red line
		divide_line(a, r, degrees, m-1);
		glColor3f(0.0,1.0,0.0); //green line
		divide_line(r, s, degrees - 60, m-1);
		glColor3f(0.0,0.0,1.0); //blue line
		divide_line(s, t, degrees + 60, m-1);
		glColor3f(0.0,0.5,0.5); //aqua line
		divide_line(t, b, degrees, m-1);
	}
	else
		drawLine(a,b);	//Draw line when m == 0
}

/*********************************************
||unproject() function
||Purpose: take an x and y value from the 
||	   screen coordinates and convert
||	   to world coordinates
*********************************************/
GLdouble * unproject(int x, int y)
{
	GLdouble modelMatrix[16];
	GLdouble projectionMatrix[16];
	int view[4];
	GLdouble * newCoord;
	
	//Retrieve model matrix
	glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
	//Retrieve projection matrix
	glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix);
	//Retrieve viewport 
	glGetIntegerv(GL_VIEWPORT,view);
	
	//Let unproject do magic
	gluUnProject((GLdouble) x, (GLdouble)(view[3]- y), (GLdouble) 1, modelMatrix,
	projectionMatrix, view, newCoord, newCoord+1, newCoord+2);
	
	return newCoord; //return new coordinates
}

/*********************************************
||unproject_float() function
||Purpose: take an x and y value from the 
||	   screen coordinates and convert
||	   to world coordinates
*********************************************/
float * unproject_float(int x, int y)
{
	GLdouble modelMatrix[16];
	GLdouble projectionMatrix[16];
	int view[4];
	GLdouble * newCoord;
	float * returnCoord;

	//Allocate memory for returnCoord
	returnCoord = (float *)malloc(3 * sizeof(float));	

	//Retrieve model matrix
	glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
	//Retrieve projection matrix
	glGetDoublev(GL_PROJECTION_MATRIX,projectionMatrix);
	//Retrieve viewport
	glGetIntegerv(GL_VIEWPORT,view);
	
	//Let unproject do magic
	gluUnProject((GLdouble) x, (GLdouble)(view[3]- y), (GLdouble) 1, modelMatrix,
	projectionMatrix, view, newCoord, newCoord+1, newCoord+2);
	
	//Convert coordinates to float
	returnCoord[0] = (float)newCoord[0];
	returnCoord[1] = (float)newCoord[1];
	returnCoord[2] = (float)newCoord[2];

	return returnCoord; //return new coordinates
}

/*********************************************
||passiveMouseLocation() function
||Purpose: update the location of the mouse 
|| 	   in a global variable and refresh
 *********************************************/
void passiveMouseLocation(int x, int y)
{
	pPassiveMouseLocation.x = x;
	pPassiveMouseLocation.y = y;
	glutPostRedisplay();
}

/*********************************************
||movingMouse() function
||Purpose: draw snowflake with a mouse drag
*********************************************/
void movingMouse(int x, int y)
{
	//Declarations
	pDragMouseStart.x; //starting position of mouse
	pDragMouseStart.y; //starting position of mouse
	pPassiveMouseLocation.x = x; //update mouse location even during draw
	pPassiveMouseLocation.y = y; //update mouse location even during draw
	GLdouble *coord; //one click draw
	int points[3][2];
	float * temp; //temporary place to hold a set of coordinates
	

	//Click with no purposeful drag
	if((abs(pDragMouseStart.x - x)) <= 10 && (abs(pDragMouseStart.y - y)) <=10)
	{
		//Unproject the starting position of the mouse
		coord = unproject(pDragMouseStart.x,pDragMouseStart.y);

		//Calculate points for the snowflake
		w[2][0]=coord[0];
		w[2][1]=coord[1];
		w[1][0] = w[2][0] + ((1.037) * cos(5.23598776)); //find new x
		w[1][1] = w[2][1] + ((1.037) * sin(5.23598776)); //find new y
		w[0][0] = w[2][0] + ((1.037) * cos(4.1887902)); //find new x
		w[0][1] = w[2][1] + ((1.037) * sin(4.1887902)); //find new y

	}	
	//Click and drag top left to bottom right
	else if(pDragMouseStart.x < x && pDragMouseStart.y < y)
	{
		//Find points in screen coordinates
		points[2][0]=(pDragMouseStart.x+(x-pDragMouseStart.x)/2.0);
		points[2][1]= pDragMouseStart.y;
		points[1][0]= x;
		points[1][1]= y;
		points[0][0]= pDragMouseStart.x;
		points[0][1]= y;

		//convert screen coordinates to world coordinates
		temp = unproject_float(points[2][0],points[2][1]);
		w[2][0] = temp[0];
		w[2][1] = temp[1];
		temp = 	unproject_float(points[1][0],points[1][1]);
		w[1][0] = temp[0];
		w[1][1] = temp[1];
		temp = 	unproject_float(points[0][0],points[0][1]);
		w[0][0] = temp[0];
		w[0][1] = temp[1];

	}
	//Click and drag bottom left to top right
	else if(pDragMouseStart.x < x && pDragMouseStart.y > y)
	{
		//Find points in screen coordinates
		points[2][0]=(pDragMouseStart.x+(x-pDragMouseStart.x)/2.0);
		points[2][1]= y;
		points[1][0]= x;
		points[1][1]= pDragMouseStart.y;
		points[0][0]= pDragMouseStart.x;
		points[0][1]= pDragMouseStart.y;

		//convert screen coordinates to world coordinates
		temp = unproject_float(points[2][0],points[2][1]);
		w[2][0] = temp[0];
		w[2][1] = temp[1];
		temp = 	unproject_float(points[1][0],points[1][1]);
		w[1][0] = temp[0];
		w[1][1] = temp[1];
		temp = 	unproject_float(points[0][0],points[0][1]);
		w[0][0] = temp[0];
		w[0][1] = temp[1];
	}
	//Click and drag bottom right to top left
	else if(pDragMouseStart.x > x && pDragMouseStart.y > y)
	{
		//Find points in screen coordinates
		points[2][0]=(pDragMouseStart.x+(x-pDragMouseStart.x)/2.0);
		points[2][1]= y;
		points[1][0]= pDragMouseStart.x;
		points[1][1]= pDragMouseStart.y;
		points[0][0]= x;
		points[0][1]= pDragMouseStart.y;

		//convert screen coordinates to world coordinates
		temp = unproject_float(points[2][0],points[2][1]);
		w[2][0] = temp[0];
		w[2][1] = temp[1];
		temp = 	unproject_float(points[1][0],points[1][1]);
		w[1][0] = temp[0];
		w[1][1] = temp[1];
		temp = 	unproject_float(points[0][0],points[0][1]);
		w[0][0] = temp[0];
		w[0][1] = temp[1];
	}
	//click and drag top right to bottom left
	else
	{

		//Find points in screen coordinates
		points[2][0]=(pDragMouseStart.x+(x-pDragMouseStart.x)/2.0);
		points[2][1]= pDragMouseStart.y;
		points[1][0]= pDragMouseStart.x;
		points[1][1]= y;
		points[0][0]= x;
		points[0][1]= y;

		//convert screen coordinates to world coordinates
		temp = unproject_float(points[2][0],points[2][1]);
		w[2][0] = temp[0];
		w[2][1] = temp[1];
		temp = 	unproject_float(points[1][0],points[1][1]);
		w[1][0] = temp[0];
		w[1][1] = temp[1];
		temp = 	unproject_float(points[0][0],points[0][1]);
		w[0][0] = temp[0];
		w[0][1] = temp[1];

	}
		glutPostRedisplay();
		

}

/*********************************************
||myMouse() function
||Purpose:  With the left mouse button you can start
|| 	   and stop the snowflakes spinning.
*********************************************/
void myMouse(int btn, int state, int x, int y)
{

	iPOINT start, end;
	GLdouble *coord;
	
   	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		pDragMouseStart.x = x;//update mouse dragging start position
		pDragMouseStart.y = y;//update mouse dragging start position 
		
		drawSecondSnowflake=1; //let it draw the second snowflake
		coord = unproject(x,y);//unproject the coordinates

		//Calculate position for coordinates
		w[2][0]=coord[0];
		w[2][1]=coord[1];
		w[1][0] = w[2][0] + ((1.037) * cos(5.23598776)); //find new x
		w[1][1] = w[2][1] + ((1.037) * sin(5.23598776)); //find new y
		w[0][0] = w[2][0] + ((1.037) * cos(4.1887902)); //find new x
		w[0][1] = w[2][1] + ((1.037) * sin(4.1887902)); //find new y
		
		//Refresh
		glutPostRedisplay();

	}	

}

/*********************************************
||myKey() function
||Purpose: -,+ keys change the number of iterations accordingly
|| 	    O will take you back to the original number of iterations
|| 		also will remove second snowflake
|| 	    Q will close the window
|| 	    0-9 will take you to that number of iterations
|| 	    Spacebar will recenter the snowflake
|| 	    w,a,s,d can be used as arrow keys
||Note: Upper limit is equal to 9 because there are 
|| 	no visible changes past this point
*********************************************/
void myKey(unsigned char key, int x, int y)
{

	if( key == 'o' | key == 'O')
	{
		n = orig; //Restore original number of iterations
		drawSecondSnowflake=0; //turn off second snowflake
		glutPostRedisplay();
	}
	
	if( key == '=' | key == '+')
	{
		if( n < 9)
		{
			n++; //Increment # of iterations
			glutPostRedisplay();
		}
	}
	
	if( key == '-' | key == '_')
	{
		if( n > 0)
		{
			n--; //Decrement # of iterations
			glutPostRedisplay();
		}
	}

	if( key == 'Q' | key == 'q')
		exit(0); //Quit the program

	if( key >= '0' && key <= '9') //any number 0-9
	{
		key = key - 48; //Convert from ascii value to actual #
		n = key; //Change iterations to that number
		glutPostRedisplay();
	}
	
	if( key == ' ' )
	{
		//Send how many x or y to return to original position
		moveSnowflakes(pTracker.x,pTracker.y);
		pTracker.x =0; //Reset tracker for x
		pTracker.y =0; //Reset tracker for y
		glutPostRedisplay();
	}
	if(key == 'w' | key == 'W')
	{
		moveSnowflakes(0,1); //move snowflake up y axis
		pTracker.y--; //Tracks how to restore original position
		glutPostRedisplay();
	}

	if(key ==  's' | key == 'S')
	{
		moveSnowflakes(0,-1); //move snowflake down y axis
		pTracker.y++; //Tracks how to restore original position
		glutPostRedisplay();
	}

	if(key == 'd' | key == 'D')
	{
		moveSnowflakes(1,0); //move snowflake right on x axis
		pTracker.x--; //Track how to restore original position
		glutPostRedisplay();
	}

	if(key == 'a' | key == 'A')
	{
		moveSnowflakes(-1,0); //move snowflake left on x axis
		pTracker.x++; //Track how to restore original position 
		glutPostRedisplay();
	}

}

/*********************************************
||specialKeys() function
||Purpose: Allows you to use the arrow keys to move
|| 	   the snowflake around the screen
*********************************************/
void specialKeys(int key, int x, int y) 
{
		if(key == GLUT_KEY_UP)
		{
			moveSnowflakes(0,1); //move snowflake up y axis
			pTracker.y--; //Tracks how to restore original position
			glutPostRedisplay();
		}

		if(key ==  GLUT_KEY_DOWN)
		{
			moveSnowflakes(0,-1); //move snowflake down y axis
			pTracker.y++; //Tracks how to restore original position
			glutPostRedisplay();
		}

		if(key == GLUT_KEY_RIGHT)
		{
			moveSnowflakes(1,0); //move snowflake right on x axis
			pTracker.x--; //Track how to restore original position
			glutPostRedisplay();
		}

		if(key == GLUT_KEY_LEFT)
		{
			moveSnowflakes(-1,0); //move snowflake left on x axis
			pTracker.x++; //Track how to restore original position 
			glutPostRedisplay();
		}

} 

/*********************************************
||displayString() function
||Purpose: draw a string on the screen
*********************************************/
void displayString(double x, double y, char *str) 
{
    glRasterPos3d(x, y, 1);
    while (*str) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
	// Other fonts available.  For example..
        //glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *str);
        //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);
        str++;
    }
}

/*********************************************
||drawMouseHertz() function
||Purpose: draw the hertz, mouse location, and
|| 	   number of iterations on screen
*********************************************/
void drawMouseHertz(int elapsedTime)
{
   //===== Mouse Location ========
    char cstrMouseCoordinate[10];
    GLfloat xString=-1.9,
	    yString=1.85; 

   //-----Construct String -------    
    sprintf( cstrMouseCoordinate, "(%d, %d)", pPassiveMouseLocation.x, pPassiveMouseLocation.y );

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0); //white line

   //------Display String -------
    displayString(xString,yString,cstrMouseCoordinate);

   //========= Hertz ============
   GLfloat hertz;
   static int timer=0, //timer accumulator
	      runOnce=1, //run if statement once
   	      frames=0; //frames accumulator
   static char cstrHertz[10]; //display hertz

   if(runOnce)
   {
	sprintf(cstrHertz, "??.?? Hz");
	runOnce--; //turn off this if statment
   }
   if(timer > 3000) //If more than 3 seconds
   {
	//---Calculate hertz -----
	hertz = (GLfloat) frames / ((GLfloat) (timer / 1000));

	//---Construct String ----
	for(int i=0; i < 10; i++)
		cstrHertz[i]=NULL;
	sprintf(cstrHertz, "%.2f Hz", hertz);

	//---Display string ----
	displayString((-xString-.5),-yString,cstrHertz);
	
	//----Reset Accumulators------
	frames = 0; 
	timer = 0;
   }
   else
   {
   	timer+=elapsedTime; //accumulate time
	frames++; //accumulate # of frames	

	displayString((-xString-.5),-yString,cstrHertz);

   }

    //=========== Iterations ===========
    char cstrIterations[13];

    sprintf(cstrIterations, "%d iterations", n);
    displayString(-1.9, 1.6,cstrIterations);

}

/*********************************************
||display() function
||Purpose: Draw 
**********************************************/
void display()
{
    int timer, start, end;

    start = glutGet(GLUT_ELAPSED_TIME); //start timer

    glBegin(GL_LINES);

   //First snowflake
    divide_line(v[0], v[1], 0, n); 
    divide_line(v[1], v[2], 120, n); 
    divide_line(v[2], v[0], -120, n); 

  //Second snowflake
   if(drawSecondSnowflake)
    {
    	divide_line(w[0], w[1], 0, n); 
    	divide_line(w[1], w[2], 120, n); 
    	divide_line(w[2], w[0], -120, n); 
    }

    glEnd();
    glFlush();

    end = glutGet(GLUT_ELAPSED_TIME); //end timer
    timer = end-start; //find elapsed time
    drawMouseHertz(timer); //send it in to be stored

}

/*********************************************
||display() function
||Purpose: Draw an empty display.
**********************************************/
void emptyDisplay()
{
    int timer, start, end;
    start = glutGet(GLUT_ELAPSED_TIME);
	glClear(GL_COLOR_BUFFER_BIT);
    end = glutGet(GLUT_ELAPSED_TIME);
    timer = end-start;
    drawMouseHertz(timer);
	glFlush();
}

void myinit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor (0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0,1.0,1.0);
}

/*********************************************
||main_menu() function
||Purpose: Handles the right click menu
*********************************************/
void main_menu(int index)
{
	switch(index)
	{
		case(0): //Clear screen
		{
    			glutDisplayFunc(emptyDisplay);
			glutPostRedisplay();
			break;
		}
		case(1): //Draw snowflakes again
		{
    			glutDisplayFunc(display);
			glutPostRedisplay();
			break;
		}
		case(4): //Quit Program
			exit(0);
	}
}

void refresher()
{ glutPostRedisplay();}

int main(int argc, char **argv)
{

  //If argument count < 2 or if the second argument is < 0 set defaults
  if(argc <  2 || atoi(argv[1])<0 || atoi(argv[1])>9)
  {
	n = 4;
	orig = 4;
  }
  else //otherwise take input from the command line
  {
    n=atoi(argv[1]); 
    orig = atoi(argv[1]); // keep the orginal so that 'o' can be used to go back 
			  // to the original input if desired
  }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Koch Snowflake");
    glutDisplayFunc(display);
    myinit();

    //Menu
    glutCreateMenu(main_menu);
    glutAddMenuEntry("Clear Screen", 0);
    glutAddMenuEntry("Redraw Snowflakes", 1);
    glutAddMenuEntry("Quit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutIdleFunc(refresher);//Constantly refresh to calculate hertz
    glutPassiveMotionFunc(passiveMouseLocation); //update mouse location
    glutMotionFunc(movingMouse); //Mouse dragging events
    glutMouseFunc(myMouse); //Mouse click events
    glutKeyboardFunc(myKey); //Keyboard events
    glutSpecialFunc(specialKeys); //Special keyboard events
    glutMainLoop();
}

