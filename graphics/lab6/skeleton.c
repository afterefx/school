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
    float d;
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
    float shininess;
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
    float shininess;
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

MATERIAL currentMaterial; //current material
Transform currentTransform[MAX_GROUPS]; //array of transformations for groups
Transform tempSave; //holds a tranform temporarily off to the side
int curGroupLevel=0; //holds the current group level
RGBColor Pixel[winWidth][winHeight];

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
	  fscanf(SceneFile, "%f", &view.d);
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
	  fscanf(SceneFile, "%f", &currentMaterial.shininess);
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
		
void findIntersections()
{
	int t1,t2,a,b,c,row,col;
	Vector u,v, curPixel;

	//get u  which is the camera position
	u.x = 0;
	u.y = 0;
	u.z = 1;
	u.w = 1;

	//find coord for current pixel
	for(row=0; row < view.d; row++)
		for(col=0; col < view.d; col++)
		{
			curPixel.x = -d + (d/n) + ((2d/n) * col);
			curPixel.y = d - (d/n) - ((2d/n) * row);
			curPixel.z = 0;
			curPixel.w = 1;


	
			//find v
			v=unit_vector(vector_subtract(curPixel - u));

			//store values for a b c
			a = dot_product(u, u);
			b = 2 * dot_product(u,v);
			c = dot_product(u,u) -1;

			//Ray/Sphere intersection
			if(b*b > 4*a*c)
			{
				if(b >0)
					t1 = (-b - sqrt(b^2 - 4*a*c))/(2*a);
				else
					t1 = (-b + sqrt(b^2 - 4*a*c))/(2*a);
				t2 = c / (a*t1);
			}
		}
}

void drawPixels()
{
	int winWidth = view.size;
	int winHeight = view.size;
	FILE *picfile;
	int j,i;
	picfile = fopen("out.ppm", "w");
	fprintf(picfile, "P6\n# %dx%d Raytracer output\n%d %d\n255\n",
                view.size, view.size, view.size, view.size);

        // For each pixel
	for (j=view.size; j >= 0; j--)     // Y is flipped!
	    for (i=0; i < view.size; i++) 
	    {
	        fprintf(picfile, "%lf%lf%lf", 
				Pixel[i][j].red,Pixel[i][j].green,Pixel[i][j].blue);
		    // Remember though that this is a number between 0 and 255
		    // so might have to convert from 0-1.
	    }
}	

int main(int argc, char **argv)
{
    readFile(argv[1]); //read the scene
    findIntersections(); //find the intersections
     drawPixels(); //draw the results
}

