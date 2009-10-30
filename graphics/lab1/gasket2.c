/* recursive subdivision of triangle to form Sierpinski gasket */
/* number of recursive steps given on command line */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>

/* initial triangle */

GLfloat v[3][2]={{-1.0, -0.58}, {1.0, -0.58}, {0.0, 1.15}};

int n;
const GLfloat pie = 3.141592653589793;

void drawLine( GLfloat *a, GLfloat *b )
/* specify one triangle */
{
       glVertex2fv(a);
       glVertex2fv(b);
}

double degreesToRadians(int degrees)
{
	return (degrees * (pie / 180.0));
}

double length(GLfloat *a, GLfloat * b)
{
	GLfloat tempValueX, tempValueY,
		lengthOfSide;

	//Length of L
	tempValueX = (a[0] - b[0]);
	//if(tempValueX < 0.0)
		//tempValueX = (tempValueX * -1.0);
	tempValueY = (a[1] - b[1]);
	//if(tempValueY < 0.0)
		//tempValueY = (tempValueY * -1.0);
	lengthOfSide = (tempValueX * tempValueX)+ (tempValueY * tempValueY);
	lengthOfSide = sqrt(lengthOfSide);

	return lengthOfSide;
}

void divide_line(GLfloat *a, GLfloat *b, int degrees, int m)
{
	//Declarations
	double lengthOfSide,
		angle1, // holds the radians of -60
		angle2, // holds the radians of +60
		radians;
	GLfloat	r[2], s[2], t[2];


	if(m > 0)
	{
	//Find the length of the line
	lengthOfSide = length(a, b);

	//Convert degrees to radians
	radians = degreesToRadians(degrees);
	angle1 = degreesToRadians(-60+degrees);
	angle2 = degreesToRadians(60+degrees);

	//Math
	r[0] = a[0] + ((lengthOfSide/3.0) * cos(radians)); //find new x
	r[1] = a[1] + ((lengthOfSide/3.0) * sin(radians)); //find new y
	s[0] = r[0] + ((lengthOfSide/3.0) * cos(angle1)); //find new x
	s[1] = r[1] + ((lengthOfSide/3.0) * sin(angle1)); //find new y	
	t[0] = s[0] + ((lengthOfSide/3.0) * cos(angle2)); //find new x
	t[1] = s[1] + ((lengthOfSide/3.0) * sin(angle2)); //find new y	
	

	//Next iteration
		glColor3f(1.0,0.0,0.0);
		divide_line(a, r, degrees, m-1);
		glColor3f(0.0,1.0,0.0);
		divide_line(r, s, degrees - 60, m-1);
		glColor3f(0.0,0.0,1.0);
		divide_line(s, t, degrees + 60, m-1);
		glColor3f(0.0,0.5,0.5);
		divide_line(t, b, degrees, m-1);
	}
	else
		drawLine(a,b);	
	/*else
	{
		glColor3f(1.0,0.0,0.0);
		drawLine(a,r);
		glColor3f(0.0,1.0,0.0);
		drawLine(r,s);
		glColor3f(0.0,0.0,1.0);
		drawLine(s,t);
		glColor3f(0.0,0.5,0.5);
		drawLine(t,b);
	}*/
}

/*void divide_snowflake(GLfloat *a, GLfloat *b, GLfloat *c, int m)
{
// triangle subdivision using vertex numbers 
    GLfloat bob[2], fred[2], joe[2];
    int j;
    if(m>0)
    {
        for(j=0; j<2; j++)
	  bob[j]=(a[j]+b[j])/2;
        for(j=0; j<2; j++) 
	  fred[j]=(a[j]+c[j])/2;
        for(j=0; j<2; j++) 
	  joe[j]=(b[j]+c[j])/2;
        divide_snowflake(a, bob, fred, m-1);
        divide_snowflake(c, fred, joe, m-1);
        divide_snowflake(b, joe, bob, m-1);
    }
    else drawLine(a,b);//,c); // draw triangle at end of recursion 
}*/

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    divide_line(v[0], v[1], 0, n); //, v[2], n);
    divide_line(v[1], v[2], 120, n); //, v[2], n);
    divide_line(v[2], v[0], -120, n); //, v[2], n);
    glEnd();
    glFlush();
}

void myinit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor (1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0,0.0,0.0);
}

int main(int argc, char **argv)
{
    n=atoi(argv[1]); /* or set number of subdivision steps here */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Koch Snowflake");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
}
