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
 
extern Display *mydisplay;
extern Window   mywindow;
extern GC mygc;
extern XEvent myevent;
extern KeySym mykey;
extern XSizeHints myhint;
extern Cursor mycursor;
extern Colormap mycmap;
extern XColor colors[10];

extern int myscreen;   
extern unsigned machine_long myforeground, mybackground;

extern int imax, jmax, im, jm;
extern machine_double scale, xm, ym, xl, yl, xr, yr;
extern machine_double xl_bb, yl_bb, xr_bb, yr_bb;
extern int width_pxl, height_pxl;
extern boolean skip_points;
extern int first_pnt;

/*                                                                           */
/* colors for X11 drawing                                                    */
/*                                                                           */
#define  Green        1
#define  Blue         2
#define  White        3
#define  Red          4
#define  Cyan         5
#define  Yellow       6
#define  Orange       7
#define  Magenta      8
#define  Black        9
