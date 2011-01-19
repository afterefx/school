/*
 * Author:  Iain Hewson   (2000)
 * Updated: Raymond Scurr (2001)
 *
 */


#include <stdio.h>

#include "Xfm.h"

#define  sqr(a)            ((a)*(a))

/*
**  makeVector
*/
Vector makeVector(GLdouble a, GLdouble b, GLdouble c, GLdouble d)
{
  Vector result;
  
  result.x = a;
  result.y = b;
  result.z = c;
  result.w = (d==0) ? 0 : 1;

  return(result);
}

/*-------------------+
**  vector_subtract  |
**-------------------+
** subtract two vectors: return a-b
*/
Vector vector_subtract(Vector a, Vector b)
{
  return makeVector(a.x-b.x, a.y-b.y, a.z-b.z, 0);
}


/*--------------
**  vector_add  |
**--------------
** return the vector produced by adding two vectors
*/
Vector vector_add(Vector a, Vector b)
{
  return makeVector(a.x+b.x, a.y+b.y, a.z+b.z, 0);
}


/*--------------
**  vector_X_n  |
**--------------
** return the vector multiplied by a scalar
*/
Vector vector_X_n(Vector a, GLdouble s)
{
  return makeVector(s * a.x, s * a.y, s * a.z, a.w);
}


/*----------------------
**  vector_dividedby_n  |
**----------------------
** divide a vector by a scalar
*/
Vector vector_dividedby_n(Vector a, GLdouble s)
{
  return makeVector(a.x / s, a.y / s, a.z / s, a.w);
}


