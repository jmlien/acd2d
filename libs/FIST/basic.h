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
/* Date:        May 6, 1996                                                  */
/*                                                                           */
/* E-Mail:      held@cosy.sbg.ac.at                                          */
/* Fax Mail:    (+43 662) 8044-611                                           */
/* Voice Mail:  (+43 662) 6044-6304                                          */
/* Snail Mail:  Martin Held                                                  */
/*              Universitaet Salzburg                                        */
/*              Institut fuer Computerwissenschaften                         */
/*              A-5020 Salzburg, Austria                                     */
/*                                                                           */
/*****************************************************************************/
#ifndef _BASIC_H_
#define _BASIC_H_
/*                                                                           */
/* the following variables are static variables, used only within this file. */
/* unfortunately, when defined as "static", gcc barks that some variable     */
/* has been defined but is not used. without the "static" definition, cc on  */
/* SGIs complains that variables are multiply defined...                     */
/*                                                                           */
extern double basic_h_local, basic_h_local_min, basic_h_local_max;
extern double basic_h_local_quot, basic_h_local_delta;
extern double basic_h_local_a, basic_h_local_b;
extern vertex basic_h_v1, basic_h_v2;
extern long basic_i_local;

#endif

#ifndef  RAND

#define  RND_MAX 2147483647

#define UniformRandom(x) \
{\
    x = ((double) random()) / RND_MAX; }

#define RandomInteger(N) \
 (\
   assert(N > 0), \
   basic_i_local = random(), \
   basic_i_local - (basic_i_local / (N)) * (N))
      
#define InitRandom(seed) \
{\
    srandom(seed); }

#else 

#ifdef RAND_MAX
#define RND_MAX RAND_MAX
#else
#define RND_MAX 32767
#endif

#define UniformRandom(x) \
{\
    x = ((double) rand()) / RND_MAX; }

#define RandomInteger(N) \
 (\
   assert(N > 0), \
   basic_i_local = rand(), \
   basic_i_local - (basic_i_local / (N)) * (N))

#define InitRandom(seed) \
{\
    srand(seed); }

#endif


#define RandomVertex(a) \
{\
   UniformRandom((a).x); \
   UniformRandom((a).y); \
   UniformRandom((a).z); }

   


#define DetExp(u_x, u_y, u_z, v_x, v_y, v_z, w_x, w_y, w_z) \
 ((u_x) * ((v_y) * (w_z)  -  (v_z) * (w_y))  - \
  (u_y) * ((v_x) * (w_z)  -  (v_z) * (w_x))  + \
  (u_z) * ((v_x) * (w_y)  -  (v_y) * (w_x)))


#define Det3D(u, v, w) \
 ((u).x * ((v).y * (w).z  -  (v).z * (w).y)  -  \
  (u).y * ((v).x * (w).z  -  (v).z * (w).x)  +  \
  (u).z * ((v).x * (w).y  -  (v).y * (w).x))


#define Det2D(u, v, w) \
 (((u).x - (v).x) * ((v).y - (w).y) + ((v).y - (u).y) * ((v).x - (w).x))


#define Length2(u)  \
 (((u).x * (u).x) + ((u).y * (u).y) + ((u).z * (u).z))


#define Length_l1(u)  \
 (Abs((u).x) + Abs((u).y) + Abs((u).z))


#define Length_l2(u)  \
 sqrt(((u).x * (u).x) + ((u).y * (u).y) + ((u).z * (u).z))


#define DotProduct(u, v)  \
 (((u).x * (v).x) + ((u).y * (v).y) + ((u).z * (v).z))


#define DotProduct2D(u, v)  \
 (((u).x * (v).x) + ((u).y * (v).y))


#define VectorProduct(p, q, r)  \
 {(r).x = (p).y * (q).z  -  (q).y * (p).z; \
  (r).y = (q).x * (p).z  -  (p).x * (q).z; \
  (r).z = (p).x * (q).y  -  (q).x * (p).y; } 


#define VectorAdd(p, q, r)  \
 {(r).x = (p).x + (q).x; \
  (r).y = (p).y + (q).y; \
  (r).z = (p).z + (q).z; } 

#define LinearComb(p, q, r, t)  \
 {(r).x = (p).x + (t) * ((q).x - (p).x); \
  (r).y = (p).y + (t) * ((q).y - (p).y); \
  (r).z = (p).z + (t) * ((q).z - (p).z); }

#define VectorSub(p, q, r)  \
 {(r).x = (p).x - (q).x; \
  (r).y = (p).y - (q).y; \
  (r).z = (p).z - (q).z; } 


#define VectorAdd2D(p, q, r)  \
 {(r).x = (p).x + (q).x; \
  (r).y = (p).y + (q).y; } 


