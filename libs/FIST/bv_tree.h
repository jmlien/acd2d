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

#ifndef _BVTREE_H_
#define _BVTREE_H_
/*                                                                           */
/* the following variables are static variables, used only within this file. */
/* unfortunately, when defined as "static", gcc barks that some variable     */
/* has been defined but is not used. without the "static" definition, cc on  */
/* SGIs complains that variables are multiply defined...                     */
/*                                                                           */
extern tree_node *bv_tree_ptr;

#endif




#define Pnt_In_BBox(bb1, i)  \
   ((((bb1).imax < i) ? false : \
     (((bb1).imin > i) ? false : \
      (((bb1).ymax < points[i].y) ? false : \
       (((bb1).ymin > points[i].y) ? false : true)))))



#ifdef STATISTICS
#define BBox_Overlap(bb1, bb2)  \
   (++bb_cntr, \
    (((bb1).imax < (bb2).imin) ? false : \
     (((bb1).imin > (bb2).imax) ? false : \
      (((bb1).ymax < (bb2).ymin) ? false : \
       (((bb1).ymin > (bb2).ymax) ? false : true)))))
#else
#define BBox_Overlap(bb1, bb2)  \
   ((((bb1).imax < (bb2).imin) ? false : \
     (((bb1).imin > (bb2).imax) ? false : \
      (((bb1).ymax < (bb2).ymin) ? false : \
       (((bb1).ymin > (bb2).ymax) ? false : true)))))
#endif


#ifdef STATISTICS
#define BBox_Contained(bb1, bb2)  \
   (++bb_cntr, \
    ((bb1).imin <= (bb2).imin)  &&  ((bb1).imax >= (bb2).imax)  && \
    ((bb1).ymin <= (bb2).ymin)  &&  ((bb1).ymax >= (bb2).ymax))
#else
#define BBox_Contained(bb1, bb2)  \
   (((bb1).imin <= (bb2).imin)  &&  ((bb1).imax >= (bb2).imax)  && \
    ((bb1).ymin <= (bb2).ymin)  &&  ((bb1).ymax >= (bb2).ymax))
#endif



#define BBox_IdenticalLeaf(bb1, bb2)  \
   (((bb1).imin == (bb2).imin)  &&  ((bb1).imax == (bb2).imax))



#define BBox_Union(bb1, bb2, bb3)  \
 { (bb3).imin = Min((bb1).imin, (bb2).imin); \
   (bb3).imax = Max((bb1).imax, (bb2).imax); \
   (bb3).ymin = Min((bb1).ymin, (bb2).ymin); \
   (bb3).ymax = Max((bb1).ymax, (bb2).ymax); }



#define BBox_Area(bb, area) \
 { area = (points[(bb).imax].x - points[(bb).imin].x) * \
          ((bb).ymax - (bb).ymin); }



#define BBox_Copy(bb1, bb2) \
 { (bb2).imin = (bb1).imin; \
   (bb2).imax = (bb1).imax; \
   (bb2).ymin = (bb1).ymin; \
   (bb2).ymax = (bb1).ymax; }



#define BBox_CopyInverted(bb1, bb2) \
 { (bb2).imin = (bb1).imax; \
   (bb2).imax = (bb1).imin; \
   (bb2).ymin = (bb1).ymax; \
   (bb2).ymax = (bb1).ymin; }



#define BBox_Enlarge(bb1, bb2) \
 { if ((bb1).imin < (bb2).imin)    (bb2).imin = (bb1).imin; \
   if ((bb1).imax > (bb2).imax)    (bb2).imax = (bb1).imax; \
   if ((bb1).ymin < (bb2).ymin)    (bb2).ymin = (bb1).ymin; \
   if ((bb1).ymax > (bb2).ymax)    (bb2).ymax = (bb1).ymax; }




/*                                                                           */
/* this macro computes the bounding box of a line segment whose end          */
/* points  i, j  are sorted according to x-coordinates.                      */
/*                                                                           */
#define BBox(i, j, bb) \
 { assert(InPointsList(i)); \
   assert(InPointsList(j)); \
   \
   MinMax(i, j, (bb).imin, (bb).imax); \
   MinMax(points[(bb).imin].y, points[(bb).imax].y, (bb).ymin, (bb).ymax); }




#ifdef STATISTICS
#define BBox_Overlap_Extended(bb1, bb2)  \
   (++bb_cntr, \
    (((bb1).ymax < (bb2).ymin) ? false : \
     (((bb1).ymin > (bb2).ymax) ? false : \
      (((bb1).xmax < points[(bb2).imin].x) ? false : \
       (((bb1).xmin > points[(bb2).imax].x) ? false : true)))))
#else
#define BBox_Overlap_Extended(bb1, bb2)  \
   ((((bb1).ymax < (bb2).ymin) ? false : \
     (((bb1).ymin > (bb2).ymax) ? false : \
      (((bb1).xmax < points[(bb2).imin].x) ? false : \
       (((bb1).xmin > points[(bb2).imax].x) ? false : true)))))
#endif

