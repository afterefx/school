/*****************************************
||Name: 	Christopher Carlisle
||Course: 	COSC 4328
||Lab #: 	4
||Objective: 	Implement lighting on house
|| 		from lab 3.
******************************************/
#include <math.h>
#include <stdlib.h>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define bool int
#define false 0
#define true 1

#ifndef M_PI
#define M_PI 3.14159
#endif


int 	winWidth, winHeight;
int 	toggle=0;
int iMaterialToggle=0;
float 	angle = 0.0, axis[3], trans[3];
char cstrHouseNum[]="House 1",
     cstrMaterial[25]= "Material: emerald";
bool 	trackingMouse = false;
bool 	redrawContinue = false;
bool    trackballMove = false;
bool 	house1= true, //track if house 1 is beign drawn
	drawSphere=false, //toggle sphere drawing on/off
	bRotateXp = false, //used to decide when to rotate
	bRotateXm = false,//used to decide when to rotate 
	bRotateYp = false,//used to decide when to rotate 
	bRotateYm = false,//used to decide when to rotate 
	bScaleP = false,//used to decide when to scale 
	bScaleM = false,//used to decide when to scale 
	firstRun = true;//used to shift coordinates once

/*----------------------------------------------------------------------*/


/*
** Draw the cube.
*/
 //Coordinates for house #1
GLfloat origHouse[][3] = {
    {-1.25,-1.0,-1.0}, {1.25,-1.0,-1.0}, {1.25,0.65,-1.0}, {-1.25,0.65,-1.0}, 
    {-1.25,-1.0,1.0}, {1.25,-1.0,1.0}, {1.25,0.65,1.0}, {-1.25,0.65,1.0},
    {.30,-1.0,1.0}, {.30, 0.0,1.0}, {-.30,0.0,1.0},{-.30, -1.0, 1.0},
    {.30,0.65,1.0}, {-.30,0.65,1.0},{0.0,1.75,0.0}, {.30,0.0,1.75},{-.30,0.0,1.75},
    {0.0,0.25,1.375}
};

 //Coordinates for house #2
GLfloat houseDos[][3] = {
    {-1.25,-1.0,-1.5}, {1.25,-1.0,-1.5}, {1.25,0.65,-1.5}, {-1.25,0.65,-1.5}, //0123
    {-1.25,-1.0,1.5}, {1.25,-1.0,1.5}, {1.25,0.65,1.5}, {-1.25,0.65,1.5}, //4567
    {-1.0,1.0,0.80},{1.0,1.0, 0.80}, {1.0,1.0, -1.2}, {-1.0, 1.0, -1.2}, //89,10,11
    {-1.0,1.5,0.80},{1.0,1.5, 0.80},{1.0,1.5,-1.2},{-1.0,1.5,-1.2}, //12,13,14,15
    {0.0, 1.75,0.60},{0.0,1.75,-0.60}, {1.25,-1.0,0.75},{2.6,-1.0,0.75}, //16,17,18,19
    {2.6,-1.0,-1.5},{1.24,0.65,0.75},{2.6,0.65,0.75},{2.6,0.65,-1.5}, //20,21,22,23
    {1.92,1.0,-.375}, {2.0,0.65,-1.5},{2.6,0.65,-2.75},{3.2,0.65,-2.75}, //24,25,26,27
    {2.3,1.0,-1.85},{2.9,1.0,-2.4},{3.7,0.65,-2.75},{3.7,0.65,-3.90}, //28,29,30,31
    {2.6,0.65,-3.90},{2.6,-1.0,-2.75},{3.7,-1.0,-2.75},{3.7,-1.0,-3.90},//32,33,34,35
    {2.6,-1.0,-3.90},{3.15,1.0,-3.325} //36,37

};

 //Coordinates for columns
GLfloat columns[][3] = {
    {0.30, -1.0, 1.75}, {0.30, -1.0, 1.65}, {0.20, -1.0, 1.65}, {0.20, -1.0, 1.75},
    {0.30, 0.0, 1.75}, {0.30, 0.0, 1.65}, {0.20, 0.0, 1.65}, {0.20, 0.0, 1.75}
    
};

 //Coordinates for base
GLfloat base[][3] = {
    {-1.5,-1.05,1.85}, {1.5,-1.05, 1.85}, {1.5, -1.05, -1.85}, {-1.5, -1.05, -1.85}
};

 //Colors for rainbow
GLfloat colors[][3] = {
    {0.0,0.0,0.0}, {1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0}, 
    {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0},
    {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0},
    {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0} 
};

/************************************************
|| displayString()
|| Purpose: Take a string in and display it 
|| 	    on the screen so that it cannot
|| 	    cannot rotate
************************************************/
void displayString(double x, double y, char *str)
{

	glColor3f(1.0,1.0,1.0); //set text color to white
    	glDisable(GL_LIGHTING); //disable lighting
	glPushMatrix(); //store comments on the side
	glLoadIdentity(); //reset the matrix
	glRasterPos3d(x,y,1); 
	while (*str)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
		str++;
	}
    	glEnable(GL_LIGHTING); //enable lighting
	glPopMatrix();

}

