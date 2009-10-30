/* recursive subdivision of triangle to form Sierpinski gasket */
/* number of recursive steps given on command line */

#include <iostream>
#include <math.h>
using namespace std;

/* initial triangle */

double v[3][2]={{-1.0, -0.58}, {1.0, -0.58}, {0.0, 1.15}};

int n;
const double pie = 3.141592653589793;

void drawLine( double *a, double *b )
/* specify one triangle */
{
	cout << "\n-------Entered draw line -------\n";
	//output results here

	cout << "a[0,1]: " << a[0] << ", " << a[1] << endl;
	cout << "b[0,1]: " << b[0] << ", " << b[1] << endl;
}

double degreesToRadians(int degrees)
{
	cout << "Entered degreesToRadians()";
	double returnValue;
	returnValue = (degrees * (pie / 180.0));
	
	cout << "Returning " << returnValue << endl;
	return returnValue;
}

double length(double *a, double * b)
{
	cout << "Entered length()\n";
	double tempValueX, tempValueY,
		lengthOfSide;

	//Length of L
	tempValueX = (a[0] - b[0]);
	if(tempValueX < 0.0)
		tempValueX = (tempValueX * -1.0);
	tempValueY = (a[1] - b[1]);
	if(tempValueY < 0.0)
		tempValueY = (tempValueY * -1.0);
	lengthOfSide = (tempValueX * tempValueX)+ (tempValueY * tempValueY);
	lengthOfSide = sqrt(lengthOfSide);

	cout << "Returning " << lengthOfSide << endl;
	return lengthOfSide;
}

void divide_line(double *a, double *b, int degrees, int m)
{
	cout << "===============================\n";
	cout << "||Now in divide_line function||\n";
	cout << "===============================\n";

	//Declarations
	double lengthOfSide,
		radians;
	double	r[2], s[2], t[2];


	//Find the length of the line
	cout << "Entering length function \n";
	cout << "Sending a[0]: " << a[0] << endl;
	cout << "a[1]: " << a[1] << endl;
	cout << "b[0]: " << b[0]  << endl; 
	cout << "b[1]: " << b[1] << endl;
	lengthOfSide = length(a, b);
	cout << "Received: " << lengthOfSide << endl;

	//Convert degrees to radians
	cout << "Entering degree to radian function. sending" << degrees;
	cout << endl;
	radians = degreesToRadians(degrees);
	cout << "Received " << radians << " radians\n";

	//Math
	r[0] = a[0] + (lengthOfSide/3.0); //find new x
	r[1] = a[1] + (lengthOfSide/3.0); //find new y 
	s[0] = a[0] + ((lengthOfSide/3.0) * cos(radians)); //find new x
	s[1] = a[1] + ((lengthOfSide/3.0) * sin(radians)); //find new y	
	t[0] = a[0] + ((lengthOfSide/3.0) * 2.0); //find new x
	t[1] = a[1] + ((lengthOfSide/3.0) * 2.0); //find new x
	
	cout << "Finished math." << endl;
	cout << "Results are: " << endl;
	cout << "a[0,1]: " << a[0] << ", " << a[1] << endl;
	cout << "r[0,1]: " << r[0] << ", " << r[1] << endl;
	cout << "s[0,1]: " << s[0] << ", " << s[1] << endl;
	cout << "t[0,1]: " << t[0] << ", " << t[1] << endl;
	cout << "b[0,1]: " << b[0] << ", " << b[1] << endl;

	cout << "Value of m is: " << m << endl;
	if(m >0)
	{
	//Next iteration
		cout << "M > 0 calling next divideLine\n";
		divide_line(a, r, degrees, m-1);
		cout << "Back in parent " << m << " going next\n";
		divide_line(r, s, degrees - 60, m-1);
		cout << "Back in parent " << m << " going next\n";
		divide_line(s, t, degrees + 60, m-1);
		cout << "Back in parent " << m << " going next\n";
		divide_line(t, b, degrees, m-1);
		cout << "End of function return to display()\n";
	}
	else
	{
		cout << "M == 0\n" ;
		cout << "Calling draw lines\n";
		drawLine(a,r);
		drawLine(r,s);
		drawLine(s,t);
		drawLine(t,b);
		cout << "stepping back up\n";
	}
}

void display()
{
	cout << "Now in display function\n";
	cout << "Headed into first divide line\n";
    divide_line(v[0], v[1], 0, n); //, v[2], n);
	cout << "Back in display(). sending next divide line\n";
    divide_line(v[1], v[2], 60, n); //, v[2], n);
	cout << "Back in display(). sending last divide line\n";
    divide_line(v[2], v[0], 120, n); //, v[2], n);
	cout << "Done with display()";
}

int main(int argc, char **argv)
{
	cout << "Entering main here" << endl;
    n=atoi(argv[1]); /* or set number of subdivision steps here */
	cout << "N is equal to " << n << endl;
	cout << "======= Entering display function ======\n";
    display();
	cout << "Done with display function \n";
	
	return 0;
}

