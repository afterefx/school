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

//Prototypes


int main(int argc, char **argv)
{
	coordinates a, b, c;
	cout << "Enter a.x: ";
	cin >> a.x;
	cout << "Enter a.y: ";
	cin >> a.y;
	cout << "Enter b.x: ";
	cin >> b.x;
	cout << "Enter b.y: ";
	cin >> b.y:

	c=divideTriangle(&a, &b);
	
	cout << "The third point is (" << c.x << ", " << c.y << ")";

	return 0;


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