/************************************************
|| normal()
|| Purpose: calculate the normal for 3 vertices
|| 	    sent in
||
************************************************/
void normal(int a, int b, int c, GLfloat array[][3], GLfloat * fNormalX,
		GLfloat *fNormalY, GLfloat *fNormalZ)
{

	GLfloat Ax, Ay, Az, Bx, By, Bz;

	Bx = array[b][0] - array[a][0];
	By = array[b][1] - array[a][1];
	Bz = array[b][2] - array[a][2];
	Ax = array[c][0] - array[a][0];
	Ay = array[c][1] - array[a][1];
	Az = array[c][2] - array[a][2];

	*fNormalX = By * Az - Bz * Ay;
	*fNormalY = Bz * Ax - Bx * Az;
	*fNormalZ = Bx * Ay - By * Ax;


}

/************************************************
|| triangle()
|| Purpose: draws a normal triangle using points
|| 	    a, b, c, and tells it which array
|| 	    to use for the points
************************************************/
void triangle(int a, int b, int c, GLfloat array[][3])
{
	GLfloat fNormalX, fNormalY, fNormalZ; //used to store normals
   switch (toggle) //change draw type
  {
	  case 0:
		  glBegin(GL_TRIANGLES);
		  break;
	  case 1:
		  glBegin(GL_POINTS);
		  break;
	  case 2:
		  glBegin(GL_LINE_STRIP);
		  break;
	  default:
		  glBegin(GL_TRIANGLES);
		  break;
  }

   	//Find normal
	normal(a,b,c,array, &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);


  	if(house1)
		glColor3fv(colors[a]);
  	glVertex3fv(array[a]); //point a
  	if(house1)
		glColor3fv(colors[b]);
  	glVertex3fv(array[b]); //point b
  	if(house1)
		glColor3fv(colors[c]);
  	glVertex3fv(array[c]); //point c

  glEnd();
}


/************************************************
|| triangleRoof()
|| Purpose: draws a normal triangle using points
|| 	    a, b, c, and tells it which array
|| 	    to use for the points
|| Note: difference form triangle() is that
||  	 the colors in this function are set 
|| 	 to draw a part of roof
************************************************/
void triangleRoof(int a, int b, int c, GLfloat array[][3])
{
	//Used to store normals
	GLfloat fNormalX, fNormalY, fNormalZ;

   switch (toggle) //change draw type
  {
	  case 0:
		  glBegin(GL_TRIANGLES);
		  break;
	  case 1:
		  glBegin(GL_POINTS);
		  break;
	  case 2:
		  glBegin(GL_LINE_STRIP);
		  break;
	  default:
		  glBegin(GL_TRIANGLES);
		  break;
  }

   	//Find normal
	normal(a,b,c,array, &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);

  	glColor3f(0.0,0.0,0.0);
  	glVertex3fv(array[a]); //point a
  	glVertex3fv(array[b]); //point b
  	glColor3f(1.0,1.0,1.0);
  	glVertex3fv(array[c]); //point c
  glEnd();
}

/************************************************
|| polygon()
|| Purpose: draws a polygon with the coordinates
|| 	    sent in for the given array
************************************************/
void polygon(int a, int b, int c , int d, GLfloat array[][3])
{
	GLfloat fNormalX, fNormalY, fNormalZ;

    /* draw a polygon via list of vertices */
  switch (toggle) //change draw type
  {
	  case 0:
		  glBegin(GL_POLYGON);
		  break;
	  case 1:
		  glBegin(GL_POINTS);
		  break;
	  case 2:
		  glBegin(GL_LINE_STRIP);
		  break;
	  default:
		  glBegin(GL_POLYGON);
		  break;
  }

  	//Find normal
	normal(a,b,c,array, &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);


    	if(house1)
  		glColor3fv(colors[a]);
  	glVertex3fv(array[a]); //point a
    	if(house1)
 		glColor3fv(colors[b]);
  	glVertex3fv(array[b]); //point b
    	if(house1)
  		glColor3fv(colors[c]);
  	glVertex3fv(array[c]); //point c
    	if(house1)
  		glColor3fv(colors[d]);
  	glVertex3fv(array[d]); //point d
    glEnd();
}

/************************************************
|| roofPart()
|| Purpose: draws a polygon with the coordinates
|| 	    sent in for the given array
|| Note: difference is drawing with a set color
|| 	 for a roof
************************************************/
void roofPart(int a, int b, int c , int d, GLfloat array[][3])
{
	GLfloat fNormalX, fNormalY, fNormalZ;

    /* draw a polygon via list of vertices */
  switch (toggle) //change draw type
  {
	  case 0:
		  glBegin(GL_POLYGON);
		  break;
	  case 1:
		  glBegin(GL_POINTS);
		  break;
	  case 2:
		  glBegin(GL_LINE_STRIP);
		  break;
	  default:
		  glBegin(GL_POLYGON);
		  break;
  }
  	//Find normal
	normal(a,b,c,array, &fNormalX, &fNormalY, &fNormalZ);
	glNormal3f(fNormalX, fNormalY, fNormalZ);

  	glColor3f(0.0,0.0,0.0);
  	glVertex3fv(array[a]); //point a
  	glVertex3fv(array[b]); //point b
  	glColor3f(1.0,1.0,1.0);
  	glVertex3fv(array[c]); //point c
  	glVertex3fv(array[d]); //point d
    glEnd();
}