/*---------------+
**  dot_product  |
**---------------+
** return the dot product of two vectors
*/
GLdouble dot_product(Vector a, Vector b)
{
  return((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}


/*---------------
**  unit_vector  |
**---------------
** return a vector divided by its length
** i.e. a vector of length 1 (a unit vector)
*/
Vector unit_vector(Vector v)
{
  // return(vector_dividedby_n(v, vector_length(v)));
  // Let's do it a bit faster
    GLdouble oneOverl = 1.0/vector_length(v);

    return (vector_X_n(v, oneOverl));
}


/*-----------------
**  vector_length  |
**-----------------
** return the length of a vector
*/
GLdouble vector_length(Vector a)
{
  return (sqrt(sqr(a.x) + sqr(a.y) + sqr(a.z)));
}


Vector matrixTimesVector(Matrix m, Vector v)
{
  Vector result;
  
  result.x = m.element[0][0] * v.x + m.element[0][1] * v.y
           + m.element[0][2] * v.z + m.element[0][3] * v.w;
  result.y = m.element[1][0] * v.x + m.element[1][1] * v.y
           + m.element[1][2] * v.z + m.element[1][3] * v.w;
  result.z = m.element[2][0] * v.x + m.element[2][1] * v.y
           + m.element[2][2] * v.z + m.element[2][3] * v.w;
  result.w = m.element[3][0] * v.x + m.element[3][1] * v.y
           + m.element[3][2] * v.z + m.element[3][3] * v.w;

  return result;
}




/*
** FUNCTIONS
*/

Matrix identityMatrix(void)
{
    Matrix result = { {1, 0, 0, 0,
                       0, 1, 0, 0,
  		       0, 0, 1, 0,
         	       0, 0, 0, 1 } };
  /*
  int row, col;

  for (row = 0; row < MATRIX_SIZE; row++)
    for (col = 0; col < MATRIX_SIZE; col++)
      result.element[row][col] = (row == col);
  */
  return result;
}

void loadIdentityMatrix(Matrix *ident)
{
  (*ident) = identityMatrix();
}

void makeMatrix(Matrix *mat,
		GLdouble a00, GLdouble a01, GLdouble a02, GLdouble a03,
		GLdouble a10, GLdouble a11, GLdouble a12, GLdouble a13,
		GLdouble a20, GLdouble a21, GLdouble a22, GLdouble a23,
		GLdouble a30, GLdouble a31, GLdouble a32, GLdouble a33)
{
  (*mat).element[0][0] = a00;
  (*mat).element[0][1] = a01;
  (*mat).element[0][2] = a02;
  (*mat).element[0][3] = a03;

  (*mat).element[1][0] = a10;
  (*mat).element[1][1] = a11;
  (*mat).element[1][2] = a12;
  (*mat).element[1][3] = a13;

  (*mat).element[2][0] = a20;
  (*mat).element[2][1] = a21;
  (*mat).element[2][2] = a22;
  (*mat).element[2][3] = a23;

  (*mat).element[3][0] = a30;
  (*mat).element[3][1] = a31;
  (*mat).element[3][2] = a32;
  (*mat).element[3][3] = a33;
}

void addMatrix(Matrix *sum, Matrix adding)
{
   int row, col;

  for (row = 0; row < MATRIX_SIZE; row++)
    for (col = 0; col < MATRIX_SIZE; col++)
      (*sum).element[row][col] += adding.element[row][col];
}

/*
** Multiplication functions
*/

Matrix multiplyMatrix(Matrix A, Matrix B)
{
  int row, col, scan;
  Matrix result;

  for (row = 0; row < MATRIX_SIZE; row++)
    for (col = 0; col < MATRIX_SIZE; col++)
    {
      result.element[row][col] = 0;
      for (scan = 0; scan < MATRIX_SIZE; scan++)
	result.element[row][col] += A.element[row][scan] * B.element[scan][col];
    }
  return result;
}

void preMultiplyMatrix(Matrix factor, Matrix *product)
{
  (*product) = multiplyMatrix(factor, *product);
}

void postMultiplyMatrix(Matrix *product, Matrix factor)
{
  (*product) = multiplyMatrix(*product, factor);
}


/*
**  Take the transpose of a matrix
*/
Matrix transposeMatrix(Matrix m)
{
  Matrix result;
  int row, col;

  for (row = 0; row < MATRIX_SIZE; row++)
    for (col = 0; col < MATRIX_SIZE; col++)
    {
      result.element[row][col] = m.element[col][row];
    }

  return result;
}

/*
**  Output Matrix - for testing and diagnostics
*/
void displayMatrix(Matrix m)
{
  int row, col;

  printf("\n");
  for (row = 0; row < MATRIX_SIZE; row++)
  {
    for (col = 0; col < MATRIX_SIZE; col++)
      printf("%2.8f ", m.element[row][col]);
    printf("\n");
  }
  printf("\n");
}



/*
**  Identity functions
*/
void loadIdentityTransform(Transform *ident)
{
  (*ident) = identityTransform();
}

Transform identityTransform(void)
{
  return scaleTransform(1, 1, 1);
}

/*
**  Scaling functions
*/
Transform scaleTransform(GLdouble x, GLdouble y, GLdouble z)
{
  Transform result;

  if (x*y*z == 0) return identityTransform();  // Don't want divide by zero
           // Bascially the inverse can't be found this could be an error
	   // or just ignored as done here!

  makeMatrix(& result.transformation,
	     x, 0, 0, 0,
	     0, y, 0, 0,
	     0, 0, z, 0,
	     0, 0, 0, 1);

  makeMatrix(& result.inverseTransformation,
	     1/x,   0,   0, 0,
	       0, 1/y,   0, 0,
	       0,   0, 1/z, 0,
	       0,   0,   0, 1);

  return result;
}

/*
**  Translation functions
*/
Transform translateTransform(GLdouble x, GLdouble y, GLdouble z)
{
  Transform result;
   makeMatrix(& result.transformation,
	     1, 0, 0, x,
	     0, 1, 0, y,
	     0, 0, 1, z,
	     0, 0, 0, 1);

  makeMatrix(& result.inverseTransformation,
	     1, 0, 0, -x,
	     0, 1, 0, -y,
	     0, 0, 1, -z,
	     0, 0, 0, 1);

  return result;
}
  

/*
**  Rotation transformations
*/

Transform rotateTransform(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
  Transform result;
  GLdouble c = cos(angle*radiansPerDegree);
  GLdouble s = sin(angle*radiansPerDegree);

  GLdouble length = sqrt(x*x + y*y + z*z);

  //a simplistic error tzest  :(
  if (length == 0) return identityTransform();

  // convert to unit vector
  x /= length;
  y /= length;
  z /= length;

  //start filling the transform
  makeMatrix(& result.transformation,
	       x*x+c-x*x*c,  x*y-x*y*c-z*s,  x*z-x*z*c+y*s, 0,
	     x*y-x*y*c+z*s,    y*y+c-y*y*c,  y*z-y*z*c-x*s, 0,
	     x*z-x*z*c-y*s,  y*z-y*z*c+x*s,    z*z+c-z*z*c, 0,
	                 0,              0,              0, 1);

  makeMatrix(& result.inverseTransformation,
	       x*x+c-x*x*c,  x*y-x*y*c+z*s,  x*z-x*z*c-y*s, 0,
	     x*y-x*y*c-z*s,    y*y+c-y*y*c,  y*z-y*z*c+x*s, 0,
	     x*z-x*z*c+y*s,  y*z-y*z*c-x*s,    z*z+c-z*z*c, 0,
	                 0,              0,              0, 1);

  return result;
}

//Rotate around the X-axis (clockwise)
Transform rotateXTransform(GLdouble angle)
{
  Transform result;
  GLdouble c = cos(angle*radiansPerDegree);
  GLdouble s = sin(angle*radiansPerDegree);

  makeMatrix(& result.transformation,
	     1,  0,  0, 0,
	     0,  c, -s, 0,
	     0,  s,  c, 0,
	     0,  0,  0, 1);

  makeMatrix(& result.inverseTransformation,
	     1,  0,  0, 0,
	     0,  c,  s, 0,
	     0, -s,  c, 0,
	     0,  0,  0, 1);

  return result;
}

//Rotate around the Y-axis (clockwise)
Transform rotateYTransform(GLdouble angle)
{
  Transform result;
  GLdouble c = cos(angle*radiansPerDegree);
  GLdouble s = sin(angle*radiansPerDegree);

  makeMatrix(& result.transformation,
	     c,  0,  s, 0,
	     0,  1,  0, 0,
	     -s, 0,  c, 0,
	     0,  0,  0, 1);

  makeMatrix(& result.inverseTransformation,
	     c,  0, -s, 0,
	     0,  1,  0, 0,
	     s,  0,  c, 0,
	     0,  0,  0, 1);

  return result;
}

//Rotate around the Z-axis (clockwise)
Transform rotateZTransform(GLdouble angle)
{
  Transform result;
  GLdouble c = cos(angle*radiansPerDegree);
  GLdouble s = sin(angle*radiansPerDegree);

  makeMatrix(& result.transformation,
	     c, -s,  0, 0,
	     s,  c,  0, 0,
	     0,  0,  1, 0,
	     0,  0,  0, 1);

  makeMatrix(& result.inverseTransformation,
	     c,  s,  0, 0,
	     -s, c,  0, 0,
	     0,  0,  1, 0,
	     0,  0,  0, 1);


  return result;
}

/*
**  Compose two transformations
*/
void applyTransform(Transform factor, Transform *t)
{
  (*t) = multiplyTransform(factor, *t);
}

Transform multiplyTransform(Transform factor, Transform t)
{
  preMultiplyMatrix(factor.transformation, & (t.transformation));
  postMultiplyMatrix(& (t.inverseTransformation),
                      factor.inverseTransformation);

  return t;
}

/*
**  Output transformations (for testing && diagnosis)
*/
void displayTransform(Transform t)
{
  printf("\nTransform\n");
  displayMatrix(t.transformation);
  printf("Inverse Transform\n");
  displayMatrix(t.inverseTransformation);
}
