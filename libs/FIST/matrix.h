/*****************************************************************************/
/*                                                                           */
/*                              W A R N I N G                                */
/*                                                                           */
/* This code comes for free, and neither the author nor his employer accept  */
/* any responsibility, to the extent permitted by applicable law, to anyone  */
/* for the consequences of using it or for whether it serves any particular  */
/* purpose or works at all! By using the code you operate on your own risk!  */
/*                                                                           */
/*****************************************************************************/
 
/*****************************************************************************/
/*                                                                           */
/* This code was derived from C code by J.M. Cychosz, Graphics Gems I, 1990. */
/*                                                                           */
/*****************************************************************************/
 
/*****************************************************************************/
/*                                                                           */
/* Code for matrix operations. Note that all matrices are assumed to be      */
/* matrices of rigid motions. That is, the fourth row of the matrices is     */
/* ignored, and this code cannot be used for manipulating matrices of        */
/* projections!                                                              */
/*                                                                           */
/*                                                                           */
/* Written by:  Martin Held                                                  */
/* Date:        Dec 22, 1996                                                 */
/* Modified:    Jan 06, 1998 (matrix multiplication added)                   */
/*                                                                           */
/* E-Mail:      held@ams.sunysb.edu                                          */
/* Fax Mail:    (+516) 632-8490                                              */
/* Voice Mail:  (+516) 632-8362                                              */
/* Snail Mail:  Martin Held                                                  */
/*              Department of Applied Mathematics                            */
/*              State University of New York at Stony Brook                  */
/*              Stony Brook, NY 11794-3600, USA                              */
/*                                                                           */
/*****************************************************************************/
 
typedef struct {
   vertex tr;
   vertex rot;
} transformation;

typedef struct { 
   double element[4][4];
} Matrix4;

#ifndef _MATRIX_H_
#define _MATRIX_H_
/*                                                                           */
/* the following variables are static variables, used only within this file. */
/* unfortunately, when defined as "static", gcc barks that some variable     */
/* has been defined but is not used. without the "static" definition, cc on  */
/* SGIs complains that variables are multiply defined...                     */
/*                                                                           */
extern double matrix_h_sine, matrix_h_cosine;
extern double matrix_h_local;

#endif





#define RotateX(matrix, a) \
{ \
  matrix_h_sine   = sin(a); \
  matrix_h_cosine = cos(a); \
  matrix_h_local  = (matrix).element[0][1]; \
  (matrix).element[0][1] = matrix_h_local * matrix_h_cosine - \
                         (matrix).element[0][2] * matrix_h_sine; \
  (matrix).element[0][2] = (matrix).element[0][2] * matrix_h_cosine + \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local  = (matrix).element[1][1]; \
  (matrix).element[1][1] = matrix_h_local * matrix_h_cosine - \
                         (matrix).element[1][2] * matrix_h_sine; \
  (matrix).element[1][2] = (matrix).element[1][2] * matrix_h_cosine + \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local  = (matrix).element[2][1]; \
  (matrix).element[2][1] = matrix_h_local * matrix_h_cosine - \
                         (matrix).element[2][2] * matrix_h_sine; \
  (matrix).element[2][2] = (matrix).element[2][2] * matrix_h_cosine + \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local  = (matrix).element[3][1]; \
  (matrix).element[3][1] = matrix_h_local * matrix_h_cosine - \
                         (matrix).element[3][2] * matrix_h_sine; \
  (matrix).element[3][2] = (matrix).element[3][2] * matrix_h_cosine + \
                         matrix_h_local * matrix_h_sine; \
}


#define RotateY(matrix, a) \
{ \
  matrix_h_sine   = sin(a); \
  matrix_h_cosine = cos(a); \
  matrix_h_local  = (matrix).element[0][0]; \
  (matrix).element[0][0] = matrix_h_local * matrix_h_cosine + \
                         (matrix).element[0][2] * matrix_h_sine; \
  (matrix).element[0][2] = (matrix).element[0][2] * matrix_h_cosine - \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local = (matrix).element[1][0]; \
  (matrix).element[1][0] = matrix_h_local * matrix_h_cosine + \
                         (matrix).element[1][2] * matrix_h_sine; \
  (matrix).element[1][2] = (matrix).element[1][2] * matrix_h_cosine - \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local = (matrix).element[2][0]; \
  (matrix).element[2][0] = matrix_h_local * matrix_h_cosine + \
                         (matrix).element[2][2] * matrix_h_sine; \
  (matrix).element[2][2] = (matrix).element[2][2] * matrix_h_cosine - \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local = (matrix).element[3][0]; \
  (matrix).element[3][0] = matrix_h_local * matrix_h_cosine + \
                         (matrix).element[3][2] * matrix_h_sine; \
  (matrix).element[3][2] = (matrix).element[3][2] * matrix_h_cosine - \
                         matrix_h_local * matrix_h_sine; \
}