#define VectorSub2D(p, q, r)  \
 {(r).x = (p).x - (q).x; \
  (r).y = (p).y - (q).y; } 


#define LinearComb2D(p, q, r, t)  \
 {(r).x = (p).x + (t) * ((q).x - (p).x); \
  (r).y = (p).y + (t) * ((q).y - (p).y); }

#define InvertVector(p) \
 {(p).x = -(p).x; \
  (p).y = -(p).y; \
  (p).z = -(p).z; }


#define MultScalar(scalar, u)  \
 {(u).x *= scalar; \
  (u).y *= scalar; \
  (u).z *= scalar; } 


#define DivScalar(scalar, u)  \
 {(u).x /= scalar; \
  (u).y /= scalar; \
  (u).z /= scalar; } 


#define InvertVector2D(p) \
 {(p).x = -(p).x; \
  (p).y = -(p).y; }


#define MultScalar2D(scalar, u)  \
 {(u).x *= scalar; \
  (u).y *= scalar; } 


#define DivScalar2D(scalar, u)  \
 {(u).x /= scalar; \
  (u).y /= scalar; } 


#define SignEps(x, eps)  \
(basic_h_local = (x), \
 ((basic_h_local <= eps) ? ((basic_h_local < -eps) ? -1 : 0) : 1))


#define MinMax3(a, b, c, min, max) {\
  if ((a) < (b)) {\
                    if ((a) < (c)) {\
                                      min = (a); \
                                      if ((b) < (c))  max = (c); \
                                      else            max = (b); \
                    }\
                    else {          \
                                      min = (c); \
                                      max = (b); \
                    }\
  }\
  else {          \
                    if ((a) < (c)) {\
                                      min = (b); \
                                      max = (c); \
                    }  \
                    else {          \
                                      max = (a); \
                                      if ((b) < (c))  min = (b); \
                                      else            min = (c); \
                    }  \
  }\
}


#define MinMax4(a, b, c, d, min, max)  {\
  if ((a) < (b)) {\
                    if ((a) < (c)) {\
                                      min = (a); \
                                      if ((b) < (c))  max = (c); \
                                      else            max = (b); \
                    }  \
                    else {          \
                                      min = (c); \
                                      max = (b); \
                    } \
  }\
  else {         \
                   if ((a) < (c)) { \
                                      min = (b); \
                                      max = (c); \
                   }  \
                   else {           \
                                      max = (a); \
                                      if ((b) < (c))  min = (b); \
                                      else            min = (c); \
                   }\
  } \
 if      ((d) < min)   min = (d); \
 else if ((d) > max)   max = (d); \
}


#define SortTwoNumbers(a, b, c) {\
  if (a > b) { \
                 c = a; \
                 a = b; \
                 b = c; \
  } \
}



#define SortThreeNumbers(a, b, c, d)  {\
  if (a < b) {\
                if (a < c) {\
                              if (b > c) {\
                                            d = b; \
                                            b = c; \
                                            c = d; \
                              }\
                }\
                else {      \
                              d = a; \
                              a = c; \
                              c = b; \
                              b = d; \
                }\
  }\
  else {      \
                if (a < c) { \
                               d = a; \
                               a = b; \
                               b = d; \
                } \
                else {       \
                               if (b < c) {\
                                             d = a; \
                                             a = b; \
                                             b = c; \
                                             c = d; \
                               }\
                               else {      \
                                             d = a; \
                                             a = c; \
                                             c = d; \
                               }\
                }\
  }\
}


#define MinMax(a, b, min, max) {\
  if ((b) < (a)) {\
                    min = (b); \
                    max = (a); \
  } \
  else {          \
                    min = (a); \
                    max = (b); \
  } \
}
   

#define MinMaxQ(a, b, min, max) \
  (((b) < (a)) ? (min = (b), max = (a))  :  (min = (a), max = (b)))


#define MinMax3Q(a, b, c, min, max) \
  (((b) < (a)) ? (((c) < (b)) ? (min = (c), max = (a))  : \
                                (((c) > (a)) ? (min = (b), max = (c)) : \
                                               (min = (b), max = (a)))) : \
                 (((c) > (b)) ? (min = (a), max = (c))  : \
                                (((c) > (a)) ? (min = (a), max = (b)) : \
                                               (min = (c), max = (b)))))


#define TetraBBox(a, b, c, d, bb_min, bb_max) {\
  MinMax4((a).x, (b).x, (c).x, (d).x, (bb_min).x, (bb_max).x); \
  MinMax4((a).y, (b).y, (c).y, (d).y, (bb_min).y, (bb_max).y); \
  MinMax4((a).z, (b).z, (c).z, (d).z, (bb_min).z, (bb_max).z); }


