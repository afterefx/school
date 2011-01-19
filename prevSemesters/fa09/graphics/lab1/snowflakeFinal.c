/********************************************************************
||Name:       Christopher Carlisle
||COURSE:     COSC 4328
||Lab #:      1
||OBJECTIVE:   Draw a Koch Snowflake
********************************************************************/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>

int n, 		//Holds number of iterations
    orig,	//Backup of original number of iterations passed in

/*Next two variables will allow us 
to move snowflake back to original position*/
    trackerX=0, 
    trackerY=0;
const GLfloat pie = 3.141592653589793;
static GLfloat spin = 0.0; 	//Amount to rotate each time

/* initial triangle */
GLfloat v[3][2]={{-0.5, -0.29}, {0.5, -0.29}, {0.0, 0.575}},
	w[3][2]={{-1.165, 0.485}, {-0.165, 0.485}, {-0.665, 1.35}};

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
||spinDisplay() function
||Purpose: Rotate snowflake on the x=y axis
||postcondition: Snowflake is rotated by 0.8 degrees on each refresh
*********************************************/
void spinDisplay()
{
	spin = 0.8;
  	glutPostRedisplay();
}

/*********************************************
||drawLine() function
||Purpose: Receive 2 points and draw a line
*********************************************/
void drawLine( GLfloat *a, GLfloat *b )
/* specify one line */
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
||myMouse() function
||Purpose:  With the left mouse button you can start
|| 	   and stop the snowflakes spinning.
*********************************************/
void myMouse(int btn, int state, int x, int y)
{

	static int button_state = 0; //keep track of spinning
   	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      		if (button_state == 0) //not spinning
        	{
         		button_state = 1;
         		glutIdleFunc(spinDisplay);//start spinning
        	}
      		else //spinning
        	{
         		button_state = 0; 
         		glutIdleFunc(NULL); //stop spinning
        	} 
}

/*********************************************
||myKey() function
||Purpose: -,+ keys change the number of iterations accordingly
|| 	    O will take you back to the original number of iterations
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
		moveSnowflakes(trackerX,trackerY);
		spin =0.0;
		trackerX =0; //Reset tracker for x
		trackerY =0; //Reset tracker for y
		glutPostRedisplay();
	}
	if(key == 'w' | key == 'W')
	{
		moveSnowflakes(0,1); //move snowflake up y axis
		trackerY--; //Tracks how to restore original position
		glutPostRedisplay();
	}

	if(key ==  's' | key == 'S')
	{
		moveSnowflakes(0,-1); //move snowflake down y axis
		trackerY++; //Tracks how to restore original position
		glutPostRedisplay();
	}

	if(key == 'd' | key == 'D')
	{
		moveSnowflakes(1,0); //move snowflake right on x axis
		trackerX--; //Track how to restore original position
		glutPostRedisplay();
	}

	if(key == 'a' | key == 'A')
	{
		moveSnowflakes(-1,0); //move snowflake left on x axis
		trackerX++; //Track how to restore original position 
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
			trackerY--; //Tracks how to restore original position
			glutPostRedisplay();
		}

		if(key ==  GLUT_KEY_DOWN)
		{
			moveSnowflakes(0,-1); //move snowflake down y axis
			trackerY++; //Tracks how to restore original position
			glutPostRedisplay();
		}

		if(key == GLUT_KEY_RIGHT)
		{
			moveSnowflakes(1,0); //move snowflake right on x axis
			trackerX--; //Track how to restore original position
			glutPostRedisplay();
		}

		if(key == GLUT_KEY_LEFT)
		{
			moveSnowflakes(-1,0); //move snowflake left on x axis
			trackerX++; //Track how to restore original position 
			glutPostRedisplay();
		}

} 

/*********************************************
||display() function
||Purpose: Draw 2 triangles and if need be divide it N times
**********************************************/
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glRotatef(spin, 1.0, 1.0, 0.0); //Rotate coordinates on x=y axis
    glBegin(GL_LINES);

   //First snowflake
    divide_line(v[0], v[1], 0, n); 
    divide_line(v[1], v[2], 120, n); 
    divide_line(v[2], v[0], -120, n); 

  //Second snowflake
    divide_line(w[0], w[1], 0, n); 
    divide_line(w[1], w[2], 120, n); 
    divide_line(w[2], w[0], -120, n); 

    glEnd();
    glFlush();
}

/*********************************************
||display() function
||Purpose: Draw an empty display.
**********************************************/
void emptyDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);
	glEnd();
	glFlush();
}

/*********************************************
||myreshape() function
||Purpose: If the window is reshaped adjust the 
|| 	   orthographic projection accordingly
|| 	   to retain the shape of the drawing
*********************************************/
void myreshape(int w, int h)
{

	glViewport( 0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //change projection mode
	glLoadIdentity(); //clearing the projection
	
	if (w <= h)
	  gluOrtho2D(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w, 2.0 * (GLfloat) h / (GLfloat) w) ; 
	else 
	  gluOrtho2D (-2.0 * (GLfloat) w / (GLfloat) h, 2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0) ;
	glMatrixMode(GL_MODELVIEW) ;

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
||spin_menu() function
||Purpose: Handles submenu options for spinning
*********************************************/
void spin_menu(int index)
{
	switch(index)
	{
		case(0): //Start spinning
		{
			glutIdleFunc(spinDisplay);
			break;
		}
		case(1): //Stop spinning
		{
			glutIdleFunc(NULL);
			break;
		}
	}
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
			glutIdleFunc(NULL);
    			glutDisplayFunc(emptyDisplay);
			glutPostRedisplay();
			break;
		}
		case(1): //Draw snowflakes again
		{
			glutIdleFunc(NULL);
    			glutDisplayFunc(display);
			glutPostRedisplay();
			break;
		}
		case(4): //Quit Program
			exit(0);
	}
}

int main(int argc, char **argv)
{
	int s_menu;

  //If argument count < 2 or if the second argument is < 0 set defaults
  if(argc <  2 || atoi(argv[1])<0 || atoi(argv[1])>9)
  {
	n = 0;
	orig = 0;
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

    //Submenu
    s_menu = glutCreateMenu(spin_menu);
    glutAddMenuEntry("Start",0);
    glutAddMenuEntry("Stop",1);

    //Menu
    glutCreateMenu(main_menu);
    glutAddMenuEntry("Clear Screen", 0);
    glutAddMenuEntry("Redraw Snowflakes", 1);
    glutAddSubMenu("Spin", s_menu);
    glutAddMenuEntry("Quit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMouseFunc(myMouse); //Mouse events
    glutKeyboardFunc(myKey); //Keyboard events
    glutSpecialFunc(specialKeys); //Special keyboard events
    glutReshapeFunc(myreshape); //Window reshape events
    glutMainLoop();
}

