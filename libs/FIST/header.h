/*****************************************************************************/
/*                                                                           */
/*            Copyright (C)           1997-2002         M. Held              */
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
 
/*                                                                           */
/* descriptions of procedure headers used                                    */
/*                                                                           */

/* api_functions.c: */

void HandlePolyhedron(boolean write_tri, char tri_file[], boolean write_geom, 
                      boolean time, char input_file[]);

void HandlePolygon(char output_file[], boolean save_polygon, 
                   int step_size, boolean write_geom, boolean write_tri,
                   char tri_file[], boolean write_ipe, boolean time,
                   int io_flag, char input_file[]);

void FIST_PolygonalArray(int num_contours, int num_vertices[], 
                         double (*input_vtx)[2], int *num_triangles,
                         int (*output_tri)[3]);



/* compute.c: */

void Compute(char output_file[], boolean save_polygon, boolean new_input,
             int step_size, boolean write_geom, boolean write_tri,
             char tri_file[], boolean write_ipe, boolean time,
             double *cpu_time);

void TerminateProgram(double cpu_time);

void ResetAll(void);




/* memory.c: */

void *ReallocateArray(void *old_ptr, int number, size_t size, 
                      char var_name[]);

#ifdef DEBUG_MEMORY
boolean IndexOutOfBounds(void *array_ptr, char var_name[], size_t size, 
                         int index);
#endif

void FreeMemory(void **ptr, char var_name[]);

unsigned machine_long ReportMaxNumberBytes(void);

unsigned machine_long ReportCurrNumberBytes(void);

boolean AllMemoryFreed();




/* data.c: */

#ifdef EXT_APPL_SITE
int StorePnt(double x, double y, int ext_appl);
#else
int StorePnt(double x, double y);
#endif

void InitPnts(int number);

void FreePnts();

void DecrementPoints(void);

void SetBoundingBox(point *bb_min, point *bb_max);

boolean InPointsList(int index);

void InitStoragePnts(int number);




/* redraw.c: */

#ifdef GRAPHICS
void ResetBufferData(void);

void Redraw(void);

void AddPntToBuffer(int index, int color);

void AddEdgeToBuffer(int index1, int index2, int color);

void AddTriToBuffer(int index1, int index2, int index3, int color1, 
                    int color2);

void DecrementPntBuffer(void);

void DecrementEdgeBuffer(void);

void FreeDrawingBuffer(void);

void ResetTriBuffer(void);

void UpdatePntEdgeBuffers(void);
#endif


/* graphics.c: */

#ifdef GRAPHICS
void FlushDrawEdge(point pnt1, point pnt2, int color);

void FlushDrawTri(point pnt1, point pnt2, point pnt3, int color1, int color);

void DrawTri(point pnt1, point pnt2, point pnt3, int color1, int color);

void DrawEdge(point pnt1, point pnt2, int color);

void FlushDrawPnt(point pnt, int color);

void DrawPnt(point pnt, int color);

boolean ClipPoint(int x, int y);

boolean ClipLine(int xs, int ys, int xe, int ye);

void ResetGraphicsData(void);
#endif



/* io.c: */

void CopyOutputData(int *num_tri, int (*output_tri)[3]);

void CopyInputData(int num_contours, int *num_vtx, double (*input_vtx)[2]);

void Help(void);

FILE *OpenFile(char *file_name, char *access);

void Error(char string[]);

void Copyright(void);

void UserInteraction(int *imax, int *jmax, int screen_i, int screen_j);

void ReadPolygon(char input_file[], boolean *new_input);

void ReadPoly(char input_file[], boolean *new_input);

void WritePolygon(char output_file[]);

void WriteGeomOutput(void);

void WriteFaces(char output_file[]);

void ReadPolyhedron(char input_file[]);




/* arg_eval.c: */