#define SphBBox(p, r, bb_min, bb_max) {\
  (bb_min).x = (p).x - r; \
  (bb_min).y = (p).y - r; \
  (bb_min).z = (p).z - r; \
  (bb_max).x = (p).x + r; \
  (bb_max).y = (p).y + r; \
  (bb_max).z = (p).z + r; }


#define CylBBox(p, q, r, bb_min, bb_max) {\
  (bb_min).x = (p).x - r; \
  (bb_min).y = (p).y - r; \
  (bb_min).z = (p).z; \
  (bb_max).x = (q).x + r; \
  (bb_max).y = (q).y + r; \
  (bb_max).z = (q).z; }


#define ConBBox(p, q, r1, r2, bb_min, bb_max) {\
  if (r1 < r2)  basic_h_local = r2; \
  else          basic_h_local = r1; \
  (bb_min).x = (p).x - basic_h_local; \
  (bb_min).y = (p).y - basic_h_local; \
  (bb_min).z = (p).z; \
  (bb_max).x = (q).x + basic_h_local; \
  (bb_max).y = (q).y + basic_h_local; \
  (bb_max).z = (q).z; }


/*                                                                           */
/* r1 >= r2  required; otherwise, we set r1 = r2!                            */
/*                                                                           */
#define UpCveTorBBox(p, r1, r2, z_min, z_max, bb_min, bb_max) { \
  if (r1 < r2) { \
     r1 = r2; \
  } \
  else { \
     basic_h_local_a = p.z - z_min; \
     basic_h_local_a = r2 * r2 - basic_h_local_a * basic_h_local_a; \
     if (basic_h_local_a < 0.0) { \
        basic_h_local_a = 0.0; \
     } \
     else { \
        basic_h_local_a = sqrt(basic_h_local_a); \
     } \
     basic_h_local_b = p.z - z_max; \
     basic_h_local_b = r2 * r2 - basic_h_local_b * basic_h_local_b; \
     if (basic_h_local_b < 0.0) { \
        basic_h_local_b = 0.0; \
     } \
     else { \
        basic_h_local_b = sqrt(basic_h_local_b); \
     } \
     basic_h_local = Min(basic_h_local_a, basic_h_local_b); \
     basic_h_local = r1 - basic_h_local; \
  } \
  (bb_min).x = (p).x - basic_h_local; \
  (bb_min).y = (p).y - basic_h_local; \
  (bb_min).z = z_min; \
  (bb_max).x = (p).x + basic_h_local; \
  (bb_max).y = (p).y + basic_h_local; \
  (bb_max).z = z_max;  }



#define UpCvxTorBBox(p, r1, r2, z_min, z_max, bb_min, bb_max) { \
  if ((z_min <= p.z)  &&  (z_max >= p.z)) {  \
     basic_h_local = r1 + r2; \
  }  \
  else {  \
     basic_h_local_a = p.z - z_min; \
     basic_h_local_a = r2 * r2 - basic_h_local_a * basic_h_local_a; \
     if (basic_h_local_a < 0.0) { \
        basic_h_local_a = 0.0; \
     } \
     else { \
        basic_h_local_a = sqrt(basic_h_local_a); \
     } \
     basic_h_local_b = p.z - z_max; \
     basic_h_local_b = r2 * r2 - basic_h_local_b * basic_h_local_b; \
     if (basic_h_local_b < 0.0) { \
        basic_h_local_b = 0.0; \
     } \
     else { \
        basic_h_local_b = sqrt(basic_h_local_b); \
     } \
     basic_h_local = Max(basic_h_local_a, basic_h_local_b); \
     basic_h_local += r1; \
  } \
  (bb_min).x = (p).x - basic_h_local; \
  (bb_min).y = (p).y - basic_h_local; \
  (bb_min).z = z_min; \
  (bb_max).x = (p).x + basic_h_local; \
  (bb_max).y = (p).y + basic_h_local; \
  (bb_max).z = z_max;  }


#define CirBBox(p, r, bb_min, bb_max) {\
  (bb_min).x = (p).x - r; \
  (bb_min).y = (p).y - r; \
  (bb_max).x = (p).x + r; \
  (bb_max).y = (p).y + r; }


#define TriBBox(a, b, c, bb_min, bb_max) {\
  MinMax3((a).x, (b).x, (c).x, (bb_min).x, (bb_max).x); \
  MinMax3((a).y, (b).y, (c).y, (bb_min).y, (bb_max).y); \
  MinMax3((a).z, (b).z, (c).z, (bb_min).z, (bb_max).z); }


#define TriBBox2D(a, b, c, bb_min, bb_max) {\
  MinMax3((a).x, (b).x, (c).x, (bb_min).x, (bb_max).x); \
  MinMax3((a).y, (b).y, (c).y, (bb_min).y, (bb_max).y); }


