//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _EV_SHORTEST_PATH_H_
#define _EV_SHORTEST_PATH_H_

#include "acd2d_ev_data.h"

namespace acd2d
{
	
	int g_id=0;
	typedef pair<Point2d, Point2d> Edge;
	
	///////////////////////////////////////////////////////////////////////////////
	// check if r,s,t make a right turn
	inline bool turn_right(const Point2d& r, const Point2d& s, const Point2d& t)
	{
		return ((s[0]-r[0])*(t[1]-s[1])-(s[1]-r[1])*(t[0]-s[0]))<0;
	}
	
	inline bool turn_left(const Point2d& r, const Point2d& s, const Point2d& t)
	{
		return ((s[0]-r[0])*(t[1]-s[1])-(s[1]-r[1])*(t[0]-s[0]))>0;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// check if t is convex on the path from end pt of the edge to t
	inline bool isconvex(ev_vertex * t)
	{
		ev_vertex * p=t->cusp[g_id];
		if( p==NULL ) return true;
		ev_vertex * pp=t->cusp[g_id]->cusp[g_id];
	
		if( pp==NULL ) return true;
		if( p->is_convex[g_id]==false ) return false;
		
		if(g_id==0)
			return !turn_left(pp->v->getPos(),p->v->getPos(),t->v->getPos());
		else
			return !turn_right(pp->v->getPos(),p->v->getPos(),t->v->getPos());
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// return the distance from vertex t to the edge
	// if the path of t to the edge is not perpendicular to the edge
	// a large number is returned.
	inline double dist(ev_vertex * t, const Edge& edge)
	{
		ev_vertex * p1=t->cusp[0];
		ev_vertex * p2=t->cusp[1];
		if( p1==NULL || p2==NULL ) return 0;
	
		Vector2d v1=p1->v->getPos()-t->v->getPos();
		Vector2d v2=p2->v->getPos()-t->v->getPos();
		Vector2d v=edge.first-edge.second;
		double dir1=v1*v;
		double dir2=v2*v;
		if( dir1*dir2>0 )  
			return 1e20; //can't see through a perpendicular line
		Vector2d n(v[1],-v[0]);
		n=n.normalize();
		return fabs(n*(edge.first-t->v->getPos()));
	}
	
	///////////////////////////////////////////////////////////////////////////////
	inline void identify_edge_visibility( ev_vertex * t, const Edge& edge)
	{
		typedef list<ev_vertex *>::iterator VIT;
		for(VIT iv=t->children[g_id].begin();iv!=t->children[g_id].end();iv++){
			
			ev_vertex * vx=*iv;
	
			//check convex
			vx->is_convex[g_id]=isconvex(vx);
	
			//find distance
			double dist1=1e18;
			if( vx->is_convex[0] && vx->is_convex[1] ) //can see edge
				dist1=dist(vx,edge);
	
			double dist2=(t->v->getPos()-vx->v->getPos()).norm()+t->dist;
	
			if( vx->dist>dist1 ){ 
				vx->dist=dist1;
				vx->parent=NULL;
			}
			if( vx->dist>dist2 ){
				vx->dist=dist2;
				vx->parent=t;
			}
	
			//decent to children
			identify_edge_visibility(vx,edge);
		}//end for
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// identify the parent.
	// vertically visible vertex has parent as null (parent is the edge)
	// the edge is (t1, t2)
	// find the shortest path for path of the vertices by the way
	inline void identify_edge_visibility
	( ev_vertex * t1, ev_vertex * t2 )
	{
		Edge edge(t1->v->getPos(),t2->v->getPos());
		t1->dist=0;
		t2->dist=0;
		g_id=0; //reset gid
		identify_edge_visibility(t1,edge);
		g_id=1; //reset gid
		identify_edge_visibility(t2,edge);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// find the path recursively
	// Well...this didn't actually find a path but assign the distance
	// from the root the t's children
	inline void shortest_path( ev_vertex * t )
	{
		typedef list<ev_vertex *>::iterator VIT;
		list<ev_vertex *>& children=t->children[g_id];
	
		for(VIT iv=children.begin();iv!=children.end();iv++){
			ev_vertex * vx=*iv;
			double dist=(t->v->getPos()-vx->v->getPos()).norm()+t->dist;
			if( vx->dist>dist ){
				vx->dist=dist;
				vx->parent=t;
			}
			//decent to children
			shortest_path(vx);
		}//end for
	}
	
	// call shortest_path( ev_vertex * t )
	// find the shortest path either from t1 or t2
	// to their children. (note: they may share decendents)
	inline void shortest_path( ev_vertex * t1, ev_vertex * t2 )
	{
		g_id=0; //reset gid
		shortest_path(t1);
		g_id=1; //reset gid
		shortest_path(t2);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// Find the region ( a list of vertices ) that is blocked by
	// two vertices that can see edge vertically
	// @parameters:
	// size  : the size of pts
	// shade : returned value in which each element contains two vertice id that can
	//         see the edge vertically
	
	inline void find_shaded_region_in_B
	( list< pair<int,int> >& shade, ev_vertex * pts, int size )
	{
		int end_pts=size-1;
		bool meet_first_bar=false;
		for( int i=1;i<end_pts;i++ ){
			if( pts[i].parent==NULL ){meet_first_bar=true; continue;} //viz vertex
			if(!meet_first_bar) continue;
			int start=i-1;
			while(i<end_pts && pts[i].parent!=NULL){ i++; }
			int end=i;
			if( end==end_pts ){ break; /*this is region C*/ }
			shade.push_back(pair<int,int>(start,end));
		}
	}

}//end namespace acd2d

#endif //_EV_SHORTEST_PATH_H_

