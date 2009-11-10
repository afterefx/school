/*
 * File: vector_ops.h
 *
 * Description: Header file for vector operations.
 */

#ifndef _XFM_H
#define _XFM_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#define  MATRIX_SIZE       4

#define bool int
#define false 0
#define true 1

#define  Pi                M_PI
#define  radiansPerDegree  M_PI/180.0

/*
** this a record structure that contains a 3D vector (x, y, z).
**
** Note - if w == 1 then it represents a point.
**        if w == 0 then it represents a vector.
*/
typedef struct _Vector {
  GLdouble x, y, z, w;
} 
Vector;

typedef struct _Matrix {
  GLdouble element[MATRIX_SIZE][MATRIX_SIZE];
} 
Matrix;

typedef struct _Transform {
  Matrix transformation;
  Matrix inverseTransformation;
} 
Transform;

/* ----- FUNCTION HEADERS -------------------------------------------------- */

Vector   vector_subtract(Vector a, Vector b);
Vector   vector_add(Vector a, Vector b);
GLdouble dot_product(Vector a, Vector b);
void     vector_copy(Vector *a, Vector b);
Vector   vector_X_n(Vector a, GLdouble s);
GLdouble vector_length(Vector a);
Vector   vector_dividedby_n(Vector a, GLdouble s);
Vector   unit_vector(Vector vector);
Vector   matrixTimesVector(Matrix m, Vector v);

Matrix identityMatrix(void);
void loadIdentityMatrix(Matrix *ident);
void makeMatrix(Matrix *mat,
		GLdouble a00, GLdouble a01, GLdouble a02, GLdouble a03,
		GLdouble a10, GLdouble a11, GLdouble a12, GLdouble a13,
		GLdouble a20, GLdouble a21, GLdouble a22, GLdouble a23,
		GLdouble a30, GLdouble a31, GLdouble a32, GLdouble a33);
void addMatrix(Matrix *sum, Matrix adding);
Matrix multiplyMatrix(Matrix A, Matrix B);
void postMultiplyMatrix(Matrix *product, Matrix factor);
void preMultiplyMatrix(Matrix factor, Matrix *product);
Matrix transposeMatrix(Matrix m);
void displayMatrix(Matrix m);

void loadIdentityTransform(Transform *ident);
Transform identityTransform(void);
Transform scaleTransform(GLdouble x, GLdouble y, GLdouble z);
Transform translateTransform(GLdouble x, GLdouble y, GLdouble z);
Transform rotateTransform(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
Transform rotateXTransform(GLdouble angle);
Transform rotateYTransform(GLdouble angle);
Transform rotateZTransform(GLdouble angle);
void applyTransform(Transform factor, Transform *t);
Transform multiplyTransform(Transform factor, Transform t);
void displayTransform(Transform t);


#endif /*  _XFM_H  */