#define EdgeBBox(a, b, bb_min, bb_max) {\
  MinMax((a).x, (b).x, (bb_min).x, (bb_max).x); \
  MinMax((a).y, (b).y, (bb_min).y, (bb_max).y); \
  MinMax((a).z, (b).z, (bb_min).z, (bb_max).z); }


#define PntInBBox(bb_min, bb_max, p, eps) \
((lt((bb_max).x - (p).x, eps)) ? false : \
 ((lt((bb_max).y - (p).y, eps)) ? false : \
  ((lt((bb_max).z - (p).z, eps)) ? false : \
   ((lt((p).x - (bb_min).x, eps)) ? false : \
    ((lt((p).y - (bb_min).y, eps)) ? false : \
     ((lt((p).z - (bb_min).z, eps)) ? false : true))))))
  
  
#define EdgeInBBox(bb_min, bb_max, p, q, eps) \
  (MinMaxQ((p).x, (q).x, basic_h_local_min, basic_h_local_max), \
   ((lt((bb_max).x - basic_h_local_min, eps)) ? false : \
    ((lt(basic_h_local_max - (bb_min).x, eps)) ? false : \
     (MinMaxQ((p).y, (q).y, basic_h_local_min, basic_h_local_max), \
      ((lt((bb_max).y - basic_h_local_min, eps)) ? false : \
       ((lt(basic_h_local_max - (bb_min).y, eps)) ? false : \
        (MinMaxQ((p).z, (q).z, basic_h_local_min, basic_h_local_max), \
         ((lt((bb_max).z - basic_h_local_min, eps)) ? false : \
          ((lt(basic_h_local_max - (bb_min).z, eps)) ? false : true)))))))))
  

#define EdgeInBBoxPlain(bb_min, bb_max, p, q) \
  (MinMaxQ((p).x, (q).x, basic_h_local_min, basic_h_local_max), \
   ((lt((bb_max).x - basic_h_local_min, 0.0)) ? false : \
    ((lt(basic_h_local_max - (bb_min).x, 0.0)) ? false : \
     (MinMaxQ((p).y, (q).y, basic_h_local_min, basic_h_local_max), \
      ((lt((bb_max).y - basic_h_local_min, 0.0)) ? false : \
       ((lt(basic_h_local_max - (bb_min).y, 0.0)) ? false : \
        (MinMaxQ((p).z, (q).z, basic_h_local_min, basic_h_local_max), \
         ((lt((bb_max).z - basic_h_local_min, 0.0)) ? false : \
          ((lt(basic_h_local_max - (bb_min).z, 0.0)) ? false : true)))))))))
  

#define TriInBBox(bb_min, bb_max, p, q, r, eps)  \
  (MinMax3Q((p).x, (q).x, (r).x, basic_h_local_min, basic_h_local_max), \
   ((lt((bb_max).x - basic_h_local_min, eps)) ? false : \
    ((lt(basic_h_local_max - (bb_min).x, eps)) ? false : \
     (MinMax3Q((p).y, (q).y, (r).y, basic_h_local_min, basic_h_local_max), \
      ((lt((bb_max).y - basic_h_local_min, eps)) ? false : \
       ((lt(basic_h_local_max - (bb_min).y, eps)) ? false : \
        (MinMax3Q((p).z, (q).z, (r).z, basic_h_local_min, basic_h_local_max), \
         ((lt((bb_max).z - basic_h_local_min, eps)) ? false : \
          ((lt(basic_h_local_max - (bb_min).z, eps)) ? false : true)))))))))


#define TriInBBoxPlain(bb_min, bb_max, p, q, r)  \
  (MinMax3Q((p).x, (q).x, (r).x, basic_h_local_min, basic_h_local_max), \
   ((lt((bb_max).x - basic_h_local_min, 0.0)) ? false : \
    ((lt(basic_h_local_max - (bb_min).x, 0.0)) ? false : \
     (MinMax3Q((p).y, (q).y, (r).y, basic_h_local_min, basic_h_local_max), \
      ((lt((bb_max).y - basic_h_local_min, 0.0)) ? false : \
       ((lt(basic_h_local_max - (bb_min).y, 0.0)) ? false : \
        (MinMax3Q((p).z, (q).z, (r).z, basic_h_local_min, basic_h_local_max), \
         ((lt((bb_max).z - basic_h_local_min, 0.0)) ? false : \
          ((lt(basic_h_local_max - (bb_min).z, 0.0)) ? false : true)))))))))


#define SphInBBox(bb_min, bb_max, p, rad, eps) \
((lt((bb_max).x - (p).x + rad, eps)) ? false : \
 ((lt((bb_max).y - (p).y + rad, eps)) ? false : \
  ((lt((bb_max).z - (p).z + rad, eps)) ? false : \
   ((lt((p).x + rad - (bb_min).x, eps)) ? false : \
    ((lt((p).y + rad - (bb_min).y, eps)) ? false : \
     ((lt((p).z + rad - (bb_min).z, eps)) ? false : true))))))
  
  
