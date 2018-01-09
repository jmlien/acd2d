//------------------------------------------------------------------------------
//  Copyright 2010-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------


#ifndef _H_UTILITY_ACD2D
#define _H_UTILITY_ACD2D
#pragma once

#include <math.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>   // define C++ stream I/O routines
#include <iomanip>
using namespace std;

namespace acd2d
{

    /* range of real numbers */
    #define SMALLNUMBER 1.0e-10
    #define HUGENUMBER  1.0e10

    /* Miscellaneous Scalar Math */
    #define abs(x)      (((x) < 0) ? (-(x)) : (x))
    #define sqr(x)      ((x) * (x))
    
    /* Angle Conversions & Constants */

    #ifndef PI
    #define PI 3.1415926535897f
    #endif

    #ifndef PI2
    #define PI2 6.2831853071794f
    #endif

    #define RAD2DEG (180/PI)
    #define DEG2RAD (PI/180)

    #define DegToRad(x) ((x)*DEG2RAD)
    #define RadToDeg(x) ((x)*RAD2DEG)


    #ifdef _WIN32

    ////////////////////////////////////////////////////////////////////////////////////////
    // Following functions define M_PI and drand48, which are not starndard c library and 
    // definitions. In addition, rint used to round off float points to int is also here.
    /////////////////////////////////////////////////////////////////////////////////////////

    #define M_PI 3.1415926 //reference PI above

    extern "C" {
        //Implementation of these functions are located in util.cpp
        double drand48();
        double erand48(register unsigned short *xsubi);
        long irand48(register unsigned short m);
        long krand48(register unsigned short *xsubi, unsigned short m);
        long lrand48();
        long mrand48();
        static void next();
        void srand48(long seedval);
        unsigned short * seed48(unsigned short seed16v[3]);
        void lcong48(unsigned short param[7]);
        long nrand48(register unsigned short *xsubi);
        long jrand48(register unsigned short *xsubi);

        /**Round to closest integer.
          *The rint() function rounds x to an integer value according
          *to the prevalent rounding mode.  The default rounding mode
          *is to round to the nearest integer.
          *@return The  rint() function returns the integer value as a float-
          *ing-point number.
          */
        double rint(double x);

    } //end extern "C"

    #endif //_WIN32

} //end of acd2d

#endif
