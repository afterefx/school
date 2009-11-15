#include <stdlib.h>
#include <stdio.h>
#include "Xfm.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*******************************************
// RGBColor struct
// Purpose: holds color values
******************************************/
typedef struct {
  double red, green, blue;
} 
RGBColor;

int numLights; //keep track of the number of lights
int numObjs; //number of objects

/*******************************************
// View struct
// Purpose: holds a size for window and 
// 	    distiance from origin
******************************************/
struct {
    int size;
    GLdouble d;
 } 
view;

#define MAX_LIGHTS  10 //maximum number of lights
#define MAX_OBJECTS    200 //maximum number of objects
RGBColor ambient_light; //ambient light for the scene
RGBColor backgroundColor = {0,0,0}; //background color for the scene

/************************************************
|| LIGHT struct
|| Purpose: holds position, color and shininess
|| 	    for a light
************************************************/
typedef struct {
    Vector position;
    RGBColor color;
    GLdouble shininess;
} 
LIGHT;

/************************************************
|| MATERIAL
|| Purpose: holds diffuse, specular, and shininess
||  	    for a material
************************************************/
typedef struct {
    RGBColor diffuse;
    RGBColor specular;
    GLdouble shininess;
} 
MATERIAL;

#define MAX_GROUPS    20  //maximum number of groups

/************************************************
|| Object strcut
|| Purpose: holds a material and transform for
|| 	   an object
************************************************/
typedef struct {
    MATERIAL material;
    Transform transform;
} 
OBJECT;

/************************************************
|| Ray strcut
|| Purpose: holds a ray's vectors
************************************************/
typedef struct {
	Vector u; //starting point
	Vector v; //direction
}
Ray;

typedef struct {
	int objNumber;
	Vector intPoint;
}
ObjectAttributes;
	
MATERIAL currentMaterial; //current material
Transform currentTransform[MAX_GROUPS]; //array of transformations for groups
Transform tempSave; //holds a tranform temporarily off to the side
int curGroupLevel=0; //holds the current group level

OBJECT objects[MAX_OBJECTS]; //array of objects
LIGHT lightSources[MAX_LIGHTS]; //array of lights