#define SphInSphBBox(p, r1, q, r2, eps) \
  ((basic_h_local_delta = r1 + r2), \
   ((basic_h_local = (q).x - (p).x), \
    (basic_h_local = Abs(basic_h_local)), \
    ((gt(basic_h_local - basic_h_local_delta, eps)) ? false : \
     ((basic_h_local = (q).y - (p).y), \
      (basic_h_local = Abs(basic_h_local)), \
      ((gt(basic_h_local - basic_h_local_delta, eps)) ? false : \
       ((basic_h_local = (q).z - (p).z), \
        (basic_h_local = Abs(basic_h_local)), \
        ((gt(basic_h_local - basic_h_local_delta, eps)) ? false : true))))))) 
  

#define TriInBBox2D(bb_min, bb_max, p, q, r, eps)  \
  (MinMax3Q((p).x, (q).x, (r).x, basic_h_local_min, basic_h_local_max), \
   ((lt((bb_max).x - basic_h_local_min, eps)) ? false : \
    ((lt(basic_h_local_max - (bb_min).x, eps)) ? false : \
     (MinMax3Q((p).y, (q).y, (r).y, basic_h_local_min, basic_h_local_max), \
      ((lt((bb_max).y - basic_h_local_min, eps)) ? false : \
       ((lt(basic_h_local_max - (bb_min).y, eps)) ? false : true))))))


#define EdgeInBBox2D(bb_min, bb_max, p, q, eps) \
  (MinMaxQ((p).x, (q).x, basic_h_local_min, basic_h_local_max), \
   ((lt((bb_max).x - basic_h_local_min, eps)) ? false : \
    ((lt(basic_h_local_max - (bb_min).x, eps)) ? false : \
     (MinMaxQ((p).y, (q).y, basic_h_local_min, basic_h_local_max), \
      ((lt((bb_max).y - basic_h_local_min, eps)) ? false : \
       ((lt(basic_h_local_max - (bb_min).y, eps)) ? false : true))))))
  

#define PntInBBox2D(bb_min, bb_max, p, eps) \
((lt((bb_max).x - (p).x, eps)) ? false : \
 ((lt((bb_max).y - (p).y, eps)) ? false : \
  ((lt((p).x - (bb_min).x, eps)) ? false : \
   ((lt((p).y - (bb_min).y, eps)) ? false : true))))
  
  
/*                                                                           */
/* this macro computes the minimum distance squared of a point  r  from the  */
/* line segment  p, q.                                                       */
/*                                                                           */
/* pq  := VectorSub(q, p, pq); needs to be supplied to the macro             */
/* pq2 := DotProduct(pq, pq); needs to be supplied to the macro              */
/*                                                                           */
#define PntEdgeDist(p, q, pq, pq2, r, dist, zero) \
{\
  VectorSub(r, p, basic_h_v1); \
  if (gt(pq2, zero)) { \
    dist = DotProduct(basic_h_v1, pq); \
    if (dist < 0.0) { \
      /*                                                                  */ \
      /* the minimum distance is obtained before  p                       */ \
      /*                                                                  */ \
      dist = DotProduct(basic_h_v1, basic_h_v1); \
    } \
    else if (dist > pq2) { \
      /*                                                                  */ \
      /* the minimum distance is obtained after  q                        */ \
      /*                                                                  */ \
      VectorSub(r, q, basic_h_v1); \
      dist = DotProduct(basic_h_v1, basic_h_v1); \
    } \
    else { \
      /*                                                                  */ \
      /* return the normal distance                                       */ \
      /*                                                                  */ \
      dist = (DotProduct(basic_h_v1, basic_h_v1) - dist * dist / pq2); \
    } \
  } \
  else { \
    /*                                                                    */ \
    /* the line segment degenerates to a point                            */ \
    /*                                                                    */ \
    dist = DotProduct(basic_h_v1, basic_h_v1); \
  } \
}



