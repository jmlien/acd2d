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
 
#define NIL  -1

#ifndef LIB_CORE
#define machine_double double
#endif

#ifndef LIB_CORE
#define machine_long long
#endif

typedef struct {
   int imin;           /* lexicographically smallest point, determines min-x */
   int imax;           /* lexicographically largest point, determines max-x  */
   double ymin;        /* minimum y-coordinate                               */
   double ymax;        /* maximum y-coordinate                               */
} bounding_box;        /* bounding box                                       */

typedef struct {
   int imin;           /* lexicographically smallest point, determines min-x */
   int imax;           /* lexicographically largest point, determines max-x  */
   double xmin;        /* minimum x-coordinate                               */
   double xmax;        /* maximum x-coordinate                               */
   double ymin;        /* minimum y-coordinate                               */
   double ymax;        /* maximum y-coordinate                               */
} bounding_box_extended;

typedef struct {
   double x;
   double y;
   double z;
#ifdef EXT_APPL_SITE
   int ext_appl;       /* this field can be set by an application program to */
                       /* refer to a user-defined entity.                    */
#endif
} vertex;                                  

typedef struct {
   double x;
   double y;
#ifdef EXT_APPL_SITE
   int ext_appl;       /* this field can be set by an application program to */
                       /* refer to a user-defined entity.                    */
#endif
} point;                                  

typedef struct {
   int v1;
   int v2;
   int v3;
   int color;
#ifdef EXT_APPL_TRI
   int ext_appl;       /* this field can be set by an application program to */
                       /* refer to a user-defined entity.                    */
#endif
} triangle;

#ifdef NORMALS
typedef struct {
   int i1;
   int i2;
   int i3;
} i_triangle;
#endif

typedef struct {
   int v1;
   int v2;
   int v3;
   int v4;
#ifdef EXT_APPL_TRI
   int ext_appl;       /* this field can be set by an application program to */
                       /* refer to a user-defined entity.                    */
#endif
} quadrangle;

typedef int list_ind;

typedef struct {
   list_ind ind;
   boolean ccw;
} loop_list;


typedef struct {
   list_ind ind;
   double dist;
} distance;

typedef int tree_ind;

typedef struct {
   int imin;           /* lexicographically smallest point, determines min-x */
   int imax;           /* lexicographically largest point, determines max-x  */
   double ymin;        /* minimum y-coordinate                               */
   double ymax;        /* maximum y-coordinate                               */
   tree_ind lson;      /* index of left child                                */
   tree_ind rson;      /* index of right child                               */
} tree_node;           /* node of the bounding-volume tree                   */


extern double eps0, eps3;
extern double  LARGE;

extern point *points;
extern int num_pnts;

extern point bb_min;
extern point bb_max;

extern int num_loops;
extern list_ind *loops;

extern int num_faces;
extern int *faces;

extern vertex *vertices;
extern int num_vertices;

#ifdef NORMALS
extern vertex *v_normals;
extern int num_v_normals;

extern vertex *t_vertices;
extern int num_t_vertices;

extern i_triangle *i_triangles;
extern int num_i_triangles;
#endif

extern triangle *triangles;
extern int num_triangles;

extern quadrangle *quads;
extern int num_quads;
extern int num_grouped;

extern int *groups;
extern int num_groups;
extern int *group_tris;
extern int *group_quads;
extern int *group_loops;

extern loop_list *convex_loops;
extern int num_convex_loops;
extern int num_concave_loops;

#ifdef GRAPHICS
extern boolean graphics;
#endif
extern boolean verbose;
extern boolean quiet;
extern boolean draw_pnts;
extern boolean do_quads;
extern boolean keep_quads;
extern boolean keep_convex;
extern boolean is_convex_face;
extern boolean draw_groups;

extern int num_contours;

extern boolean ears_sorted;
extern boolean ears_fancy;
extern boolean ears_random;
extern boolean ears_strip;
extern boolean no_grid_yet;
extern boolean no_hashing_edges;
extern boolean no_hashing_pnts;

#ifdef NORMALS
extern boolean tex_norm_data_exists;
#endif

extern boolean statistics;

extern tree_ind bv_root;

extern unsigned machine_long int_cntr;
extern unsigned machine_long bb_cntr;
extern unsigned machine_long cell_cntr;
extern unsigned machine_long pnt_tri_cntr;

extern boolean ccw_loop;

