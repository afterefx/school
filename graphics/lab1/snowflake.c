/* recursive subdivision of triangle to form Sierpinski gasket */
/* number of recursive steps given on command line */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

struct coordinates
{
	GLfloat x;
	GLfloat y;
};

GLfloat v[3][2]={{-1.0, -0.58}, {1.0, -0.58}, {0.0, 1.15}};

int n;

void triangle( GLfloat *a, GLfloat *b, GLfloat *c)
/* specify one triangle */
{
       glVertex2fv(a);
       glVertex2fv(b);
       glVertex2fv(c);
}

void divide_snowflake(GLfloat *a, GLfloat *b, GLfloat *c, int m)
{
/* triangle subdivision using vertex numbers */
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
    else triangle(a,b,c); /* draw triangle at end of recursion */
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINE_LOOP);
    divide_snowflake(v[0], v[1], v[2], n);
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

GLfloat length(coordinates *a, coordinates *b)
{
	GLfloat tempValueX, tempValueY; //Holding places to do calculations
	GLfloat length; //Current length of the side of the triangle
	
	//Length of L
	tempValueX = abs(a.x-b.x);
	tempValueY = abs(a.y-b.y);
	length = (tempValueX * tempValueX)+(tempValueY * tempValueY);
	length = sqrt(length); 

	//Testing
	cout << "The length of the line between {" << a.x  << ", " << a.y << "}";
	cout << " and {" << b.x << ", " << b.y << "} is " << length;

	return length;
}

GLfloat slope(coordinates *a, coordinates *b)
{
	//y2-y1/x2-x
	return ((b.y - a.y)/(b.x-a.x));	
}

coordinates divideTriangle(coordinates *a, coordinates *b, int m)
{
	coordinates c,   //Final coordinates
		    r,  //point for smaller triangle nearest a
		    s,  //point after turning 60 degrees
		    t,  //point for smaller triangle nearest b
		    primaryUnitV; //holds a unit vector; 
		    secondaryUnitV; //holds a unit vector; 
	GLfloat L; //Holds the length of the line
	//	v0[2], v1[2], v3[2],
	//	m, //Holds the slope of the line
	//	yInter; //Holds the y-intercept
	//
	//	m = slope(&a, &b); //Store the slope of the side
	//	yInter = a.y-(m*a.x);
	//	for(int i=0; i < 2 ; i++)
		
	//	find unit vector
	//	multiply by magnitude
	//	add on to a

	L = length(&a, &b); //Store the length of the side

	//Unit vector b-a/l
		primaryUnitV.x = b.x - a.x;
		primaryUnitV.y = b.y - a.y;
		primaryUnitV.x = primaryUnitV.x / L;	
		primaryUnitV.y = primaryUnitV.y / L;	

	//Make a copy of the primary to the 2nd unit vector before changing it
		secondaryUnitV.x = (primaryUnitV.x * 0.5);
		secondaryUnitV.y = (primaryUnitV.y * 0.5);

	//Multiply by magnitude
		primaryUnitV.x = primaryUnitV.x * (1.0/3.0);
		primaryUnitV.y = primaryUnitV.y * (1.0/3.0);

	//Add on to a for r
		r.x = primaryUnitV.x + a.x;
		r.y = primaryUnitV.y + a.y;

	//Add on to a for s
		s.x = secondaryUnitV.x + a.x;
		s.y = secondaryUnitV.y + a.y;

	//Add on to a for t
		t.x = (primaryUnitV.x * 2.0)+ a.x;
		t.y = (primaryUnitV.y * 2.0)+ a.y;

}