/*                                                                           */
/* This macro solves the following 2x2 linear system:                        */
/*                                                                           */
/*      A[0][0] * x + A[0][1] * y = B[0]                                     */
/*      A[1][1] * x + A[1][1] * y = B[1]                                     */
/*                                                                           */
/* If a unique solution exists, then exists := 1, and the solution is stored */
/* in xy[2]. If the solution is not unique, then exists := 2, and a solution */
/* is stored in xy[2]. Otherwise, exists := 0.                               */
/*                                                                           */
/* The threshold zero is used for determining whether a value is zero.       */
/*                                                                           */
/* i, j, I, J  are dummy integers needed within the macro.                   */
/*                                                                           */
#define LinearEqnSolver_2x2(A, B, xy, exists, zero, i, j, I, J) \
{ \
  /*                                                                      */ \
  /* find a column with a non-zero element                                */ \
  /*                                                                      */ \
  exists = 0; \
  if (!eq((A)[0][0], zero) ||  !eq((A)[1][0], zero)) { \
    I = 0; \
    J = 1; \
  } \
  else if (!eq((A)[0][1], zero) ||  !eq((A)[1][1], zero)) { \
    I = 1; \
    J = 0; \
  } \
  else { \
    if (eq((B)[0], zero)  &&  eq((B)[1], zero)) { \
      (xy)[0] = (xy)[1] = 0.0; \
      (exists) = 2;  \
    } \
    I = J = 0; \
  } \
  \
  /*                                                                      */ \
  /* determine i s.t. Abs(A[i][I]) is maximum.                            */ \
  /*                                                                      */ \
  if ((I > 0)  ||  (J > 0)) { \
    if (Abs((A)[0][I]) > Abs((A)[1][I])) { \
      i = 0; \
      j = 1; \
    } \
    else { \
      i = 1; \
      j = 0; \
    } \
    \
    basic_h_local_quot  = (A)[j][I] / (A)[i][I]; \
    basic_h_local_delta = (A)[j][J] - basic_h_local_quot * (A)[i][J]; \
    if (!eq(basic_h_local_delta, zero)) { \
      (xy)[J] = ((B)[j] - basic_h_local_quot * (B)[i]) / basic_h_local_delta; \
      (xy)[I] = ((B)[i] - (xy)[J] * (A)[i][J]) / (A)[i][I]; \
      (exists) = 1; \
    } \
    else { \
      basic_h_local_delta = (B)[j] - basic_h_local_quot * (B)[i]; \
      if (eq(basic_h_local_delta, zero)) { \
        (xy)[J] = 0.0; \
        (xy)[I] = (B)[i] / (A)[i][I]; \
        (exists) = 2; \
      } \
      else { \
        (exists) = 0; \
      } \
    } \
  } \
}



/*                                                                           */
/* this macro computes the bounding box of a general cylinder.               */
/*                                                                           */
/* pq := q - p   (needs to be supplied to the macro)                         */
/*                                                                           */
#define CylinderBBox(p, q, pq, radius, bb_min, bb_max) \
{ \
  /*                                                                       */ \
  /* compute min/max x-coordinates                                         */ \
  /*                                                                       */ \
  basic_h_v1.x =  0.0; \
  basic_h_v1.y = -(pq).z; \
  basic_h_v1.z =  (pq).y; \
  VectorProduct(pq, basic_h_v1, basic_h_v2);  \
  if (gt(basic_h_local = Length_l2(basic_h_v2), ZERO)) { \
    if ((basic_h_v2.x *= ((radius) / basic_h_local)) > 0.0) { \
      (bb_max).x = Max((p).x, (q).x) + basic_h_v2.x; \
      (bb_min).x = Min((p).x, (q).x) - basic_h_v2.x; \
    } \
    else { \
      (bb_max).x = Max((p).x, (q).x) - basic_h_v2.x; \
      (bb_min).x = Min((p).x, (q).x) + basic_h_v2.x; \
    }  \
  } \
  else { \
    MinMax((p).x, (q).x, (bb_min).x, (bb_max).x); \
  } \
  /*                                                                       */ \
  /* compute min/max y-coordinates                                         */ \
  /*                                                                       */ \
  basic_h_v1.x = -(pq).z; \
  basic_h_v1.y =  0.0; \
  basic_h_v1.z =  (pq).x; \
  VectorProduct(pq, basic_h_v1, basic_h_v2);  \
  if (gt(basic_h_local = Length_l2(basic_h_v2), ZERO)) { \
    if ((basic_h_v2.y *= ((radius) / basic_h_local)) > 0.0) { \
      (bb_max).y = Max((p).y, (q).y) + basic_h_v2.y; \
      (bb_min).y = Min((p).y, (q).y) - basic_h_v2.y; \
    } \
    else { \
      (bb_max).y = Max((p).y, (q).y) - basic_h_v2.y; \
      (bb_min).y = Min((p).y, (q).y) + basic_h_v2.y; \
    }  \
  } \
  else { \
    MinMax((p).y, (q).y, (bb_min).y, (bb_max).y); \
  } \
  /*                                                                       */ \
  /* compute min/max z-coordinates                                         */ \
  /*                                                                       */ \
  basic_h_v1.x = -(pq).y; \
  basic_h_v1.y =  (pq).x; \
  basic_h_v1.z =  0.0; \
  VectorProduct(pq, basic_h_v1, basic_h_v2);  \
  if (gt(basic_h_local = Length_l2(basic_h_v2), ZERO)) { \
    if ((basic_h_v2.z *= ((radius) / basic_h_local)) > 0.0) { \
      (bb_max).z = Max((p).z, (q).z) + basic_h_v2.z; \
      (bb_min).z = Min((p).z, (q).z) - basic_h_v2.z; \
    } \
    else { \
      (bb_max).z = Max((p).z, (q).z) - basic_h_v2.z; \
      (bb_min).z = Min((p).z, (q).z) + basic_h_v2.z; \
    } \
  } \
  else { \
    MinMax((p).z, (q).z, (bb_min).z, (bb_max).z); \
  } \
}


