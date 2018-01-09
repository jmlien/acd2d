/*****************************************************************************/
/*                                                                           */
/*            Copyright (C)           1997-1999         M. Held              */
/*                                                                           */
/* This code is not in the public domain. All rights reserved! Please make   */
/* sure to read the full copyright statement contained in "README" or in the */
/* "main" file of this code, such as "main.c".                               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* Written by:  Martin Held                                                  */
/* Date:        March 16, 1997                                               */
/* Modified:                                                                 */
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

#ifndef _NUMERICS_H_
#define _NUMERICS_H_
/*                                                                           */
/* the following variables are static variables, used only within this file. */
/* unfortunately, when defined as "static", gcc barks that some variable     */
/* has been defined but is not used. without the "static" definition, cc on  */
/* SGIs complains that variables are multiply defined...                     */
/*                                                                           */
extern point numerics_h_p, numerics_h_q, numerics_h_r;
extern double numerics_h_det, numerics_h_dot;
extern int numerics_h_ori1, numerics_h_ori2;

#define ZERO eps3

#endif




#define BaseLength(u, v, base, delta) { \
   (base).x = (v).x - (u).x; \
   (base).y = (v).y - (u).y; \
   delta    = Abs((base).x) + Abs((base).y); }


#define SideLength(u, v, base, delta) { \
   (base).x = (v).x - (u).x; \
   (base).y = (v).y - (u).y; \
   delta    = (base).x * (base).x + (base).y * base.y; }



/*                                                                           */
/* this macro checks whether  i3,  which is collinear with  i1, i2,  is      */
/* between  i1, i2. note that we rely on the lexicographic sorting of the    */
/* points!                                                                   */
/*                                                                           */
#define InBetween(i1, i2, i3) \
   ((i1 <= i3)  &&  (i3 <= i2))


#define StrictlyInBetween(i1, i2, i3) \
   ((i1 < i3)  &&  (i3 < i2))


/*                                                                           */
/* this macro computes the determinant  det(points[i],points[j],points[k])   */
/* in a consistent way.                                                      */
/*                                                                           */
#define StableDet2D(i, j, k, det) \
 { assert(InPointsList(i)); \
   assert(InPointsList(j)); \
   assert(InPointsList(k)); \
   \
   if ((i == j)  ||  (i == k)  ||   (j == k)) { \
      det = C_0_0; \
   } \
   else { \
      numerics_h_p = points[i]; \
      numerics_h_q = points[j]; \
      numerics_h_r = points[k]; \
      \
      if (i < j) { \
         if (j < k)            /* i < j < k */ \
            det =  Det2D(numerics_h_p, numerics_h_q, numerics_h_r); \
         else if (i < k)       /* i < k < j */ \
            det = -Det2D(numerics_h_p, numerics_h_r, numerics_h_q); \
         else                  /* k < i < j */ \
            det =  Det2D(numerics_h_r, numerics_h_p, numerics_h_q); \
      } \
      else { \
         if (i < k)            /* j < i < k */ \
            det = -Det2D(numerics_h_q, numerics_h_p, numerics_h_r); \
         else if (j < k)       /* j < k < i */ \
            det =  Det2D(numerics_h_q, numerics_h_r, numerics_h_p); \
         else                  /* k < j < i */ \
            det = -Det2D(numerics_h_r, numerics_h_q, numerics_h_p); \
      } \
   } \
 }



/*                                                                           */
/* this macro sets ori to +1 if the points  i, j, k are given in CCW order,  */
/*                        -1 if the points  i, j, k are given in CW order,   */
/*                         0 if the points  i, j, k are collinear.           */
/*                                                                           */
#define Orientation(i, j, k, ori) \
 { StableDet2D(i, j, k, numerics_h_det); \
   \
   if (lt(numerics_h_det, ZERO))        ori = -1; \
   else if (gt(numerics_h_det, ZERO))   ori =  1; \
   else                                 ori =  0; \
}




/*                                                                           */
/* this macro checks whether  l  is in the cone defined by  i, j  and  j, k  */
/*                                                                           */
#define IsInCone(i, j, k, l, convex, flag) \
 { assert(InPointsList(i)); \
   assert(InPointsList(j)); \
   assert(InPointsList(k)); \
   assert(InPointsList(l)); \
   \
   flag = true; \
   if (convex) { \
      if ((l != i)  &&  (i != j)) { \
         Orientation(i, j, l, numerics_h_ori1); \
         if (numerics_h_ori1 < 0)                    flag = false; \
         else if (numerics_h_ori1 == 0) { \
            if (i < j) { \
               if (!InBetween(i, j, l))              flag = false; \
            } \
            else { \
               if (!InBetween(j, i, l))              flag = false; \
            } \
         } \
      } \
      if ((l != k)  &&  (j != k)  &&  (flag == true)) { \
         Orientation(j, k, l, numerics_h_ori2); \
         if (numerics_h_ori2 < 0)                    flag = false; \
         else if (numerics_h_ori2 == 0) { \
            if (j < k) { \
               if (!InBetween(j, k, l))              flag = false; \
            } \
            else { \
               if (!InBetween(k, j, l))              flag = false; \
            } \
         } \
      } \
   } \
   else { \
      Orientation(i, j, l, numerics_h_ori1); \
      if (numerics_h_ori1 <= 0) { \
         Orientation(j, k, l, numerics_h_ori2); \
         if (numerics_h_ori2 < 0)                   flag = false; \
      } \
   } \
}