#define RotateZ(matrix, a) \
{ \
  matrix_h_sine   = sin(a); \
  matrix_h_cosine = cos(a); \
  matrix_h_local  = (matrix).element[0][0]; \
  (matrix).element[0][0] = matrix_h_local * matrix_h_cosine - \
                         (matrix).element[0][1] * matrix_h_sine; \
  (matrix).element[0][1] = (matrix).element[0][1] * matrix_h_cosine + \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local  = (matrix).element[1][0]; \
  (matrix).element[1][0] = matrix_h_local * matrix_h_cosine - \
                         (matrix).element[1][1] * matrix_h_sine; \
  (matrix).element[1][1] = (matrix).element[1][1] * matrix_h_cosine + \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local  = (matrix).element[2][0]; \
  (matrix).element[2][0] = matrix_h_local * matrix_h_cosine - \
                         (matrix).element[2][1] * matrix_h_sine; \
  (matrix).element[2][1] = (matrix).element[2][1] * matrix_h_cosine + \
                         matrix_h_local * matrix_h_sine; \
  matrix_h_local  = (matrix).element[3][0]; \
  (matrix).element[3][0] = matrix_h_local * matrix_h_cosine - \
                         (matrix).element[3][1] * matrix_h_sine; \
  (matrix).element[3][1] = (matrix).element[3][1] * matrix_h_cosine + \
                         matrix_h_local * matrix_h_sine; \
}


#define ScaleXYZ(matrix, sx, sy, sz) \
{ \
  (matrix).element[0][0] *= sx; \
  (matrix).element[0][1] *= sy; \
  (matrix).element[0][2] *= sz; \
  (matrix).element[1][0] *= sx; \
  (matrix).element[1][1] *= sy; \
  (matrix).element[1][2] *= sz; \
  (matrix).element[2][0] *= sx; \
  (matrix).element[2][1] *= sy; \
  (matrix).element[2][2] *= sz; \
  (matrix).element[3][0] *= sx; \
  (matrix).element[3][1] *= sy; \
  (matrix).element[3][2] *= sz; \
}


#define TranslateXYZ(matrix, tx, ty, tz) \
{ \
  (matrix).element[3][0] += tx; \
  (matrix).element[3][1] += ty; \
  (matrix).element[3][2] += tz; \
}


#define InitIdentity(matrix) \
{ \
  (matrix).element[0][1] = (matrix).element[0][2] = (matrix).element[0][3] = 0.0; \
  (matrix).element[1][0] = (matrix).element[1][2] = (matrix).element[1][3] = 0.0; \
  (matrix).element[2][0] = (matrix).element[2][1] = (matrix).element[2][3] = 0.0; \
  (matrix).element[3][0] = (matrix).element[3][1] = (matrix).element[3][2] = 0.0; \
  (matrix).element[0][0] = (matrix).element[1][1] = (matrix).element[2][2] = 1.0; \
  (matrix).element[3][3] = 1.0; \
}


/* note:  pnt  and  new_pnt  must not be the same memory cell! */
#define MatrixTransformation(matrix, pnt, new_pnt) \
{ \
  (new_pnt).x = (pnt).x * (matrix).element[0][0] \
              + (pnt).y * (matrix).element[1][0] \
              + (pnt).z * (matrix).element[2][0] \
              +           (matrix).element[3][0]; \
  (new_pnt).y = (pnt).x * (matrix).element[0][1] \
              + (pnt).y * (matrix).element[1][1] \
              + (pnt).z * (matrix).element[2][1] \
              +           (matrix).element[3][1]; \
  (new_pnt).z = (pnt).x * (matrix).element[0][2] \
              + (pnt).y * (matrix).element[1][2] \
              + (pnt).z * (matrix).element[2][2] \
              +           (matrix).element[3][2]; \
}