/*                                                                           */
/* this macro computes the bounding box of a general cone.                   */
/*                                                                           */
/* pq := q - p   (needs to be supplied to the macro)                         */
/*                                                                           */
#define ConeBBox(p, q, pq, radius1, radius2, bb_min, bb_max) \
{ \
  /*                                                                       */ \
  /* compute min/max x-coordinates                                         */ \
  /*                                                                       */ \
  basic_h_v1.x =  0.0; \
  basic_h_v1.y = -(pq).z; \
  basic_h_v1.z =  (pq).y; \
  VectorProduct(pq, basic_h_v1, basic_h_v2);  \
  if (gt(basic_h_local = Length_l2(basic_h_v2), ZERO)) { \
    if ((basic_h_v2.x /= basic_h_local) > 0.0) { \
      basic_h_local_a = (p).x + radius1 * basic_h_v2.x; \
      basic_h_local_b = (q).x + radius2 * basic_h_v2.x; \
      (bb_max).x = Max(basic_h_local_a, basic_h_local_b); \
      basic_h_local_a = (p).x - radius1 * basic_h_v2.x; \
      basic_h_local_b = (q).x - radius2 * basic_h_v2.x; \
      (bb_min).x = Min(basic_h_local_a, basic_h_local_b); \
    } \
    else { \
      basic_h_local_a = (p).x - radius1 * basic_h_v2.x; \
      basic_h_local_b = (q).x - radius2 * basic_h_v2.x; \
      (bb_max).x = Max(basic_h_local_a, basic_h_local_b); \
      basic_h_local_a = (p).x + radius1 * basic_h_v2.x; \
      basic_h_local_b = (q).x + radius2 * basic_h_v2.x; \
      (bb_min).x = Min(basic_h_local_a, basic_h_local_b); \
    } \
  } \
  else { \
    MinMax((p).x, (q).x, (bb_min).x, (bb_max).x); \
  } \
  /*                                                                       */ \
  /* compute min/max y-coordinates                                         */ \
  /*                                                                       */ \
  basic_h_v1.x = -(pq).z; \
  basic_h_v1.y =  0.0; \
  basic_h_v1.z =  (pq).x; \
  VectorProduct(pq, basic_h_v1, basic_h_v2);  \
  if (gt(basic_h_local = Length_l2(basic_h_v2), ZERO)) { \
    if ((basic_h_v2.y /= basic_h_local) > 0.0) { \
      basic_h_local_a = (p).y + radius1 * basic_h_v2.y; \
      basic_h_local_b = (q).y + radius2 * basic_h_v2.y; \
      (bb_max).y = Max(basic_h_local_a, basic_h_local_b); \
      basic_h_local_a = (p).y - radius1 * basic_h_v2.y; \
      basic_h_local_b = (q).y - radius2 * basic_h_v2.y; \
      (bb_min).y = Min(basic_h_local_a, basic_h_local_b); \
    } \
    else { \
      basic_h_local_a = (p).y - radius1 * basic_h_v2.y; \
      basic_h_local_b = (q).y - radius2 * basic_h_v2.y; \
      (bb_max).y = Max(basic_h_local_a, basic_h_local_b); \
      basic_h_local_a = (p).y + radius1 * basic_h_v2.y; \
      basic_h_local_b = (q).y + radius2 * basic_h_v2.y; \
      (bb_min).y = Min(basic_h_local_a, basic_h_local_b); \
    }  \
  } \
  else { \
    MinMax((p).y, (q).y, (bb_min).y, (bb_max).y); \
  } \
  /*                                                                       */ \
  /* compute min/max z-coordinates                                         */ \
  /*                                                                       */ \
  basic_h_v1.x = -(pq).y; \
  basic_h_v1.y =  (pq).x; \
  basic_h_v1.z =  0.0; \
  VectorProduct(pq, basic_h_v1, basic_h_v2);  \
  if (gt(basic_h_local = Length_l2(basic_h_v2), ZERO)) { \
    if ((basic_h_v2.z /= basic_h_local) > 0.0) { \
      basic_h_local_a = (p).z + radius1 * basic_h_v2.z; \
      basic_h_local_b = (q).z + radius2 * basic_h_v2.z; \
      (bb_max).z = Max(basic_h_local_a, basic_h_local_b); \
      basic_h_local_a = (p).z - radius1 * basic_h_v2.z; \
      basic_h_local_b = (q).z - radius2 * basic_h_v2.z; \
      (bb_min).z = Min(basic_h_local_a, basic_h_local_b); \
    } \
    else { \
      basic_h_local_a = (p).z - radius1 * basic_h_v2.z; \
      basic_h_local_b = (q).z - radius2 * basic_h_v2.z; \
      (bb_max).z = Max(basic_h_local_a, basic_h_local_b); \
      basic_h_local_a = (p).z + radius1 * basic_h_v2.z; \
      basic_h_local_b = (q).z + radius2 * basic_h_v2.z; \
      (bb_min).z = Min(basic_h_local_a, basic_h_local_b); \
    }  \
  } \
  else { \
    MinMax((p).z, (q).z, (bb_min).z, (bb_max).z); \
  } \
}