/************************************************
|| makeColumn()
|| Purpose: draws a default column
************************************************/
void makeColumn()
{
	polygon(0,3,2,1, columns); //bottom
	polygon(0,1,5,4, columns); //right
	polygon(3,0,4,7, columns); //front
	polygon(2,3,7,6, columns); //left
	polygon(1,2,6,5, columns); //back
}

/************************************************
|| modifyView()
|| Purpose: Used to make modifications to the
|| 	    geometry being displayed. Scaling,
|| 	    and roation.
************************************************/
void modifyView()
{
    if(bScaleP)
    {
	    glScalef(1.1,1.1,1.1);//scale bigger
	    bScaleP = false; //turn off scale
    }

    if(bScaleM)
    {
	    glScalef(0.9,0.9,0.9);//scale smaller
	    bScaleM = false; //turn off scale
    }
    
    if(bRotateXp)
    {
	    glRotatef(5, 0.0, 1.0, 0.0); //rotate to the left
	    bRotateXp = false; //turn off rotate
    }

    if(bRotateXm)
    {
	    glRotatef(-5, 0.0, 1.0, 0.0); //rotate to the right
	    bRotateXm = false; //turn off rotate
    }

    if(bRotateYp)
    {
	    glRotatef(5, 1.0, 0.0, 0.0); //rotate up
	    bRotateYp = false;//turn off rotate 
    }

    if(bRotateYm)
    {
	    glRotatef(-5, 1.0, 0.0, 0.0); //rotate down
	    bRotateYm = false;//turn off rotate 
    }
}