void readFile(char *fname)
{
  char filename[1024]; //holds filename
  FILE *SceneFile; //points to the file
  char cmd[512]; // holds a command
  char Buff[2048];

  double x, y, z, angle, ni;
  char axis;
	
  //Initialize number of lights & objects
  numLights = 0; 
  numObjs = 0;

  //Start reading scene with identity transform
  loadIdentityTransform(&currentTransform[curGroupLevel]);

  //Open the file
  if ((SceneFile = fopen(fname, "r")) == NULL) 
  {
      printf("Error opening scene file \n");
      exit(1);
  }
  fscanf(SceneFile, "%s", cmd);  //stores first word into cmd

  //Loop to read contents of the file
  while (!feof(SceneFile))
  {
      if (!strcmp(cmd, "view")) //compares string to view
      {
	  fscanf(SceneFile, "%d", &view.size);
	  //printf("View size: %d\n", view.size);
	  fscanf(SceneFile, "%lf", &view.d);
	  //printf("View distance to corner of image plane: %f\n", view.d);
      }
      else if (!strncmp(cmd, "#", 1)) 
      {
	  /* Comment, let's ignore! */
	  fgets(Buff, 2048, SceneFile); //takes in till \n is found
      }
      else if (!strcmp(cmd, "group")) 
      {
          ++curGroupLevel;
	  //printf("Current group level: %d\n", curGroupLevel);

	  // get transform for previous group level
          currentTransform[curGroupLevel] = currentTransform[curGroupLevel-1]; 
      }
      else if (!strcmp(cmd, "groupend")) 
      {
	  --curGroupLevel;
	  //printf("Current group level: %d\n", curGroupLevel);
      }
      else if (!strcmp(cmd, "background"))
      {
	  fscanf(SceneFile, "%lf", &backgroundColor.red);
	  fscanf(SceneFile, "%lf", &backgroundColor.green);
	  fscanf(SceneFile, "%lf", &backgroundColor.blue);
      }
      else if (!strcmp(cmd, "ambient"))
      {
	  fscanf(SceneFile, "%lf", &ambient_light.red);
	  fscanf(SceneFile, "%lf", &ambient_light.green);
	  fscanf(SceneFile, "%lf", &ambient_light.blue);
      }
      else if (!strcmp(cmd, "light"))
      {
	  if (numLights>=MAX_LIGHTS)
	  {
	      fprintf(stderr, "Error: max number of lights exceeded in description file\n");
	      exit(-1);
	  }
	  fscanf(SceneFile,"%lf", &lightSources[numLights].color.red);
	  fscanf(SceneFile,"%lf", &lightSources[numLights].color.green);
	  fscanf(SceneFile,"%lf", &lightSources[numLights].color.blue);
	  fscanf(SceneFile,"%lf", &lightSources[numLights].position.x);
	  fscanf(SceneFile,"%lf", &lightSources[numLights].position.y);
	  fscanf(SceneFile,"%lf", &lightSources[numLights].position.z);
	  //printf("Color red: %lf\n", lightSources[numLights].color.red);
	  //printf("Color green: %lf\n", lightSources[numLights].color.green);
	  //printf("Color blue: %lf\n", lightSources[numLights].color.blue);
	  //printf("Position x: %lf\n", lightSources[numLights].position.x);
	  //printf("Position y: %lf\n", lightSources[numLights].position.y);
	  //printf("Position z: %lf\n", lightSources[numLights].position.z);

	  numLights++;
      }
      else if (!strcmp(cmd, "sphere"))
      {
	  objects[numObjs].transform = currentTransform[curGroupLevel];
	  objects[numObjs].material = currentMaterial;
	  numObjs++;
	  if (numObjs>MAX_OBJECTS)
	  {
	      fprintf(stderr, "Error: max number of objects exceeded in description file\n");
	      exit(-1);
	  }
      }
      else if (!strcmp(cmd, "material"))
      {
	  fscanf(SceneFile, "%lf", &currentMaterial.diffuse.red);
	  fscanf(SceneFile, "%lf", &currentMaterial.diffuse.green);
	  fscanf(SceneFile, "%lf", &currentMaterial.diffuse.blue);
	  fscanf(SceneFile, "%lf", &currentMaterial.specular.red);
	  fscanf(SceneFile, "%lf", &currentMaterial.specular.green);
	  fscanf(SceneFile, "%lf", &currentMaterial.specular.blue);
	  fscanf(SceneFile, "%lf", &currentMaterial.shininess);
	  // What do I do with this?
	  // you need to put this into the object
      }
      else if (!strcmp(cmd, "refraction"))
      {
	      //needs to be setup later
      }
      else if (!strcmp(cmd, "move"))
      {
	fscanf(SceneFile, "%lf", &x);
	fscanf(SceneFile, "%lf", &y);
	fscanf(SceneFile, "%lf", &z);

	//save current matrix off to the side
	tempSave = currentTransform[curGroupLevel];

	//set current matrix to identity (loadIdentityTransoform b/c 
	// we can't send in our current matrix)
	loadIdentityTransform(&currentTransform[curGroupLevel]);

	//apply passed in move to identity. so that it's first (reversing
	//order)
	applyTransform(translateTransform(x,y,z),&currentTransform[curGroupLevel]);  

	//now put save matrix on to transform passed in. (reversing order)
	applyTransform( tempSave, &currentTransform[curGroupLevel]); 
      }
      else if (!strcmp(cmd, "scale"))
      {
	//printf("Scale\n");
	fscanf(SceneFile, "%lf", &x);
	fscanf(SceneFile, "%lf", &y);
	fscanf(SceneFile, "%lf", &z);

	//save current matrix off to the side
	tempSave = currentTransform [curGroupLevel];

	//set current matrix to identity (loadIdentTransform b/c we can't send in
	//our current matrix)
	loadIdentityTransform(&currentTransform[curGroupLevel]);
	//apply passed in scale to identity. so that it's first (reversing
	//order)
        applyTransform( scaleTransform(x, y, z), &currentTransform[curGroupLevel]); 

	//now put save matrix on to transform passed in. (reversing order)
	applyTransform( tempSave, &currentTransform[curGroupLevel]);  
      }
      else if (!strcmp(cmd, "rotate"))
      {
	fscanf(SceneFile, "%lf", &angle);
	fscanf(SceneFile, "%lf", &x);
	fscanf(SceneFile, "%lf", &y);
	fscanf(SceneFile, "%lf", &z);

	//save current matrix off to the side
	tempSave = currentTransform[curGroupLevel];

	//set current matrix to identity (loadIdentTransform b/c we can't send in
	//our current matrix)
	loadIdentityTransform(&currentTransform[curGroupLevel]);

	//apply passed in rotate to identity. so that it's first (reversing
	//order)
	applyTransform(rotateTransform(angle,x,y,z), &currentTransform[curGroupLevel]); 
	//now put save matrix on to transform passed in. (reversing order)
	applyTransform( tempSave, &currentTransform[curGroupLevel]);  
      }
      else
      {
	  fprintf(stderr, "Error: Unknown cmd '%s' in description file\n", cmd);
	  exit(-1);
      }
      fscanf(SceneFile, "%s", cmd); //get next command
  }

  fclose(SceneFile); //close the scene file
}

