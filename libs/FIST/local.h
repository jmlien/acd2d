/*****************************************************************************/
/*                                                                           */
/*            Copyright (C)             2000            M. Held              */
/*                                                                           */
/* This code is not in the public domain. All rights reserved! Please make   */
/* sure to read the full copyright statement contained in "README" or in the */
/* "main" file of this code, such as "main.c".                               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/* Written by:  Martin Held                                                  */
/* Date:        Aug 30, 2000                                                 */
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
/* the following variables are static variables within martin.h, used only   */
/* within that file. unfortunately, when defined there as "static", gcc      */
/* barks that some variable has been defined but is not used. without the    */
/* "static" definition, g++ on Suns and cc on SGIs complains that variables  */
/* are multiply defined...                                                   */
/*                                                                           */
double martin_h_local;

#endif


#ifndef _BASIC_H_
#define _BASIC_H_
/*                                                                           */
/* the following variables are static variables within basic.h, used only    */
/* within that file. unfortunately, when defined there as "static", gcc      */
/* barks that some variable has been defined but is not used. without the    */
/* "static" definition, g++ on Suns and cc on SGIs complains that variables  */
/* are multiply defined...                                                   */
/*                                                                           */
double basic_h_local, basic_h_local_min, basic_h_local_max;
double basic_h_local_quot, basic_h_local_delta;
double basic_h_local_a, basic_h_local_b;
vertex basic_h_v1, basic_h_v2;
long basic_i_local;

#endif


#ifndef _MATRIX_H_
#define _MATRIX_H_
/*                                                                           */
/* the following variables are static variables within matrix.h, used only   */
/* within that file. unfortunately, when defined there as "static", gcc      */
/* barks that some variable has been defined but is not used. without the    */
/* "static" definition, g++ on Suns and cc on SGIs complains that variables  */
/* are multiply defined...                                                   */
/*                                                                           */
double matrix_h_sine, matrix_h_cosine;
double matrix_h_local;

#endif


#ifndef _NUMERICS_H_
#define _NUMERICS_H_
/*                                                                           */
/* the following variables are static variables within numerics.h, used only */
/* within that file. unfortunately, when defined there as "static", gcc      */
/* barks that some variable has been defined but is not used. without the    */
/* "static" definition, g++ on Suns and cc on SGIs complains that variables  */
/* are multiply defined...                                                   */
/*                                                                           */
point numerics_h_p, numerics_h_q, numerics_h_r;
double numerics_h_det, numerics_h_dot;
int numerics_h_ori1, numerics_h_ori2;

#endif


#ifndef _BVTREE_H_
#define _BVTREE_H_
/*                                                                           */
/* the following variables are static variables within bv_tree.h, used only  */
/* within that file. unfortunately, when defined there as "static", gcc      */
/* barks that some variable has been defined but is not used. without the    */
/* "static" definition, g++ on Suns and cc on SGIs complains that variables  */
/* are multiply defined...                                                   */
/*                                                                           */
tree_node *bv_tree_ptr;

#endif