/************************************************
|| applyMaterial()
|| Purpose: used to apply materials to objects
************************************************/
void applyMaterial()
{
	GLfloat ambr, ambg, ambb, //ambient
	difr, difg, difb, //diffuse
	specr, specg, specb,  //specular
	shineness, //shine
	material[4];
	
	switch (iMaterialToggle)
	{

	//emerald
		case 0:
		 ambr=0.0215; ambg=0.1745; ambb=0.0215; //ambient
		 difr=0.07568; difg=0.61424; difb=0.07568; //diffuse
		 specr=0.633; specg=0.727811; specb=0.633; //specular
		 shineness=0.6; //shineness
		 break;

	//jade
		case 1:
		 ambr=0.135; ambg=0.2225; ambb=0.1575; //ambient
		 difr=0.54; difg=0.89; difb=0.63; //diffuse
		 specr=0.316228; specg=0.316228; specb=0.316228; //specular
		 shineness=0.1; //shineness
		 break;

	//obsidian
		case 2:
		 ambr=0.05375; ambg=0.05; ambb=0.06625; //ambient
		 difr=0.18275; difg=0.17; difb=0.22525; //diffuse
		 specr=0.332741; specg=0.328634; specb=0.346435; //specular
		 shineness=0.3; //shineness
		 break;

	//pearl
		case 3:
		 ambr=0.25; ambg=0.20725; ambb=0.20725; //ambient
		 difr=1; difg=0.829; difb=0.829; //diffuse
		 specr=0.296648; specg=0.296648; specb=0.296648; //specular
		 shineness=0.088; //shineness
		 break;

	//ruby
		case 4:
		 ambr=0.1745; ambg=0.01175; ambb=0.01175; //ambient
		 difr=0.61424; difg=0.04136; difb=0.04136; //diffuse
		 specr=0.727811; specg=0.626959; specb=0.626959; //specular
		 shineness=0.6; //shineness
		 break;

	//turquoise	 
		case 5:
		 ambr=0.1; ambg=0.18725; ambb=0.1745; //ambient
		 difr=0.396; difg=0.74151; difb=0.69102; //diffuse
		 specr=0.297254; specg=0.30829; specb=0.306678; //specular
		 shineness=0.1; //shineness
		 break;

	//brass
		case 6:
		 ambr=0.329412; ambg=0.223529; ambb=0.027451; //ambient
		 difr=0.780392; difg=0.568627; difb=0.113725; //diffuse
		 specr=0.992157; specg=0.941176; specb=0.807843; //specular
		 shineness=0.21794872; //shineness
		 break;

	//bronze
		case 7:
		 ambr=0.2125; ambg=0.1275; ambb=0.054; //ambient
		 difr=0.714; difg=0.4284; difb=0.18144; //diffuse
		 specr=0.393548; specg=0.271906; specb=0.166721; //specular
		 shineness=0.2; //shineness
		 break;

	//chrome
		case 8:
		 ambr=0.25; ambg=0.25; ambb=0.25; //ambient
		 difr=0.4; difg=0.4; difb=0.4; //diffuse
		 specr=0.774597; specg=0.774597; specb=0.774597; //specular
		 shineness=0.6; //shineness
		 break;

	//copper
		case 9:
		 ambr=0.19125; ambg=0.0735; ambb=0.0225; //ambient
		 difr=0.7038; difg=0.27048; difb=0.0828; //diffuse
		 specr=0.256777; specg=0.137622; specb=0.086014; //specular
		 shineness=0.1; //shineness
		 break;

	//gold
		case 10:
		 ambr=0.24725; ambg=0.1995; ambb=0.0745; //ambient
		 difr=0.75164; difg=0.60648; difb=0.22648; //diffuse
		 specr=0.628281; specg=0.555802; specb=0.366065; //specular
		 shineness=0.4; //shineness
		 break;

	//silver
		case 11:
		 ambr=0.19225; ambg=0.19225; ambb=0.19225; //ambient
		 difr=0.50754; difg=0.50754; difb=0.50754; //diffuse
		 specr=0.508273; specg=0.508273; specb=0.508273; //specular
		 shineness=0.4; //shineness
		 break;

	//black plastic
		case 12:
		 ambr=0.0; ambg=0.0; ambb=0.0; //ambient
		 difr=0.01; difg=0.01; difb=0.01; //diffuse
		 specr=0.50; specg=0.50; specb=0.50; //specular
		 shineness=0.25; //shineness
		 break;

	//cyan plastic
		case 13:
		 ambr=0.0; ambg=0.1; ambb=0.06; //ambient
		 difr=0.0; difg=0.50980392; difb=0.50980392; //diffuse
		 specr=0.50196078; specg=0.50196078; specb=0.50196078; //specular
		 shineness=0.25; //shineness
		 break;

	//green plastic
		case 14:
		 ambr=0.0; ambg=0.0; ambb=0.0; //ambient
		 difr=0.1; difg=0.35; difb=0.1; //diffuse
		 specr=0.45; specg=0.55; specb=0.45; //specular
		 shineness=0.25; //shineness
		 break;

	//red plastic
		case 15:
		 ambr=0.0; ambg=0.0; ambb=0.0; //ambient
		 difr=0.5; difg=0.0; difb=0.0; //diffuse
		 specr=0.7; specg=0.6; specb=0.6; //specular
		 shineness=0.25; //shineness
		 break;

	//white plastic
		case 16:
		 ambr=0.0; ambg=0.0; ambb=0.0; //ambient
		 difr=0.55; difg=0.55; difb=0.55; //diffuse
		 specr=0.70; specg=0.70; specb=0.70; //specular
		 shineness=0.25; //shineness
		 break;

	//yellow plastic
		case 17:
		 ambr=0.0; ambg=0.0; ambb=0.0; //ambient
		 difr=0.5; difg=0.5; difb=0.0; //diffuse
		 specr=0.60; specg=0.60; specb=0.50; //specular
		 shineness=0.25; //shineness
		 break;

	//black rubber
		case 18:
		 ambr=0.02; ambg=0.02; ambb=0.02; //ambient
		 difr=0.01; difg=0.01; difb=0.01; //diffuse
		 specr=0.4; specg=0.4; specb=0.4; //specular
		 shineness=0.078125; //shineness
		 break;
		 
	//cyan rubber
		case 19:
		 ambr=0.0; ambg=0.05; ambb=0.05; //ambient
		 difr=0.4; difg=0.5; difb=0.5; //diffuse
		 specr=0.04; specg=0.7; specb=0.7; //specular
		 shineness=0.078125; //shineness
		 break;

	//green rubber
		case 20:
		 ambr=0.0; ambg=0.05; ambb=0.0; //ambient
		 difr=0.4; difg=0.5; difb=0.4; //diffuse
		 specr=0.04; specg=0.7; specb=0.04; //specular
		 shineness=0.078125; //shineness
		 break;

	//red rubber
		case 21:
		 ambr=0.05; ambg=0.0; ambb=0.0; //ambient
		 difr=0.5; difg=0.4; difb=0.4; //diffuse
		 specr=0.7; specg=0.04; specb=0.04; //specular
		 shineness=0.078125; //shineness
		 break;

	//white rubber
		case 22:
		 ambr=0.05; ambg=0.05; ambb=0.05; //ambient
		 difr=0.5; difg=0.5; difb=0.5; //diffuse
		 specr=0.7; specg=0.7; specb=0.7; //specular
		 shineness=0.078125; //shineness
		 break;

	//yellow rubber
		case 23:
		 ambr=0.05; ambg=0.05; ambb=0.0; //ambient
		 difr=0.5; difg=0.5; difb=0.4; //diffuse
		 specr=0.7; specg=0.7; specb=0.04; //specular
		 shineness=0.078125; //shineness
		 break;

	//chrome
		default:
		 ambr=ambg=ambb=0.25; //ambient
		 difr=difg=difb=0.4; //diffuse
		 specr=specg=specb=0.774597; //specular
		 shineness=76.8; //shineness
		 break;
	}


	material[3]=1.0;

	material[0] = ambr; material[1] = ambg; material[2] = ambb; //store ambient
	glMaterialfv(GL_FRONT, GL_AMBIENT, material); //set ambient
	material[0] = difr; material[1] = difg; material[2] = difb; //store diffuse
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material); //set diffuse
	material[0] = specr; material[1] = specg; material[2] = specb; //store specular
	glMaterialfv(GL_FRONT, GL_SPECULAR, material); //set specular
	glMaterialf(GL_FRONT, GL_SHININESS, shineness); //set shineness
}

