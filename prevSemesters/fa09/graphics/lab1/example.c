/* a skeleton for a simple OpenGL program
   The functions are:
      display     - code for your object
      reshape     - setup drawing window
      mouse       - clicking a mouse starts/stop spin
      spinDisplay - increments a counter used in rotation 
*/
#include <GLUT/glut.h>
#include <stdlib.h>

static GLfloat spin = 0.0;

void display(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);  // clear to black
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glRotatef(spin, 1.0, 1.0, 0.0);

   /* place code for your lines here */

   glPopMatrix();
   glFlush();
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -50.0, 50.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
/****** code for rotating ******************************/
void spinDisplay (void)
{
  spin += 2.0;
  if (spin > 360.0)
     spin -= 360.0;
  glutPostRedisplay();
}
/****** use the mouse to start and stop spinning  *******/
void mouse(int button, int state, int x, int y)
{
   static int button_state = 0;
   if (state == GLUT_DOWN)
      if (button_state == 0)
        {
         button_state = 1;
         glutIdleFunc(spinDisplay);
        }
      else
        {
         button_state = 0;
         glutIdleFunc(NULL);
        } 
}
/********************************************************/
 
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_RGB);
   glutInitWindowSize (450, 450); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;  
}
