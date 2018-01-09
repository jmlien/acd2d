//------------------------------------------------------------------------------
//  Copyright 2007-2008 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#include "acd2d_ev_data.h"
#include "acd2d_edge_visibility.h"
#include "acd2d_ev_function.h"
#include "acd2d_ev_shortest_path.h"

#include <list>
using namespace std;

namespace acd2d
{
	
	///////////////////////////////////////////////////////////////////////////////
	enum TANGENT { LEFT, CUSP, RIGHT };
	typedef list<int>::iterator FIT;  //funnel vertex iterator
	
	inline const Point2d& getPos(ev_vertex * pts, int id)
	{
		return pts[id].v->getPos();
	}
	
	///////////////////////////////////////////////////////////////////////////////
	//l must have at least two elements
	pair<TANGENT,FIT>
	tangent( list<int>& l, int x, ev_vertex * pts)
	{
		bool turnRight=false;
	
		FIT s=l.begin();
		FIT e=l.end(); e--;
		const Point2d& xpos=getPos(pts,x);
	
		for(FIT i=l.begin();i!=e;i++){
			FIT nit=i; nit++;
			bool tr=turn_right(xpos,getPos(pts,*i),getPos(pts,*nit));
			if(i!=s)
				if( tr!=turnRight ) 
					return pair<TANGENT,FIT>(CUSP,i); //CUSP means nothing here..
			turnRight=tr;
		}
		return pair<TANGENT,FIT>(CUSP,l.end()); //nothing tangent to x
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// find if point x tangent to any point in the funnel f
	// if yes, return which side the f x tangents to and the point it tangent to.
	// if no, x must connect to the cusp of f
	// f.left and f.right must not be empt
	pair<TANGENT,FIT>
	tangent( ev_funnel & f, int x, ev_vertex * pts)
	{
		const Point2d& xpos=pts[x].v->getPos();
		if( f.left.size()>1 ){
			//check if x tangents to first of the left
			FIT li=f.left.begin();
			FIT lin=li; lin++;
			bool t1=turn_right(xpos,getPos(pts,*li),getPos(pts,*lin));
			bool t2=turn_right(xpos,getPos(pts,*li),getPos(pts,f.right_pt()) );
			if( t1!=t2 )
				return pair<TANGENT,FIT>(LEFT,li);
			//check if x tangents to the left
			pair<TANGENT,FIT> lt=tangent(f.left,x,pts);
			if( lt.second!=f.left.end() ){ 
				lt.first=LEFT; return lt; 
			}
		}
	
		if( f.right.size()>1 ){
			//check if x tangents to last of the right
			FIT ri=f.right.end(); ri--;
			FIT rip=ri; rip--;
			bool t1=turn_right(xpos,getPos(pts,*ri),getPos(pts,*rip) );
			bool t2=turn_right(xpos,getPos(pts,*ri),getPos(pts,f.left_pt()) );
			if(t1!=t2)
				return pair<TANGENT,FIT>(RIGHT,ri);
			//check if x tangents to the right
			pair<TANGENT,FIT> rt=tangent(f.right,x,pts);
			if( rt.second!=f.right.end() ){ 
				rt.first=RIGHT; return rt; 
			}
		}
	
		//cusp
		return pair<TANGENT,FIT>(CUSP,f.left.end());
	}
	
	inline void build_viz_tree
	( ev_funnel & f, ev_triangle * tri, ev_vertex * pts )
	{
		typedef list<int>::iterator IT;
		int left_pt=f.left_pt();   //left most point of the funnel
		int right_pt=f.right_pt(); //right most point of the funnel
		int x=tri->getOppV(left_pt,right_pt); //the point to be added
		ev_triangle * left_tri=tri->getOppT(left_pt,x);
		ev_triangle * right_tri=tri->getOppT(x,right_pt);
		ev_funnel f1,f2;
	
		//find new cusp
		pair<TANGENT,FIT> new_cusp;
		if( f.getVSize()>4 )
			new_cusp=tangent(f,x,pts);
		else
			new_cusp.first=CUSP;
	
		switch( new_cusp.first ){
		case LEFT:
			pts[*new_cusp.second].addChild(&pts[x],g_id);
			if( left_tri!=NULL ){
				f1.cusp=*new_cusp.second;
				f1.left.insert(f1.left.end(),f.left.begin(),new_cusp.second);
				f1.left.push_back(f1.cusp);
				f1.right.push_back(x);
				f1.right.push_front(f1.cusp);
			}
			if( right_tri!=NULL ){
				f2.cusp=f.cusp;
				f2.left.push_back(x);
				f2.left.insert(f2.left.end(),new_cusp.second, f.left.end());
				f2.right.swap(f.right);
			}
			break;
		case RIGHT:
			pts[*new_cusp.second].addChild(&pts[x],g_id);
			if( left_tri!=NULL ){
				f1.cusp=f.cusp;
				f1.left.swap(f.left);
				FIT it=new_cusp.second; it++;
				f1.right.insert(f1.right.end(), f.right.begin(),it);
				f1.right.push_back(x);
			}
			if( right_tri!=NULL ){
				f2.cusp=*new_cusp.second;
				f2.left.push_back(x);
				f2.left.push_back(f2.cusp);
				f2.right.insert(f2.right.end(), new_cusp.second, f.right.end());
			}
			break;
		case CUSP:
			pts[f.cusp].addChild(&pts[x],g_id);
			if( left_tri!=NULL ){
				f1.left.swap(f.left);
				f1.right.push_back(f.cusp);
				f1.right.push_back(x);
				f1.cusp=f.cusp;
			}
			if( right_tri!=NULL ){
				f2.left.push_back(x);
				f2.left.push_back(f.cusp);
				f2.right.swap(f.right);
				f2.cusp=f.cusp;
			}
			break;
		}
	
		if( left_tri==NULL && right_tri==NULL ) return; //end of the dual tree
		if( left_tri!=NULL ) build_viz_tree(f1,left_tri,pts);
		if( right_tri!=NULL ) build_viz_tree(f2,right_tri,pts);
	}
	
	
	///////////////////////////////////////////////////////////////////////////////
	// build a visibility tree from s to all points in array pts
	// using trianglulation tri
	void build_viz_tree
	(int s, int e, ev_triangle * tri, ev_vertex * pts)
	{
		//init process
		ev_funnel f;
		pts[s].addChild(&pts[e],g_id);
		f.cusp=s;
		f.right.push_back(s);
		f.right.push_back(e);
		f.left.push_back(s);
		build_viz_tree(f,tri,pts);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	//build two visibility trees from s and e to points in array pts
	void build_viz_tree
	( int s, int e, ev_vertex * pts )
	{
		//triangulate the poly
		int vsize=e-s+1;
		//the tri that contains s and e
		ev_triangle * tri=triangulate(&pts[s],vsize);
		if( tri==NULL )
			return;
		//build trees
		g_id=0; //reset gid
		build_viz_tree(0,vsize-1,tri,pts+s);
		g_id=1;
		build_viz_tree(vsize-1,0,tri,pts+s);
	
		//delete triangles
		free_triangle(tri);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// Find the shortest path for all point between s and e to the edge (s,e)
	void shortest_path_to_edge(vertex * s, vertex * e,P2E_CALLBACK p2ec)
	{
		//count vertex size
		int vsize=1;
		vertex * ptr=s;
		do{vsize++; ptr=ptr->getNext();}while(ptr!=e);
	
		//create ev_vertex
		ev_vertex * pts=new ev_vertex[vsize];
		ptr=s;
		for( int i=vsize-1;i>=0;i--,ptr=ptr->getNext()){
			pts[i].v=ptr;
		}
	
		//build two trees from the end pts of edge
		build_viz_tree(0,vsize-1,pts);
	
		//identify visible vertex from edge
		identify_edge_visibility(&pts[0],&pts[vsize-1]);
	
		{//find path for shaded area in region B
			list< pair<int,int> > shade;
			typedef list< pair<int,int> >::iterator SIT;
			find_shaded_region_in_B(shade,pts,vsize);
			for( SIT is=shade.begin();is!=shade.end();is++ ){
				for( int i=is->first;i<=is->second;i++ ){
					pts[i].reset();
				}
				build_viz_tree(is->first, is->second, pts);
				shortest_path(&pts[is->first],&pts[is->second]);
			}//end for SIT
		}//end
	
		{//begin copy distances
			for( int i=0;i<vsize;i++ ){
				pts[i].v->setConcavity(pts[i].dist);
			}
		}//end copy distances
	
		if( p2ec!=NULL ) p2ec(pts);
	
		//delete the pts
		delete [] pts;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// Get all visible vertices from s in the polygon
	void visible_vertices( list<vertex *>& visibleV, vertex * s)
	{
		if( s==NULL ) return;
		int size=0;
		vertex * ptr=s;
		do{size++; ptr=ptr->getNext();}while(ptr!=s);
		//create ev_vertex
		ev_vertex * pts=new ev_vertex[size];
		ptr=s;
		for( int i=0;i<size;i++,ptr=ptr->getNext()) pts[i].v=ptr;
		//triangulate
		ev_triangle * tri = triangulate(pts,size);
	
		g_id=0; //reset gid
		build_viz_tree(0,size-1,tri,pts);
	
		//pust visible vertex into the list
		typedef list<ev_vertex*>::iterator VIT;
		for( VIT iv=pts->children[0].begin();iv!=pts->children[0].end();iv++ )
			visibleV.push_back((*iv)->v);
	
		//delete pts
		free_triangle(tri);
		delete [] pts;
	}

} //namespace acd2d