/************************************************
|| displayMaterial()
|| Purpose: used to display which material is being displayed
************************************************/
void displayMaterial()
{
	switch (iMaterialToggle)
	{

	//emerald
		case 0:
	    	 sprintf(cstrMaterial, "Material: emerald");
		 break;

	//jade
		case 1:
	    	 sprintf(cstrMaterial, "Material: jade");
		 break;

	//obsidian
		case 2:
	    	 sprintf(cstrMaterial, "Material: obsidian");
		 break;

	//pearl
		case 3:
	    	 sprintf(cstrMaterial, "Material: pearl");
		 break;

	//ruby
		case 4:
	    	 sprintf(cstrMaterial, "Material: ruby");
		 break;

	//turquoise	 
		case 5:
	    	 sprintf(cstrMaterial, "Material: turquoise");
		 break;

	//brass
		case 6:
	    	 sprintf(cstrMaterial, "Material: brass");
		 break;

	//bronze
		case 7:
	    	 sprintf(cstrMaterial, "Material: bronze");
		 break;

	//chrome
		case 8:
	    	 sprintf(cstrMaterial, "Material: chrome");
		 break;

	//copper
		case 9:
	    	 sprintf(cstrMaterial, "Material: copper");
		 break;

	//gold
		case 10:
	    	 sprintf(cstrMaterial, "Material: gold");
		 break;

	//silver
		case 11:
	    	 sprintf(cstrMaterial, "Material: silver");
		 break;

	//black plastic
		case 12:
	    	 sprintf(cstrMaterial, "Material: black plastic");
		 break;

	//cyan plastic
		case 13:
	    	 sprintf(cstrMaterial, "Material: cyan plastic");
		 break;

	//green plastic
		case 14:
	    	 sprintf(cstrMaterial, "Material: green plastic");
		 break;

	//red plastic
		case 15:
	    	 sprintf(cstrMaterial, "Material: red plastic");
		 break;

	//white plastic
		case 16:
	    	 sprintf(cstrMaterial, "Material: white plastic");
		 break;

	//yellow plastic
		case 17:
	    	 sprintf(cstrMaterial, "Material: yellow plastic");
		 break;

	//black rubber
		case 18:
	    	 sprintf(cstrMaterial, "Material: black rubber");
		 break;
		 
	//cyan rubber
		case 19:
	    	 sprintf(cstrMaterial, "Material: cyan rubber");
		 break;

	//green rubber
		case 20:
	    	 sprintf(cstrMaterial, "Material: green rubber");
		 break;

	//red rubber
		case 21:
	    	 sprintf(cstrMaterial, "Material: red rubber");
		 break;

	//white rubber
		case 22:
	    	 sprintf(cstrMaterial, "Material: white rubber");
		 break;

	//yellow rubber
		case 23:
	    	 sprintf(cstrMaterial, "Material: yellow rubber");
		 break;

	//chrome
		default:
	    	 sprintf(cstrMaterial, "Material: chrome");
		 break;
	}


}

/************************************************
|| drawMyHomes()
|| Purpose: called to draw the houses
************************************************/
void drawMyHomes(void)
{
    modifyView();
    
    applyMaterial();


    //draw house 1
    if(house1)
    {
	    //draw house
	    polygon(1,0,3,2,origHouse);//back side
	    polygon(4,11,13,7,origHouse); //left front
	    polygon(9,12,13,10,origHouse); //front top middle
	    polygon(8,5,6,12,origHouse);//right front
	    polygon(3,7,6,2,origHouse); //top
	    polygon(7,3,0,4,origHouse); //left side
	    polygon(2,6,5,1,origHouse); //right side
	    polygon(5,4,0,1,origHouse); //bottom side
	    polygon(15,9,10,16,origHouse); //front porch top
	    triangle(2,3,14,origHouse); //roof starts here
	    triangle(3,7,14,origHouse);
	    triangle(7,6,14,origHouse);
	    triangle(6,2,14,origHouse);
	    triangle(9,10,17,origHouse); //eave roof starts here
	    triangle(15,9,17,origHouse);
	    triangle(16,15,17,origHouse);
	    triangle(10,16,17,origHouse);

	    //making columns
	    makeColumn();

	    //making columns
	    glTranslatef(-.5,0.0,0.0);
		makeColumn();
	    glTranslatef(.5,0.0,0.0); 
       }
    else
    {
	    //scale second house down 
	    //first time it's drawn to 
	    //prevent clipping
	    if(firstRun)
	    {
		    glScalef(0.7,0.7,0.7); //scale smaller
		    firstRun=false;
	    }

	    //Positing to left for a little clipping prevention
	    glTranslatef(-1.2,0.0,0.7);

	    //main base
	    glColor3f(0.6,0.4,0.2);
	    polygon(1,0,3,2,houseDos); //back side
	    polygon(3,7,6,2,houseDos); //top
	    polygon(7,3,0,4,houseDos); //left side
	    polygon(2,6,5,1,houseDos); //right side
	    polygon(4,5,6,7,houseDos); //front side
	    polygon(5,4,0,1,houseDos); //bottom side

	    //partial roof
	    roofPart(7,6,9,8,houseDos); //front roof part
	    roofPart(6,2,10,9,houseDos); //right roof part
	    roofPart(2,3,11,10, houseDos); //back roof part
	    roofPart(3,7,8,11, houseDos); //left roof part

	    //upper floor
	    glColor3f(0.6,0.4,0.2);
	    polygon(8,9,13,12, houseDos); //front 
	    polygon(9,10,14,13, houseDos); //right
	    polygon(10,11,15,14, houseDos); //back
	    polygon(11,8,12,15, houseDos); //left
	    
	    //upper roof
	    triangleRoof(12,13,16, houseDos); //front
	    roofPart(13,14,17,16, houseDos); //right
	    roofPart(15,12,16,17, houseDos); //back
	    triangleRoof(14,15,17, houseDos);//left

	    //wing base
	    glColor3f(0.6,0.4,0.2);
	    polygon(19,18,1,20, houseDos); //bottom
	    polygon(18,19,22,21, houseDos); //front
	    polygon(19,20,23,22, houseDos); //right
	    polygon(20,1,2,23, houseDos); //back

	    //wing roof
	    triangleRoof(21,22,24, houseDos); //front
	    triangleRoof(22,23,24, houseDos); //right
	    triangleRoof(23,2,24, houseDos); //back
	    triangleRoof(2,21,24, houseDos); //left

	    //eave to garage
	    polygon(23,25,26,27, houseDos); //bottom
	    triangleRoof(25,23,28, houseDos); //front
	    roofPart(23, 27, 29, 28, houseDos); //right
	    triangleRoof(27,26,29, houseDos); //back
	    roofPart(26,25,28,29, houseDos); //left

	    //base garage
	    glColor3f(0.6,0.4,0.2);
	    polygon(34,33,36,35, houseDos); //bottom
	    polygon(33,34,30,26, houseDos); //front
	    polygon(34,35,31,30, houseDos); //right
	    polygon(36,33,26,32, houseDos); //left
	    polygon(35,36,32,31, houseDos); //back

	    //roof garage
	    triangleRoof(26,30,37, houseDos); //frront
	    triangleRoof(30,31,37, houseDos); //right
	    triangleRoof(32,26,37, houseDos); //left
	    triangleRoof(31,32,37, houseDos); //back

	    //Keep translate from continuously adding on itself
	    glTranslatef(1.2,0.0,-0.7);


    }

}