boolean ArgEval(int argc, char *argv[], boolean *color_graphics,
                boolean *graphics, boolean *save_poly, char *output_file,
                boolean *read_input, char *input_file,  boolean *read_poly,
                boolean *do_quads, boolean *keep_quads,
                int *step_size, boolean *write_geom, boolean *verbose,
                boolean *read_obj, boolean *write_tri, char *tri_file,
                boolean *write_ipe, boolean *help, boolean *time,
                boolean *statistics, boolean *ears_sorted, 
                boolean *ears_strip, boolean *ears_random, boolean *ears_fancy,
                boolean *copy, boolean *use_colors,
                boolean *draw_concave, boolean *sgi_output, boolean *quiet,
                boolean *draw_groups, boolean *convex_faces, 
                boolean *make_convex); 

void EvalError(void);




/* list.c: */

int GetNumList(void);

void InitStorageList(int number);

void InitStorageLoops(int number);

void StoreChain(list_ind ind);

list_ind GetNextChain(boolean *done);

void ResetListData(void);

void RotateLinks(list_ind ind1, list_ind ind2);

void SplitSplice(list_ind ind1, list_ind ind2, list_ind ind3, list_ind ind4);

int GetOriginal(list_ind ind);

#ifdef NORMALS
void SetTexNormData(list_ind ind, int index1, int index2);

int GetNormalIndex(list_ind ind);

int GetTextureIndex(list_ind ind);
#endif

void SetOriginal(list_ind ind, int original);

int GetAngle(list_ind ind);

void SetAngle(list_ind ind, int convex);

list_ind FetchPrevData(list_ind ind1, int *index);

list_ind FetchNextData(list_ind ind1, int *index);

void FetchData(list_ind ind1, int *index);

void DeleteLinks(list_ind ind);

void InsertAfter(list_ind ind1, list_ind ind2);

list_ind MakeNode(int index);

int MakeLoopHeader(void);

list_ind MakeHook(void);

void FreeList(void);

void DeleteHook(int curr_loop);

int CountListElements(list_ind ind1);

void DecrementLoops(void);

void UpdateIndex(list_ind ind, int index);

void SwapLinks(list_ind ind1);

boolean InPolyList(list_ind ind);

boolean InLoopList(int loop);

void ResetPolyList(list_ind ind);

list_ind GetNode(void);

void InitFace(int number);

void DecrementFaces(void);



/* clean_data.c: */

void CleanPolygon(int *removed);

void CleanPolyhedralFace(int loop_min, int loop_max, int *removed);

void FreeUnsorted(void);




/* numerics.c: */

double Angle(point p, point p1, point p2);

int IsConvex(int i1, int i2, int i3, list_ind ind);

boolean InCone(int i, int j, int k, int l, boolean convex);

boolean SegIntersect(int i1, int i2, int i3, int i4, int i5);

double GetRatio(int i, int j, int k);

double det2D(int i, int j, int k);

int orientation(int i, int j, int k);

void SetEpsilon(double eps);

int SpikeAngle(int i, int j, int k, list_ind ind);

boolean PointInTriangle(int i1, int i2, int i3, int i4);


/* orientation.c: */

void AdjustOrientation(int i1, int i2);

void DetermineOrientation(list_ind ind);

double PolygonArea(list_ind ind);

void FreeOrientation(void);




/* ear_clip.c: */

boolean IsEar(list_ind ind2, list_ind *ind1, list_ind *ind3, double *ratio);

void ClassifyEars(list_ind ind);

void ClassifyAngles(list_ind ind);

boolean ClipEar(boolean *done);

void SetConvexityStatus(boolean status);

void SetTangentNumber(int number);



/* heap.c: */

void FreeHeap(void);

void DumpOnHeap(double ratio, list_ind ind, list_ind prev, list_ind next);

void InsertIntoHeap(double ratio, list_ind ind, list_ind prev, list_ind next,
                    boolean left_right);

boolean DeleteFromHeap(list_ind *ind, list_ind *prev, list_ind *next);

void InitHeap(void);

void MakeHeap(void);




/* vertex.c: */

boolean InVertexList(int index);

void FreeVertices();

void InitVertices(int number);

#ifdef EXT_APPL_SITE
int StoreVertex(double x, double y, double z, int ext_appl);
#else
int StoreVertex(double x, double y, double z);
#endif

void FreeTriangles();

void StoreTriangle(int i, int j, int k, int color);

