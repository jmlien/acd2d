//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#include "acd2d_ev_function.h"
#include "data.h"

#include <vector>
using namespace std;
///////////////////////////////////////////////////////////////////////////
#ifdef WIN32
extern "C"{
#include "triangulate.h"
}
#else 
#include "triangulate.h"
#endif

namespace acd2d
{
	
	typedef pair<int,ev_triangle *> Edge; //a pair of pt id and tri id
	typedef vector<Edge> EV;   //vector of Edge
	
	inline void add(EV& ev, int pid, ev_triangle * t)
	{
		Edge dte(pid,t);
		ev.push_back(dte);
	}
	
	inline ev_triangle * find(EV& ev, int pid) //return tri
	{
		typedef EV::iterator it;
		for(it i=ev.begin();i!=ev.end();i++)
			if( i->first==pid ) return i->second;
			return NULL;
	}
	
	//returns the triangle contains (0,N-1) and build the dual of given triangles
	inline ev_triangle * dualT(int * t, int tsize, int psize)
	{
		int last=psize-1;
	
		ev_triangle * start_T=NULL; //the triangle with edge (0,psize-1)
		ev_triangle * first_T=NULL;
	
		vector<EV> hash(psize,EV());
		
		for( int it=0;it<tsize;it++ ){ //for each triangle
			ev_triangle * cur_tri=new ev_triangle();
			if( it==0 ) first_T=cur_tri;
			for(int ie=0;ie<3;ie++ ){ //for each edge in this triangle
				int vid1=t[it*3+ie];       //edge start
				int vid2=t[it*3+(ie+1)%3]; //edge end
				cur_tri->v[ie]=vid1;
				if( (vid1==0&&vid2==last) || (vid1==last&&vid2==0) ){ 
					start_T=cur_tri; 
				}
	
				ev_triangle * nei_tri=find(hash[vid1],vid2); //neighboring tri
				if( nei_tri==NULL ){ //not found
					add(hash[vid2],vid1,cur_tri); continue; 
				}
				//found
				{for(int i=0;i<3;i++) if(cur_tri->t[i]==NULL){cur_tri->t[i]=nei_tri; break;}}
				{for(int i=0;i<3;i++) if(nei_tri->t[i]==NULL){nei_tri->t[i]=cur_tri; break;}}
			}
		}//end it
		
		if(start_T==NULL) free_triangle(first_T);
		return start_T;
	}
	
	//return the triangle contains s and e
	ev_triangle * triangulate( ev_vertex * pts, int polysize )
	{
		//repare for triangulation
		double * v=new double[polysize*2];
		int trisize=polysize-2;
		int * t=new int[3*trisize];   //to hold resulting triangles
		
		//copy vertices
		for(int i=0;i<polysize;i++){
			const Point2d& pos=pts[i].v->getPos();
			v[2*i]=pos[0]; v[2*i+1]=pos[1];
		}//end for
		
		FIST_PolygonalArray(1, &polysize, (double(*)[2])v, &trisize, (int(*)[3])t);
	
		ev_triangle * tri=dualT(t,trisize,polysize);
	
		delete [] v;
		delete [] t;
		return tri;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// deallocate all triangles
	void free_triangle( ev_triangle * t, ev_triangle * p )
	{
		for( int i=0;i<3;i++ ){
			if( t->t[i]==NULL || t->t[i]==p ) continue;
			free_triangle(t->t[i],t);
		}
		delete t;
	}
	
	void free_triangle( ev_triangle * t )
	{
		free_triangle(t,NULL);
	}
	
} //namespace acd2d