/*                                                                           */
/* returns    0 ... if angle is 180 degrees                                  */
/*            1 ... if angle between 0 and 180 degrees                       */
/*            2 ... if angle is 0 degrees                                    */
/*           -1 ... if angle between 180 and 360 degrees                     */
/*           -2 ... if angle is 360 degrees                                  */
/*                                                                           */
#define IsConvexAngle(i, j, k, ind, angle) \
 { assert(InPointsList(i)); \
   assert(InPointsList(j)); \
   assert(InPointsList(k)); \
   \
   if (i == j) { \
      if (j == k) { \
         /*                                                               */ \
         /* all three vertices are identical; we set the angle to 1 in    */ \
         /* order to enable clipping of  j.                               */ \
         /*                                                               */ \
         angle = 1; \
      } \
      else { \
         /*                                                               */ \
         /* two of the three vertices are identical; we set the angle to  */ \
         /* to 1 order to enable clipping of  j.                          */ \
         /*                                                               */ \
         angle = 1; \
      } \
   } \
   else if (j == k) { \
      /*                                                                  */ \
      /* two vertices are identical. we could either determine the angle  */ \
      /* by means of yet another lengthy analysis, or simply set the      */ \
      /* angle to -1. using -1 means to err on the safe side, as all the  */ \
      /* incarnations of this vertex will be clipped right at the start   */ \
      /* of the ear-clipping algorithm. thus, eventually there will be no */ \
      /* other duplicates at this vertex position, and the regular        */ \
      /* classification of angles will yield the correct answer for j.    */ \
      /*                                                                  */ \
      angle = -1; \
   } \
   else { \
      Orientation(i, j, k, numerics_h_ori1); \
      if (numerics_h_ori1 > 0) { \
         angle =  1; \
      } \
      else if (numerics_h_ori1 < 0) { \
         angle = -1; \
      } \
      else { \
         /*                                                               */ \
         /* 0, 180, or 360 degrees.                                       */ \
         /*                                                               */ \
         VectorSub2D(points[i], points[j], numerics_h_p); \
         VectorSub2D(points[k], points[j], numerics_h_q); \
         numerics_h_dot = DotProduct2D(numerics_h_p, numerics_h_q); \
         if (numerics_h_dot < 0.0) { \
            /*                                                            */ \
            /* 180 degrees.                                               */ \
            /*                                                            */ \
            angle = 0; \
         } \
         else { \
            /*                                                            */ \
            /* 0 or 360 degrees? this cannot be judged locally, and more  */ \
            /* work is needed.                                            */ \
            /*                                                            */ \
            angle = SpikeAngle(i, j, k, ind); \
         } \
      } \
   } \
}




/*                                                                           */
/* this macro checks whether point  i4  is inside of or on the boundary      */
/* of the triangle  i1, i2, i3                                               */
/*                                                                           */
#define PntInTriangle(i1, i2, i3, i4, inside) \
 { \
   ++pnt_tri_cntr; \
   inside = false; \
   Orientation(i2, i3, i4, numerics_h_ori1); \
   if (numerics_h_ori1 >= 0) { \
      Orientation(i1, i2, i4, numerics_h_ori1); \
      if (numerics_h_ori1 >= 0) { \
         Orientation(i3, i1, i4, numerics_h_ori1); \
         if (numerics_h_ori1 >= 0)  inside = true; \
      } \
   } \
}


/*                                                                           */
/* this macro checks whether point  i4  is inside of or on the boundary      */
/* of the triangle  i1, i2, i3. it also returns a classification if  i4  is  */
/* on the boundary of the triangle (except for the edge  i2, i3).            */
/*                                                                           */
#define VtxInTriangle(i1, i2, i3, i4, inside, classifier) \
 { \
   ++pnt_tri_cntr; \
   inside = false; \
   Orientation(i2, i3, i4, numerics_h_ori1); \
   if (numerics_h_ori1 >= 0) { \
      Orientation(i1, i2, i4, numerics_h_ori1); \
      if (numerics_h_ori1 > 0) { \
         Orientation(i3, i1, i4, numerics_h_ori1); \
         if (numerics_h_ori1 > 0) { \
            inside = true; \
            classifier = 0; \
         } \
         else if (numerics_h_ori1 == 0) { \
            inside = true; \
            classifier  = 1; \
         } \
      } \
      else if (numerics_h_ori1 == 0) { \
         Orientation(i3, i1, i4, numerics_h_ori1); \
         if (numerics_h_ori1 > 0) { \
            inside = true; \
            classifier = 2; \
         } \
         else if (numerics_h_ori1 == 0) { \
            inside = true; \
            classifier  = 3; \
         } \
      } \
   } \
}