Ray ray_from_eye_through(Vector curPixel)
{
	Ray tempRay;

	//Camera
	tempRay.u.x = 0;
	tempRay.u.y = 0;
	tempRay.u.z = 1;
	tempRay.u.w = 1;

	tempRay.v=vector_subtract(curPixel , tempRay.u);
	printf("Vector\nX: %f, Y: %f, Z: %f\n", tempRay.v.x, tempRay.v.y, tempRay.v.z);
	tempRay.v=unit_vector(vector_subtract(curPixel , tempRay.u));
	printf("\nUnit Vector\nX: %f, Y: %f, Z: %f\n", tempRay.v.x, tempRay.v.y, tempRay.v.z);
	//printf("\nX: %f, Y: %f, Z: %f\n", tempRay.v.x, tempRay.v.y, tempRay.v.z);
	return tempRay;
}

RGBColor Shade(ObjectAttributes obj, Ray ray)
{
	RGBColor color; 
	int iNum;

	//store the object number
	iNum = obj.objNumber;



	//Color = black
	color.red = ambient_light.red * objects[iNum].material.diffuse.red;
	color.green = ambient_light.green * objects[iNum].material.diffuse.green;
	color.blue = ambient_light.blue * objects[iNum].material.diffuse.blue;

	return color;

}

Ray moveRay(Ray tempRay, Matrix inverse)
{
	//inverse = transposeMatrix(inverse);

	tempRay.u = matrixTimesVector(inverse, tempRay.u);
	tempRay.v = matrixTimesVector(inverse, tempRay.v);

	return tempRay;
}

ObjectAttributes closest_intersection(Ray ray)
{
	int i, smallestObj;
	Ray currRay;
	ObjectAttributes obj; //object to return values
	double a, //a for quad equation
	       b, //b for quad equation
	       c, //c for quad equation
	       t, //smallest t for current obj
	       t1, //t1 for current obj
	       t2, //t2 for current obj
	       smallestT=9000000.0; //smallest t


	obj.objNumber = -1;
	//For each object
	for(i=0; i < numObjs; i++)
	{

		//Move current ray with inverse of current object 
		//to see if it intersects with origin object
		currRay = moveRay(ray, objects[i].transform.inverseTransformation);

		//store values for a b c
		a = dot_product(currRay.v, currRay.v);
		b = 2 * dot_product(currRay.u,currRay.v);
		c = dot_product(currRay.u,currRay.u) - 1;

		printf("A: %lf, B: %lf, C: %lf\n",a,b,c);

		t=0;
		//Calculate t
		if(b*b > 4*a*c)
		{
			printf("Checking for hit\n");
			if(b >0)
			{
				printf("Greater than 0\n");
				t1 = (-b - sqrt((b*b) - 4*a*c))/(2*a);
			}
			else
				printf("Less than 0\n");
				t1 = (-b + sqrt((b*b) - 4*a*c))/(2*a);

			t2 = c / (a*t1);

			//Find smallest t for current obj
			if(t1 < t2 && t1 > 0)
			{
				t = t1;
			}
			if(t2 < t1 && t2 > 0)
			{
				t = t2;
			}

			printf("t1 is: %lf\n", t1);
			printf("t2 is: %lf\n", t2);
			printf("t is: %lf\n", t);

			//If smallest obj keep track of it
			if(t < smallestT && t > 0)
			{
				printf("Object #: %d\n", i+1);


				smallestT = t;
				printf("Smallest t is: %lf\n", smallestT);
				obj.objNumber = i;

			}
		}

	}//done with each object

	//Calculate point of intersect
	obj.intPoint = vector_add(ray.u, vector_X_n(ray.v, smallestT));
		
	//return object attributes
	return obj;  //obj # with smallest t &  the point

}