/*----------------------------------------------------------------------*/
/* 
** These functions implement a simple trackball-like motion control.
*/

float lastPos[3] = {0.0F, 0.0F, 0.0F};
int curx, cury;
int startX, startY;

void trackball_ptov(int x, int y, int width, int height, float v[3])
{
    float d, a;

    /* project x,y onto a hemi-sphere centered within width, height */
    v[0] = (2.0F*x - width) / width;
    v[1] = (height - 2.0F*y) / height;
    d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
    v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
    a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}

void mouseMotion(int x, int y)
{
    float curPos[3], dx, dy, dz;

    trackball_ptov(x, y, winWidth, winHeight, curPos);
	if(trackingMouse)
	{
	    dx = curPos[0] - lastPos[0];
	    dy = curPos[1] - lastPos[1];
	    dz = curPos[2] - lastPos[2];

	    if (dx || dy || dz) {
		angle = 90.0F * sqrt(dx*dx + dy*dy + dz*dz);

		axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
		axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
		axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

		lastPos[0] = curPos[0];
		lastPos[1] = curPos[1];
		lastPos[2] = curPos[2];
	    }
	} 

    glutPostRedisplay();
}

void startMotion(int x, int y)
{
    trackingMouse = true;
    redrawContinue = false;
    startX = x; startY = y;
    curx = x; cury = y;
    trackball_ptov(x, y, winWidth, winHeight, lastPos);
	trackballMove=true;
}

void stopMotion(int x, int y)
{
    trackingMouse = false;

    if (startX != x || startY != y) 
    {
	redrawContinue = true;
    } 
    else 
    {
	angle = 0.0F;
	redrawContinue = false;
	trackballMove = false;
    }
	trackballMove = false;
}

/*----------------------------------------------------------------------*/

/************************************************
|| display()
|| Purpose: draw text in 2 seperate viewports
|| 	    and then the houses in one full window
|| 	    viewport
************************************************/
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    //############ VIEWPORT 1 ###############
    //have a viewport in the top right corner to display strings
    glViewport((winWidth-80),(winHeight-38),40,20);
    if(house1)
	    sprintf(cstrHouseNum, "House 1");
    else
	    sprintf(cstrHouseNum, "House 2");

    displayString(-4.0,4.0,cstrHouseNum); //draw a string in the viewport

    //############ VIEWPORT 2 ###############
    glViewport((winWidth-185),(winHeight-38),40,20);
    displayMaterial();
    displayString(-4.0,-4.0,cstrMaterial); //draw a string in the viewport

    //############ VIEWPORT 3 ###############
    //have a viewport for the rest of the window

    glViewport(0.0,0.0,winWidth,winHeight);
    if (trackballMove) 
    {
	    glRotatef(angle, axis[0], axis[1], axis[2]); 
    }

    drawMyHomes(); //draw the houses

    if(drawSphere) //to help with trackball controls
    {
        glColor3f(0.2,0.2,0.2);
    	glutWireSphere(1.75,20,20);
    }

    glutSwapBuffers();
}

/*----------------------------------------------------------------------*/

void mouseButton(int button, int state, int x, int y)
{
	if(button==GLUT_LEFT_BUTTON) 
		switch(state) 
		{
		    case GLUT_DOWN:
			y=winHeight-y;
			startMotion( x,y);
			break;
		    case GLUT_UP:
			stopMotion( x,y);
			break;
		} 
}

/************************************************
|| myReshape()
|| Purpose: reshape the viewport if the window size
|| 	    changes
************************************************/
void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);

    //keep scene from becoming distorted
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if(w <= h)
	    glOrtho(-4.0,4.0, -4.0 * (GLfloat) h / (GLfloat) w, 4.0 * (GLfloat)
			    h / (GLfloat) w, -4.0, 4.0);
    else
	    glOrtho(-4.0 * (GLfloat) w / (GLfloat) h, 4.0 * (GLfloat)
			    w / (GLfloat) h, -4.0, 4.0, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);

    winWidth = w;
    winHeight = h;
}