void InitTriangles(int number);

void StoreQuad(int i, int j, int k, int l);

void FreeQuads();

void InitQuads(int number);

void FreeGroups(void);

boolean InGroupList(int index);

void InitGroupTriangles(void);

void StoreGroupNumber(void);

void StoreGroupData(void);

void StoreConvexLoop(int i);

void FreeConvexLoops(void);

#ifdef NORMALS
int StoreV_Normal(double x, double y, double z);

void InitV_Normals(int number);

void FreeV_Normals();

boolean InV_NormalList(int index);

int StoreT_Vertex(double x, double y, double z);

void InitT_Vertices(int number);

void FreeT_Vertices();

boolean InT_VertexList(int index);

void FreeI_Triangles();
#endif



/* misc.c: */

void Fake3D(void);




/* desperate.c: */

boolean Desperate(list_ind ind, int i, boolean *splitted);

boolean LetsHope(list_ind ind);

void FreeDistances(void);

int WindingNumber(list_ind ind, point p);




/* bridge.c: */

void FindLeftMostVertex(list_ind ind, list_ind *left_ind, int *left_i);

void ConstructBridges(int i1, int i2);

boolean FindBridge(list_ind ind, int i, int start, list_ind *ind1, int *i1);

void InsertBridge(list_ind ind1, int i1, list_ind ind3, int i3);

int d_comp();
 
void FreeBridges(void);




/* project.c: */

void ProjectFace(int loop_min, int loop_max);




/* triangulate.c: */

void Triangulate(boolean write_tri, char tri_file[], boolean write_geom, 
                 boolean time, double *cpu_time);




/* write_ipe.c: */

void WriteIpePolygon(void);

void WriteIpeOutput(void);




/* simple.c: */

boolean SimpleFace(list_ind ind1);

boolean TrivialPolygon(list_ind ind1);



/* elapsed.c: */

double elapsed();




/* statistics.c: */

void Statistics(double *min_angle, double *avg_min_angle, int num_dist,
                double angle_dist[]);




/* bv_tree.c: */

boolean TreeIntersectionExists(bounding_box bb, int i1, int i2,
                               list_ind ind5, int i5);

void FreeTree(void);

void BuildTree(int loop_min, int loop_max);

void InsertSegmentIntoTree(bounding_box bb);

boolean DeleteSegmentFromTree(tree_ind node, bounding_box bb, 
                              boolean *deleted);

void UpdateTree(int i1, int i2, int i3);

double TopQuality(int i1, int i2, bounding_box_extended bb_e);




/* grid.c: */

boolean GridIntersectionExists(bounding_box bb, int i1, int i2,
                               list_ind ind5, int i5);

void InsertSegmentIntoGrid(bounding_box bb);

void BuildGrid(int loop_min, int loop_max);

void FreeGrid(void);

double TopQualityGrid(int i1, int i2);

void BuildPntsGrid(int loop_ind);

void BuildBuckets(int i);

boolean BucketIntersectionExists(int i1, list_ind ind1, int i2, int i3, 
                                 bounding_box bb);

void DeleteReflexVertex(list_ind ind);




/* bottleneck.c: */

boolean CheckBottleNeck(int i1, int i2, int i3, list_ind ind4);




/* quads.c: */

void DetermineQuads(void);



/* degenerate.c: */

boolean HandleDegeneracies(int i1, list_ind ind1, int i2, int i3, int i4, 
                           list_ind ind4);



/* thin.c: */

void Thin(char output_file[], boolean save_polygon, boolean new_input,
          int keep_pnts, boolean time, double *cpu_time);



/* no_hash.c: */

void PrepareNoHashEdges(int curr_loop_min, int curr_loop_max);

void FreeNoHash(void);

void PrepareNoHashPnts(int curr_loop_min);

boolean NoHashIntersectionExists(int i1, list_ind ind1, int i2, int i3, 
                                 bounding_box bb);

void DeleteReflexVertex(list_ind ind);

boolean NoHashEdgeIntersectionExists(bounding_box bb, int i1, int i2,
                                     list_ind ind5, int i5);