/* note:  pnt  and  new_pnt  must not be the same memory cell! */
#define MatrixRotation(matrix, pnt, new_pnt) \
{ \
  (new_pnt).x = (pnt).x * (matrix).element[0][0] \
              + (pnt).y * (matrix).element[1][0] \
              + (pnt).z * (matrix).element[2][0]; \
  (new_pnt).y = (pnt).x * (matrix).element[0][1] \
              + (pnt).y * (matrix).element[1][1] \
              + (pnt).z * (matrix).element[2][1]; \
  (new_pnt).z = (pnt).x * (matrix).element[0][2] \
              + (pnt).y * (matrix).element[1][2] \
              + (pnt).z * (matrix).element[2][2]; \
}



#define GetTransformationMatrix(matrix, trans, reverse) \
{ \
  InitIdentity(matrix); \
  if (reverse)  { \
      TranslateXYZ(matrix, trans.tr.x, trans.tr.y, trans.tr.z); \
      RotateZ(matrix, trans.rot.z); \
      RotateY(matrix, trans.rot.y); \
      RotateX(matrix, trans.rot.x); \
   } \
   else  { \
      RotateX(matrix, trans.rot.x); \
      RotateY(matrix, trans.rot.y); \
      RotateZ(matrix, trans.rot.z); \
      TranslateXYZ(matrix, trans.tr.x, trans.tr.y, trans.tr.z); \
   } \
}



/* computes  mat1 * mat2. note:  mat1, mat2, mat3 must point to different    */
/* memory cells!                                                             */
#define MatrixMultiplication(mat1, mat2, mat3) \
{ \
  (mat3).element[0][0] = (mat1).element[0][0] * (mat2).element[0][0] \
                       + (mat1).element[1][0] * (mat2).element[0][1] \
                       + (mat1).element[2][0] * (mat2).element[0][2]; \
  (mat3).element[0][1] = (mat1).element[0][1] * (mat2).element[0][0] \
                       + (mat1).element[1][1] * (mat2).element[0][1] \
                       + (mat1).element[2][1] * (mat2).element[0][2]; \
  (mat3).element[0][2] = (mat1).element[0][2] * (mat2).element[0][0] \
                       + (mat1).element[1][2] * (mat2).element[0][1] \
                       + (mat1).element[2][2] * (mat2).element[0][2]; \
  \
  (mat3).element[1][0] = (mat1).element[0][0] * (mat2).element[1][0] \
                       + (mat1).element[1][0] * (mat2).element[1][1] \
                       + (mat1).element[2][0] * (mat2).element[1][2]; \
  (mat3).element[1][1] = (mat1).element[0][1] * (mat2).element[1][0] \
                       + (mat1).element[1][1] * (mat2).element[1][1] \
                       + (mat1).element[2][1] * (mat2).element[1][2]; \
  (mat3).element[1][2] = (mat1).element[0][2] * (mat2).element[1][0] \
                       + (mat1).element[1][2] * (mat2).element[1][1] \
                       + (mat1).element[2][2] * (mat2).element[1][2]; \
  \
  (mat3).element[2][0] = (mat1).element[0][0] * (mat2).element[2][0] \
                       + (mat1).element[1][0] * (mat2).element[2][1] \
                       + (mat1).element[2][0] * (mat2).element[2][2]; \
  (mat3).element[2][1] = (mat1).element[0][1] * (mat2).element[2][0] \
                       + (mat1).element[1][1] * (mat2).element[2][1] \
                       + (mat1).element[2][1] * (mat2).element[2][2]; \
  (mat3).element[2][2] = (mat1).element[0][2] * (mat2).element[2][0] \
                       + (mat1).element[1][2] * (mat2).element[2][1] \
                       + (mat1).element[2][2] * (mat2).element[2][2]; \
  \
  (mat3).element[3][0] = (mat1).element[0][0] * (mat2).element[3][0] \
                       + (mat1).element[1][0] * (mat2).element[3][1] \
                       + (mat1).element[2][0] * (mat2).element[3][2] \
                       + (mat1).element[3][0]; \
  (mat3).element[3][1] = (mat1).element[0][1] * (mat2).element[3][0] \
                       + (mat1).element[1][1] * (mat2).element[3][1] \
                       + (mat1).element[2][1] * (mat2).element[3][2] \
                       + (mat1).element[3][1]; \
  (mat3).element[3][2] = (mat1).element[0][2] * (mat2).element[3][0] \
                       + (mat1).element[1][2] * (mat2).element[3][1] \
                       + (mat1).element[2][2] * (mat2).element[3][2] \
                       + (mat1).element[3][2]; \
  \
  (mat3).element[0][3] = (mat3).element[1][3] = (mat3).element[2][3] = 0.0; \
  (mat3).element[3][3] = 1.0; \
}
