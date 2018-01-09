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
 
/* graphics.c */

#ifdef GRAPHICS

void SetBB(machine_double *xl, machine_double *yl, machine_double *xr, machine_double *yr,
           machine_double xl_bb, machine_double yl_bb, machine_double xr_bb, machine_double yr_bb);

void InitializeMapping(machine_double xl, machine_double yl, machine_double xr, machine_double yr,
                       int imax, int jmax, machine_double *scale,
                       machine_double *xm, machine_double *ym, int *im, int *jm);

void DrawLine(Display *mydisplay, Window  mywindow, GC mygc,
              int im, int jm, machine_double scale, machine_double xm, machine_double ym,
              machine_double xc1, machine_double yc1, machine_double xc2, machine_double yc2);

void DrawTriangle(Display *mydisplay, Window  mywindow, GC mygc,
                  int im, int jm, machine_double scale, machine_double xm, machine_double ym,
                  machine_double xc1, machine_double yc1, machine_double xc2, machine_double yc2,
                  machine_double xc3, machine_double yc3);

void DrawFilledTriangle(Display *mydisplay, Window  mywindow, GC mygc,
                        int im, int jm, machine_double scale, machine_double xm, machine_double ym,
                        machine_double xc1, machine_double yc1, machine_double xc2, machine_double yc2,
                        machine_double xc3, machine_double yc3);

void DrawFilledCircle(Display *mydisplay, Window  mywindow, GC mygc,
                      int im, int jm, machine_double scale, machine_double xm, machine_double ym,
                      machine_double xc1, machine_double yc1, unsigned int radius);

void MapWorldCoordinates(machine_double scale, machine_double xm, machine_double ym, int im, int jm,
                         machine_double x, machine_double y, int *i, int *j);

void MapWindowCoordinates(machine_double scale, machine_double xm, machine_double ym, int im, int jm,
                          int i,int j, machine_double *x, machine_double *y);

void ConsumeTime(void);

void ChooseCursor(int cursor_shape);

void ChooseSprayCanCursor(void);

void SelectInputEvents(void);

void InitGraphicsContext(void);

void CreateWindow(int argc, char *argv[]);

void SetWindowPositionSize(void);

void SetPixelValues(boolean color_graphics);

void GraphicsStartUp(void);

void FreeGraphics(void);

void QuickDisplay();

void ProcessMouseClick(int curr_loop);

void ClosePolygon(int curr_loop, boolean *new_input);

#endif