//Trace one pixel
RGBColor trace(Ray ray)
{
	ObjectAttributes obj;
	obj = closest_intersection(ray); //get nearest obj location & #

	printf("Object # for if is: %d\n", obj.objNumber);
	if(obj.objNumber != -1)
	{
		printf("Return color");
		return Shade(obj, ray);
	}
	else
	{
		printf("Return BG\n");
		return backgroundColor;
	}
}

void drawPixels(RGBColor Pixel[][view.size])
{
	FILE *picfile;
	int j,i;
	picfile = fopen("out.ppm", "w");
	fprintf(picfile, "P6\n%d %d\n255\n", view.size, view.size);

        // For each pixel
	for (j=view.size; j >= 0; j--)     // Y is flipped!
	    for (i=0; i < view.size; i++) {
	        fprintf(picfile, "%c%c%c", 
				(int) Pixel[i][j].red*255,(int)
				Pixel[i][j].green*255,(int) Pixel[i][j].blue*255);
		    // Remember though that this is a number between 0 and 255
		    // so might have to convert from 0-1.
	    }



	for (j=view.size; j >= 0; j--)     // Y is flipped!
	{
		printf("Row %d\n",i);
	    for (i=0; i < view.size; i++) {
		printf("Col %d\n",j);
	        printf("Red: %lf, Green: %lf, Blue: %lf\n", 
				Pixel[i][j].red,Pixel[i][j].green,Pixel[i][j].blue);

		  glBegin(GL_POINTS);
		  glColor3f(Pixel[i][j].red, Pixel[i][j].green, Pixel[i][j].blue);
		  glVertex2f(i, j);
		  glEnd();
	    }
	}
	

}	


void RayCast()
{
    	RGBColor Pixel[view.size][view.size]; //keep track of colors
	int row,col;
	GLdouble d,n;
	RGBColor color;
	Vector p,  //point of intersection 
	       curPixel; //current pixel being calculated


	d = view.d; 
	n = view.size; //number of pixels
	
	//For each pixel
    	printf("Entering pixel rows\n");
	for(row=0; row < view.size; row++)
	{
		printf("\n\nEntering row %d\n", row);
		for(col=0; col < view.size; col++)
		{
    			printf("\nEntering col %d\n", col);

			//Finding the center of the current pixel
    			printf("Finding x ");
			curPixel.x = -d + (d/n) + (((2*d)/n) * row);
    			printf("Finding y ");
			curPixel.y = - d + (d/n) + (((2*d)/n) * col);
    			printf("Placing z ");
			curPixel.z = 0;
    			printf("Placing w ");
			curPixel.w = 1;

			//Find color and store it
    			printf("Entering trace\n");
			color=	trace(ray_from_eye_through(curPixel));

			Pixel[row][col].red = color.red;
			Pixel[row][col].green = color.green;
			Pixel[row][col].blue = color.blue;
		}
	}
     drawPixels(Pixel); //draw the results
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       	glMatrixMode(GL_MODELVIEW);
       	glLoadIdentity();
       	glFlush();
       	glColor3f(0,0,0);

       	printf("Casting rays\n");
       	RayCast(); //start finding points
	printf("Number of Objects: %d\n", numObjs);
       	glutSwapBuffers();
}

void myinit()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPointSize(1.0);
	gluOrtho2D(0, view.size, 0, view.size);
	glClearColor(0.0,0.0,0.0,0.0);

}

int main(int argc, char **argv)
{
	//Read file in
	printf("Reading file\n");
       	readFile(argv[1]); //read the scene

	//Get openGL display of results ready
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(view.size, view.size);
	glutCreateWindow("ray tracer");
	myinit(); //setup lights and window

    glutDisplayFunc(display); //start drawing
    glutMainLoop(); //start looping
}