extern int tri_color;  /* 0 ... output triangle equals input triangle        */
                       /* 1 ... output triangle belongs to input quad        */
                       /* 2 ... output triangle belongs to convex face       */
                       /* 3 ... output triangle belongs to concave face      */
extern boolean use_colors;
extern boolean draw_concave;
#define TriTriColor  0 /* 0 ... output triangle equals input triangle        */
#define TriQuadColor 1 /* 1 ... output triangle belongs to input quad        */
#define TriCvxColor  2 /* 2 ... output triangle belongs to convex face       */
#define TriCveColor  3 /* 3 ... output triangle belongs to concave face      */

extern boolean sgi_output;
extern boolean make_convex;

/*                                                                           */
/* colors for X11 drawing                                                    */
/*                                                                           */
#define  NoColor      0
#define  PolyColor    1
#define  PntsColor    6
#define  TriColor     3
#define  TriFillColor 4
#define  ConvexColor  5
#define  ZeroColor    2
#define  TangentColor 7
#define  EarColor     8
#define  BridgeColor  3
#define  SplitColor   2


#ifdef LIB_CORE                               /* if the CORE library is used */

#define C_0_0         "0.0"
#define C_0_01        "0.01"
#define C_0_1         "0.1"
#define C_0_3         "0.3"
#define C_0_4         "0.4"
#define C_0_5         "0.5"
#define C_0_6         "0.6"
#define C_1_0         "1.0"
#define C_1_1         "1.1"
#define C_2_0         "2.0"
#define C_2_5         "2.5"
#define C_4_0         "4.0"
#define C_10_0        "10.0"
#define C_20_0        "20.0"
#define C_100_0       "100.0"
#define C_180_0       "180.0"

#else

#define C_0_0         0.0
#define C_0_01        0.01
#define C_0_1         0.1
#define C_0_3         0.3
#define C_0_4         0.4
#define C_0_5         0.5
#define C_0_6         0.6
#define C_1_0         1.0
#define C_1_1         1.1
#define C_2_0         2.0
#define C_2_5         2.5
#define C_4_0         4.0
#define C_10_0        10.0
#define C_20_0        20.0
#define C_100_0       100.0
#define C_180_0       180.0

#endif


#ifdef THRESHOLD

#define EPS0     1.0e-8
#define EPS3     1.0e-12

#else

#ifdef LIB_CORE

#define EPS0     "0.0"
#define EPS3     "0.0"

#else

#define EPS0     0.0
#define EPS3     0.0

#endif
#endif


#ifdef MEM_COPY

/*                                                                           */
/* unfortunately, the following stuff is needed in order to make FIST comply */
/* with CORE's memory re-allocation...                                       */
/*                                                                           */

#include <stdlib.h>
#include <stdio.h>

template <class DATATYPE>
DATATYPE* ReallocateArray_Copy(DATATYPE* old_mem, int old_size, int new_size, size_t size, char var_name[])
{
	DATATYPE* new_mem = NULL;

	if (old_mem != NULL)
	{
		new_mem = new DATATYPE[new_size];
		if (new_mem != NULL)
		{
			for (int i = 0; i < old_size; i++)
				new_mem[i] = old_mem[i];
			delete []old_mem;
		}
		else
		{
		 	fprintf(stderr, "*** Array `%s' cannot be reallocated! ***\n", var_name);
         		fprintf(stderr, "*** Cannot get %d elements of %d bytes... ***\n", new_size, size);
         		exit(1);	
		}
	}
	else
	{
		new_mem = new DATATYPE[new_size];
		if (new_mem == NULL)
		{
			fprintf(stderr, "*** Array `%s' cannot be reallocated! ***\n", var_name);
         		fprintf(stderr, "*** Cannot get %d elements of %d bytes... ***\n", new_size, size);
         		exit(1);
		}
	}

	return new_mem;	
}

template <class DATATYPE>
void FreeMemory_Copy(DATATYPE** ptr, char var_name[])
{
	if (*ptr == NULL) return;
	
#ifdef DEBUG_MEMORY
        fprintf(stderr, "\nMDebug: `%s' has been freed\n", var_name);
        UpdateMemoryStatus(*ptr, mem_freed, 0, 0);
#endif /* DEBUG_MEMORY */

	delete []*ptr;
	*ptr = NULL;
}

#endif /* MEM_COPY */