/************************************************
|| myKey()
|| Purpose: allow keys to control my program
|| keys: 'q' to quit
||	 's' to toggle drawing of a sphere
|| 	 '1' to draw house 1
|| 	 '2' to draw house 2
|| 	 '+' to zoom in
|| 	 '-' to zoom out
|| 	 't' to toggle drawing of verticies, lines, and polygons
|| 	 'm' to go to the next material
|| 	 'n' to go to the previous material
************************************************/
void myKey(unsigned char key, int x, int y)
{

	if( key == 'Q' | key == 'q')
		exit(0); //Quit the program
	if( key == 'S' | key == 's') //draw a sphere
	{
		drawSphere = !drawSphere;
		glutPostRedisplay();
	}
	if( key == '1') //draw house 1
	{
		house1 = true;
		glutPostRedisplay();
	}
	if( key == '2') //draw house 2
	{
		house1 = false;
		glutPostRedisplay();
	}
	if( key == '+' | key == '=') //zoom in
	{
		bScaleP = true;
		glutPostRedisplay();
	}
	if( key == '-' | key == '_') //zoom out
	{
		bScaleM = true;
		glutPostRedisplay();
	}
	if( key == 't'  | key == 'T') //toggle drawing mode
	{
		if(toggle < 2)
			toggle++;
		else
			toggle=0;
		glutPostRedisplay();
	}
	if( key == 'm' | key == 'M') //toggle materials
	{
		if(iMaterialToggle >22)
			iMaterialToggle=0;
		else
			iMaterialToggle++;
		glutPostRedisplay();
	}
	if( key == 'n' | key == 'N') //toggle materials
	{
		if(iMaterialToggle <1)
			iMaterialToggle=23;
		else
			iMaterialToggle--;
		glutPostRedisplay();
	}

}

/*********************************************
||specialKeys() function
||Purpose: Allows you to use the arrow keys to move
|| 	   the snowflake around the screen
|| 
||Keys:    up arrow rotates the house up
|| 	   down arrow rotates the house down
|| 	   left arrow rotates the house left
|| 	   right arrow rotates the house right
*********************************************/
void specialKeys(int key, int x, int y) 
{
		if(key == GLUT_KEY_UP)
		{
			bRotateYm = true;
			glutPostRedisplay();
		}

		if(key ==  GLUT_KEY_DOWN)
		{
			bRotateYp = true;
			glutPostRedisplay();
		}

		if(key == GLUT_KEY_RIGHT)
		{
			bRotateXp = true;
			glutPostRedisplay();
		}

		if(key == GLUT_KEY_LEFT)
		{
			bRotateXm = true;
			glutPostRedisplay();
		}

} 

/************************************************
|| myinit()
|| Purpose: initialize settings
************************************************/
void myinit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
}

/************************************************
|| enableLighting()
|| Purpose: setup lights for the scene
************************************************/
void enableLighting()
{
    GLfloat diffuse0[]={1.0, 1.0, 1.0, 1.0}; //Light diffuse
    GLfloat ambient0[]={1.0, 1.0, 1.0, 1.0};
    GLfloat ambient2[]={1.0, 1.0, 1.0, 1.0};
    GLfloat specular0[]={1.0,1.0,1.0,1.0};
    GLfloat light0_pos[]={1.0,2.0,13.0,1.0}; //closer top right
    GLfloat light1_pos[]={-1.0,3.0,13.0,1.0}; //back top left
    GLfloat light2_pos[]={-9.0,-8,32.0,1.0}; //middle bottom slight left
    GLfloat light3_pos[]={1.0,1.0,13.0,1.0}; //far back
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING); //enable lighting

    //Light 0
	    glEnable(GL_LIGHT0); //enable light 0
	    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos); //position light 0
	    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0); //ambient light 0
	    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0); //diffuse light 0
	    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0); //specular light 0
    
    //Light 1
	    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos); //position light 1
	    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient0); //ambient light 1
	    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse0); //diffuse light 1
	    glLightfv(GL_LIGHT1, GL_SPECULAR, specular0); //specular light 1
    
    //Light 2
	    glLightfv(GL_LIGHT2, GL_POSITION, light2_pos); //position light 2
	    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2); //ambient light 2
	    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse0); //diffuse light 2
	    glLightfv(GL_LIGHT2, GL_SPECULAR, specular0); //specular light 2

    //Light 3
	    glLightfv(GL_LIGHT3, GL_POSITION, light3_pos); //position light 3
	    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient0); //ambient light 3
	    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse0); //diffuse light 3
	    glLightfv(GL_LIGHT3, GL_SPECULAR, specular0); //specular light 3
}

/************************************************
|| light_menu()
|| Purpose: create a right click menu to toggle lights
************************************************/
void light_menu(int index)
{
	switch(index)
	{
		case 0:
	    		glEnable(GL_LIGHT0); //enable light 0
			break;
		case 1:
	    		glDisable(GL_LIGHT0); //disable light 0
			break;
		case 2:
	    		glEnable(GL_LIGHT1); //enable light 1
			break;
		case 3:
	    		glDisable(GL_LIGHT1); //disable light 1
			break;
		case 4:
	    		glEnable(GL_LIGHT2); //enable light 2
			break;
		case 5:
	    		glDisable(GL_LIGHT2); //disable light 2
			break;
		case 6:
	    		glEnable(GL_LIGHT3); //enable light 3
			break;
		case 7:
	    		glDisable(GL_LIGHT3); //disable light 3
			break;
		default:
			break;
	}

	glutPostRedisplay();
}