#define SphSphTest3D(p, r_1, q, r_2, eps) \
  ((basic_h_local_delta = (r_1) + (r_2)), \
   (basic_h_v1.x = (q).x - (p).x), \
   (basic_h_v1.y = (q).y - (p).y), \
   (basic_h_v1.z = (q).z - (p).z), \
   (basic_h_local = DotProduct(basic_h_v1,basic_h_v1) - \
                    basic_h_local_delta * basic_h_local_delta), \
   (le(basic_h_local, eps)))



#define CirCirTest3D(p, r_1, q, r_2, eps) \
  ((basic_h_local_delta = (r_1) + (r_2)), \
   (basic_h_v1.x = (q).x - (p).x), \
   (basic_h_v1.y = (q).y - (p).y), \
   (basic_h_local = DotProduct2D(basic_h_v1,basic_h_v1) - \
                    basic_h_local_delta * basic_h_local_delta), \
   (le(basic_h_local, eps)))



#define Roots2abc(a, b, c, roots, num_roots) \
{ \
   assert(eq(ZERO2 - ZERO * ZERO, ZERO)); \
   if (eq(a, ZERO)) { \
      if (eq(b, ZERO)) { \
         if (eq(c, ZERO)) { \
            num_roots = -1; \
         } \
         else { \
            num_roots = 0; \
         } \
      } \
      else { \
         roots[0] = - c / b; \
         num_roots = 1; \
      } \
   } \
   else { \
      basic_h_local_delta = b * b - 4 * a * c; \
      if (basic_h_local_delta > 0.0) { \
         if (b > 0) { \
            basic_h_local = - 0.5 * (b + sqrt(basic_h_local_delta)); \
         } \
         else { \
            basic_h_local = - 0.5 * (b - sqrt(basic_h_local_delta)); \
         } \
         if (eq(basic_h_local, ZERO)) { \
            roots[0] = basic_h_local / a; \
            num_roots = 1; \
         } \
         else { \
            roots[0] = basic_h_local / a; \
            roots[1] = c / basic_h_local; \
            num_roots = 2; \
         } \
      } \
      else if (eq(basic_h_local_delta, ZERO2)) { \
         roots[0] = - b / (2.0 * a); \
         num_roots = 1; \
      } \
      else { \
         num_roots = 0; \
      } \
   } \
}




#define Roots2pq(p, q, roots, num_roots) \
{ \
   assert(eq(ZERO2 - ZERO * ZERO, ZERO)); \
   basic_h_local_delta = p * p - 4 * q; \
   if (basic_h_local_delta > 0.0) { \
      if (p > 0) { \
         basic_h_local = - 0.5 * (p + sqrt(basic_h_local_delta)); \
      } \
      else { \
         basic_h_local = - 0.5 * (p - sqrt(basic_h_local_delta)); \
      } \
      if (eq(basic_h_local, ZERO)) { \
         roots[0] = basic_h_local; \
         num_roots = 1; \
      } \
      else { \
         roots[0] = basic_h_local; \
         roots[1] = q / basic_h_local; \
         num_roots = 2; \
      } \
   } \
   else if (eq(basic_h_local_delta, ZERO2)) { \
      roots[0] = - 0.5 * p; \
      num_roots = 1; \
   } \
   else { \
      num_roots = 0; \
   } \
}



/*                                                                           */
/* this macro checks whether the point  P  lies within the open interior of  */
/* the 2D triangle  A,B,C.                                                   */
/*                                                                           */
/* Warnings: * It is assumed that the triangle is oriented CCW.              */
/*           * Grazing contacts and degenerate triangles may be handled      */
/*             incorrectly.              */
/*                                                                           */
#define TriPnt2D(A, B, C, P) \
((Det2D(A, B, P) <= 0.0) ? 0 : \
 ((Det2D(B, C, P) <= 0.0) ? 0 : \
  ((Det2D(C, A, P) <= 0.0) ? 0 : 1)))
