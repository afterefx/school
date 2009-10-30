#include <stdlib.h>
#include <stdio.h>
#include "Xfm.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

typedef float point[4];

typedef struct {
  GLdouble red, green, blue;
} 
RGBColor;

GLfloat mat_specular[]={0.8, 0.1, 0.1, 0.1};
GLfloat mat_diffuse[]={0.3, 1.0, 0.3, 1.0};
GLfloat mat_ambient[]={1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess={100.0};

int numLights; //keep track of the number of lights
int numObjs; //number of objects
int winWidth, winHeight;

struct {
    int size;
    float d;
 } 
view;

#define MAX_LIGHTS  10
#define MAX_OBJECTS    200
RGBColor ambient_light;
RGBColor backgroundColor = {0,0,0};

typedef struct {
    Vector position;
    RGBColor color;
    GLfloat shininess;
} 
LIGHT;

typedef struct {
    RGBColor diffuse;
    RGBColor specular;
    GLfloat shininess;
} 
MATERIAL;

#define MAX_GROUPS    20

typedef struct {
    MATERIAL material;
    Transform transform;
} 
OBJECT;

MATERIAL currentMaterial; //current material
Transform currentTransform[MAX_GROUPS]; //array of transformations for groups
Transform tempSave;
GLdouble loadAMatrix[16];
int curGroupLevel=0;

OBJECT objects[MAX_OBJECTS]; //array of objects
LIGHT lightSources[MAX_LIGHTS]; //array of lights

/********************************************
// loadColor()
// 
// receives a RGBColor and puts it into a GLfloat
// pointer. 
//
********************************************/
void loadColor(GLfloat *clr, RGBColor iclr) 
{
    clr[0] = iclr.red; //red
    clr[1] = iclr.green; //green
    clr[2] = iclr.blue; //blue
    clr[3] = 1.0; //alpha
}

/*******************************************
// setmaterial()
//
// takes in material struct and breaks it down
// setting each attribute with glMaterial fv
//
*******************************************/
void setmaterial(MATERIAL mat) 
{
    GLfloat clr[4];
    loadColor(clr, mat.specular);
//    printf("setting specular to (%lf,%lf,%lf,%lf)\n", clr[0], clr[1], clr[2], clr[3]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, clr);
    loadColor(clr, mat.diffuse);
 //   printf("setting diffuse to (%lf,%lf,%lf,%lf)\n", clr[0], clr[1], clr[2], clr[3]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, clr);
  //  printf("setting shininess to %lf\n", mat.shininess);

    glMaterialf(GL_FRONT, GL_SHININESS, mat.shininess);
}

void singleArrayMatrix( Matrix mat, GLdouble *m )
{
	int r,c;
	for(r=0; r < 4; r++)
	{
	  for(c=0; c < 4; c++)
	  {
	      m[(r*4)+c] = mat.element[r][c];
	  }
	}
}

void display(void)
{
    int i,r,c,e;
    GLdouble m[16]; //single array matrix to used to load
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // if (trackballMove) { glRotatef(angle, axis[0], axis[1], axis[2]); }
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glPushMatrix();
    for (i = 0; i < numObjs; i++) {

//  What steps do I need to do to make sure the state is correct for this
// object?  And that it doesn't corrupt the state for others???
// Draw each object
	      // Maybe I can store a transform for each object and just load it

	 setmaterial(objects[i].material);
	 singleArrayMatrix( transposeMatrix(objects[i].transform.transformation), m );
	 //displayMatrix( transposeMatrix(objects[i].transform.transformation ));
	 //singleArrayMatrix( objects[i].transform.transformation, m );
	 //displayTransform( objects[i].transform );
	 glLoadMatrixd( m );      
	      // Draw the object using glut function or my own sphere code.
	glutSolidSphere(1,50,50); 
	      // Hmm, anything else need to be done?
    }
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

//don't need to fix this
void myReshape(int w, int h)
{
    //glViewport(0, 0, w/2, h); // Maybe I want to use only one half the window...
    glViewport(0, 0, w, h);

    winWidth = w;
    winHeight = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glFrustum(-view.d, view.d, -view.d * (GLfloat) h / (GLfloat) (w),
            view.d * (GLfloat) h / (GLfloat) (w), 1, 1000);
    else
        glFrustum(-view.d * (GLfloat) (w) / (GLfloat) h, view.d * (GLfloat) (w) / (GLfloat) h, -view.d,
            view.d, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}


void myinit()
{
    int i;
    GLfloat clr[4];
    GLfloat light[4];
    
    glMatrixMode(GL_MODELVIEW);
/* set up ambient, diffuse, and specular components for light each light */

    //printf("Number of lights: %d\n" ,numLights);
    for (i=0; i < numLights; i++) {
    	//printf(" Setting up light: %d\n" ,i);
        glEnable(GL_LIGHT0+i);  /* enable light i */
	if(i==0)
	{
		loadColor(clr, ambient_light);
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, clr);
	}
       	loadColor(clr, lightSources[i].color);
	glLightfv(GL_LIGHT0+i, GL_DIFFUSE, clr);
	glLightfv(GL_LIGHT0+i, GL_SPECULAR, clr);
	light[0] = lightSources[i].position.x;
	light[1] = lightSources[i].position.y;
	light[2] = lightSources[i].position.z;
	light[3] = 1.0;
	glLightfv(GL_LIGHT0+i, GL_POSITION, light);
    }

/* define material proerties for front face of all polygons */

    glShadeModel(GL_SMOOTH); /*enable smooth shading */
    glEnable(GL_LIGHTING);   /* enable lighting */
    glEnable(GL_DEPTH_TEST); /* enable z buffer */
    glEnable(GL_NORMALIZE); // I could have scaling so I'm assuming normals wrong.
    glClearColor (backgroundColor.red, backgroundColor.green,
		    backgroundColor.blue, 1.0);
    glMatrixMode(GL_PROJECTION);
}

void kbdCB(unsigned char key, int x, int y)
{
   switch (key)
   {
       case 'q': case 'Q':
       case 27: exit(0); break;  // Esc
   }
}

void readFile(char *fname)
{
  char filename[1024]; //holds filename
  FILE *SceneFile; //points to the file
  char cmd[512]; //
  char Buff[2048];

  GLdouble x, y, z, angle, ni;
  char axis;
	
  numLights = 0;
  numObjs = 0;

  loadIdentityTransform(&currentTransform[curGroupLevel]);
  if ((SceneFile = fopen(fname, "r")) == NULL) 
  {
      printf("Error opening scene file \n");
      exit(1);
  }
  fscanf(SceneFile, "%s", cmd);  //stores first word into cmd
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
	  // We've got a new group so let's add a new level.
	  // What should our initial transform be?
	  // Let's just start with the parents transform!
          ++curGroupLevel;
	  //printf("Current group level: %d\n", curGroupLevel);
          currentTransform[curGroupLevel] = currentTransform[curGroupLevel-1]; // get transform for previous group level
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
	  // I've got an object.  Should I set all of it's attributes here?
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
      }
      else if (!strcmp(cmd, "move"))
      {
	  fscanf(SceneFile, "%lf", &x);
	  fscanf(SceneFile, "%lf", &y);
	  fscanf(SceneFile, "%lf", &z);

	  //save current matrix off to the side
	  tempSave = currentTransform[curGroupLevel];
	  //set current matrix to identity (glLoadIdentity b/c we can't send in
	  //our current matrix)
	  loadIdentityTransform(&currentTransform[curGroupLevel]);
	  //apply passed in move to identity. so that it's first (reversing
	  //order)
	  applyTransform( translateTransform(x, y, z), &currentTransform[curGroupLevel]);  
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
	  //set current matrix to identity (glLoadIdentity b/c we can't send in
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
	  //set current matrix to identity (glLoadIdentity b/c we can't send in
	  //our current matrix)
	  loadIdentityTransform(&currentTransform[curGroupLevel]);
	  //apply passed in rotate to identity. so that it's first (reversing
	  //order)
	  applyTransform( rotateTransform(angle,x,y,z), &currentTransform[curGroupLevel]); 
	  //now put save matrix on to transform passed in. (reversing order)
	  applyTransform( tempSave, &currentTransform[curGroupLevel]);  
      }
      else
      {
	  fprintf(stderr, "Error: Unknown cmd '%s' in description file\n", cmd);
	  exit(-1);
      }
      fscanf(SceneFile, "%s", cmd);
  }

  fclose(SceneFile); //close the scene file
}
		
int main(int argc, char **argv)
{
    //level=3;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    readFile(argv[1]);
    //glutInitWindowSize(view.size*2, view.size);
    glutInitWindowSize(view.size, view.size);
    glutCreateWindow("ViewScene");
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(kbdCB);
    glutMainLoop();
}

