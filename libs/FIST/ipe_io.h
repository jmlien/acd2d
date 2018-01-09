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
/* Date:        Feb 9, 1996                                                  */
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


FILE *InitIpeFile(char *file_name);

FILE *InitIpe(char *file_name, double xl, double xr, double yl, double yr);

void CloseIpeFile(FILE *ipe_file);

void WriteBeginGroup(FILE *ipe_file);

void WriteEndGroup(FILE *ipe_file);

void WriteEnd(FILE *ipe_file);

void WriteLineBegin(FILE *ipe_file, int dashed, double width, int no_vertices);

void WriteSegmentBegin(FILE *ipe_file, int dashed, double width, 
                        int no_vertices);

void WriteArcBegin(FILE *ipe_file, int dashed, double width);

void WriteObjStroke(FILE *ipe_file, double r, double g, double b);

void WriteObjFill(FILE *ipe_file, double r, double g, double b);

void WriteArcCCW(FILE *ipe_file, double xc, double yc, double r, 
                 double alpha, double beta);

void WriteArcCW(FILE *ipe_file, double xc, double yc, double r, 
                 double alpha, double beta);

void WriteMark(FILE *ipe_file, double r, double g, double b, int type, 
               double size, double x, double y);

void WriteLineFirstPnt(FILE *ipe_file, double x, double y);

void WriteLineNextPnt(FILE *ipe_file, double x, double y);

void WriteSegmentFirstPnt(FILE *ipe_file, double x, double y);

void WriteSegmentNextPnt(FILE *ipe_file, double x, double y);

void WriteSegmentEndPnt(FILE *ipe_file, double x, double y);

void WriteSegmentClosePnt(FILE *ipe_file, double x, double y);

void WriteSplineBegin(FILE *ipe_file, int dashed, double width, int
                      no_vertices); 

void WriteSplineFirstPnt(FILE *ipe_file, double x, double y);

void WriteSplineLastPnt(FILE *ipe_file, double x, double y);

void WriteSplineSecondPnt(FILE *ipe_file, double x, double y);

void WriteSplineNextPnt(FILE *ipe_file, double x, double y);

#ifdef LIB_CORE
void SetIpeDimensions(machine_double xmin, machine_double xmax, machine_double ymin, machine_double ymax);

void SetWorldDimensions(machine_double xmin, machine_double xmax, machine_double ymin, machine_double ymax);
#else
void SetIpeDimensions(double xmin, double xmax, double ymin, double ymax);

void SetWorldDimensions(double xmin, double xmax, double ymin, double ymax);
#endif

void SetScaleFactor(void);

void WritePnt(FILE *ipe_file, double x, double y);

void InitIpeDimensions(double xmin, double ymin, double xmax, double ymax,
                       double ixmin, double iymin, double ixmax, double iymax);

void WriteLineSegment(FILE *ipe_file, double r, double g, double b, 
                      double width, int dashed, 
                      double x1, double y1, double x2, double y2);

void WriteCircularArc(FILE *ipe_file, double r, double g, double b, 
                      double width, int dashed, double xc, double yc, 
                      double x1, double y1, double x2, double y2, boolean ccw);

void WriteDiscretizedCircularArc(FILE *ipe_file, double xc, double yc, 
                                 double x1, double y1, double x2, double y2, 
                                 boolean ccw, double delta, int *num_segs);
