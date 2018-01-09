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
/* Date:        Nov 15, 1993                                                 */
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
 

#ifndef _MARTIN_H_
#define _MARTIN_H_
/*                                                                           */
/* the following variables are static variables, used only within this file. */
/* unfortunately, when defined as "static", gcc barks that some variable     */
/* has been defined but is not used. without the "static" definition, cc on  */
/* SGIs complains that variables are multiply defined...                     */
/*                                                                           */
extern double martin_h_local;

#endif



/*                                                                           */
/* defines my boolean data type                                              */
/*                                                                           */
#ifdef BOOL_DEFINED
typedef bool boolean;
#else
#define false 0
#define true  (!false)
typedef unsigned char  boolean;
#endif


/*                                                                           */
/*some useful constants; some of them may be defined in  math.h              */
/*

                                                                             */
#ifdef LIB_CORE

#ifndef M_E
#define M_E             "2.71828182845904553488"
#endif
#ifndef M_1_E
#define M_1_E           "0.36787944117144227851"
#endif
#ifndef M_LOG2E
#define M_LOG2E         "1.44269504088896360904"
#endif
#ifndef M_LOG10E
#define M_LOG10E        "0.43429448190325182765"
#endif
#ifndef M_LN2
#define M_LN2           "0.69314718055994530942"
#endif
#ifndef M_LN10
#define M_LN10          "2.30258509299404568402"
#endif
#ifndef M_PI
#define M_PI            "3.14159265358979323846"
#endif
#ifndef M_PI_2
#define M_PI_2          "1.57079632679489661923"
#endif
#ifndef M_PI_4
#define M_PI_4          "0.78539816339744830962"
#endif
#ifndef M_1_PI
#define M_1_PI          "0.31830988618379067154"
#endif
#ifndef M_2_PI
#define M_2_PI          "0.63661977236758134308"
#endif
#ifndef M_2PI
#define M_2PI           "6.28318530717958623199"
#endif
#ifndef M_2_SQRTPI
#define M_2_SQRTPI      "1.12837916709551257390"
#endif
#ifndef M_SQRT2
#define M_SQRT2         "1.41421356237309504880"
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2       "0.70710678118654752440"
#endif
#ifndef M_SQRT3
#define M_SQRT3         "1.73205080756887719317"
#endif
#ifndef M_SQRT1_3
#define M_SQRT1_3       "0.57735026918962584208"
#endif

#else

#ifndef M_E
#define M_E             2.71828182845904553488
#endif
#ifndef M_1_E
#define M_1_E           0.36787944117144227851
#endif
#ifndef M_LOG2E
#define M_LOG2E         1.44269504088896360904
#endif
#ifndef M_LOG10E
#define M_LOG10E        0.43429448190325182765
#endif
#ifndef M_LN2
#define M_LN2           0.69314718055994530942
#endif
#ifndef M_LN10
#define M_LN10          2.30258509299404568402
#endif
#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2          1.57079632679489661923
#endif
#ifndef M_PI_4
#define M_PI_4          0.78539816339744830962
#endif
#ifndef M_1_PI
#define M_1_PI          0.31830988618379067154
#endif
#ifndef M_2_PI
#define M_2_PI          0.63661977236758134308
#endif
#ifndef M_2PI
#define M_2PI           6.28318530717958623199
#endif
#ifndef M_2_SQRTPI
#define M_2_SQRTPI      1.12837916709551257390
#endif
#ifndef M_SQRT2
#define M_SQRT2         1.41421356237309504880
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2       0.70710678118654752440
#endif
#ifndef M_SQRT3
#define M_SQRT3         1.73205080756887719317
#endif
#ifndef M_SQRT1_3
#define M_SQRT1_3       0.57735026918962584208
#endif

#endif

/*                                                                           */
/* these macros are convenient...                                            */
/*                                                                           */
#define Min(a, b)     ((a) < (b) ? (a) : (b))
#define Max(a, b)     ((a) <= (b) ? (b) : (a))
#define Max3(a, b, c) (((a) > (b)) ? (((a) > (c)) ? (a) : (c))\
: (((b) > (c)) ? (b) : (c)))
#define Min3(a, b, c) (((a) < (b)) ? (((a) < (c)) ? (a) : (c))\
: (((b) < (c)) ? (b) : (c)))
#define Sign(x) \
(martin_h_local = x, \
 ((martin_h_local > 0.0) ? 1 : ((martin_h_local < 0.0) ? -1 : 0)))
#define Abs(x)        (((x) >= 0.0) ? (x) : -(x))
#define Square(x)     ((x) * (x))
/* #define xor(a, b)     (!(a) ^ !(b)) */
#define Odd(x)        ((x) % 2 == 1)
#define Even(x)       (! Odd(x))
#define Swap(i1, i2, i) \
{i  = i1; \
 i1 = i2; \
 i2 = i; }

/*                                                                           */
/* some macros for epsilon-based comparisons with respect to zero...         */
/*                                                                           */
#define lt(a, eps) ((a) < -eps)
#define le(a, eps) ((a) <= eps)
#define ge(a, eps) (!((a) < -eps))
#define eq(a, eps) (((a) <= eps) && !((a) < -eps))
#define gt(a, eps) (!((a) <= eps))