/************************************************
|| main_menu()
|| Purpose: handle arguments from main menu
************************************************/
void main_menu(int index)
{
	if(index==4)
		exit(0);
}

/************************************************
|| material_menu()
|| Purpose: toggle material being used with right
|| 	    click menu
************************************************/
void material_menu(int index)
{
	switch(index)
	{
		case 0:
			iMaterialToggle=0;
			break;
		case 1:
			iMaterialToggle=1;
			break;
		case 2:
			iMaterialToggle=2;
			break;
		case 3:
			iMaterialToggle=3;
			break;
		case 4:
			iMaterialToggle=4;
			break;
		case 5:
			iMaterialToggle=5;
			break;
		case 6:
			iMaterialToggle=6;
			break;
		case 7:
			iMaterialToggle=7;
			break;
		case 8:
			iMaterialToggle=8;
			break;
		case 9:
			iMaterialToggle=9;
			break;
		case 10:
			iMaterialToggle=10;
			break;
		case 11:
			iMaterialToggle=11;
			break;
		case 12:
			iMaterialToggle=12;
			break;
		case 13:
			iMaterialToggle=13;
			break;
		case 14:
			iMaterialToggle=14;
			break;
		case 15:
			iMaterialToggle=15;
			break;
		case 16:
			iMaterialToggle=16;
			break;
		case 17:
			iMaterialToggle=17;
			break;
		case 18:
			iMaterialToggle=18;
			break;
		case 19:
			iMaterialToggle=19;
			break;
		case 20:
			iMaterialToggle=20;
			break;
		case 21:
			iMaterialToggle=21;
			break;
		case 22:
			iMaterialToggle=22;
			break;
		case 23:
			iMaterialToggle=23;
			break;
		default:
			iMaterialToggle=0;
			break;
	}
	glutPostRedisplay();

}

/************************************************
|| menus()
|| Purpose: create a right click menu
************************************************/
void menus()
{
    int l_menu, light1, light2, light3, light4, m_menu;

    //Lights
    light1 = glutCreateMenu(light_menu);
    glutAddMenuEntry("On", 0);
    glutAddMenuEntry("Off", 1);

    light2 = glutCreateMenu(light_menu);
    glutAddMenuEntry("On", 2);
    glutAddMenuEntry("Off", 3);

    light3 = glutCreateMenu(light_menu);
    glutAddMenuEntry("On", 4);
    glutAddMenuEntry("Off", 5);

    light4 = glutCreateMenu(light_menu);
    glutAddMenuEntry("On", 6);
    glutAddMenuEntry("Off", 7);

    //Material Menu
    m_menu = glutCreateMenu(material_menu);
    glutAddMenuEntry("emerald",0);
    glutAddMenuEntry("jade",1);
    glutAddMenuEntry("obsidian",2);
    glutAddMenuEntry("pearl",3);
    glutAddMenuEntry("ruby",4);
    glutAddMenuEntry("turquoise",5);
    glutAddMenuEntry("brass",6);
    glutAddMenuEntry("bronze",7);
    glutAddMenuEntry("chrome",8);
    glutAddMenuEntry("copper",9);
    glutAddMenuEntry("gold",10);
    glutAddMenuEntry("silver",11);
    glutAddMenuEntry("black plastic",12);
    glutAddMenuEntry("cyan plastic",13);
    glutAddMenuEntry("green plastic",14);
    glutAddMenuEntry("red plastic",15);
    glutAddMenuEntry("white plastic",16);
    glutAddMenuEntry("yellow plastic",17);
    glutAddMenuEntry("black rubber",18);
    glutAddMenuEntry("cyan rubber",19);
    glutAddMenuEntry("green rubber",20);
    glutAddMenuEntry("red rubber",21);
    glutAddMenuEntry("white rubber",22);
    glutAddMenuEntry("yellow rubber",23);



    //Light Submenu
    l_menu = glutCreateMenu(light_menu);
    glutAddSubMenu("Light 1", light1);
    glutAddSubMenu("Light 2", light2);
    glutAddSubMenu("Light 3", light3);
    glutAddSubMenu("Light 4", light4);

    //Menu
    glutCreateMenu(main_menu);
    glutAddSubMenu("Lights", l_menu);
    glutAddSubMenu("Materials", m_menu);
    glutAddMenuEntry("Quit", 4);

    glutAttachMenu(GLUT_RIGHT_BUTTON);


}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("My House");
    glutReshapeFunc(myReshape); //reshape window function
    glutDisplayFunc(display); //display function
    glutKeyboardFunc(myKey);  //keyboard function
    glutSpecialFunc(specialKeys); //special keys function
    glutMouseFunc(mouseButton); //mouse function
    glutMotionFunc(mouseMotion); //mouse motion function
    glEnable(GL_DEPTH_TEST); 
    enableLighting(); //setup lights
    myinit(); //initialize environment
    menus(); //setup menus
    glutMainLoop();
}


