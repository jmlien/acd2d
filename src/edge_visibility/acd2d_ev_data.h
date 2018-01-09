//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef EV_DATA_H_
#define EV_DATA_H_

#include <stdlib.h>
#include <list>
using namespace std;

#include "acd2d_data.h"


namespace acd2d
{
	
	typedef cd_vertex vertex;
	typedef cd_poly poly;
	
	/**
	 * ev_triangle for visibility computation
	 */
	class ev_triangle{
	
	public:
	
		ev_triangle(){ 
			for( int i=0;i<3;i++ ){
				v[i]=-1; 
				t[i]=NULL;
			}
		}
	
		int getOppV(int v1, int v2){
			for( int i=0;i<3;i++ ){
				if( v[i]!=v1 && v[i]!=v2 ) return v[i];
			}
			cerr<<"!ERROR: getOppV"<<endl;
			exit(1);
		}
	
		ev_triangle * getOppT(int v1, int v2){
			for( int i=0;i<3;i++ ){
				if( t[i]==NULL ) return NULL;
				if( t[i]->isIn(v1) && t[i]->isIn(v2) ) return t[i];
			}
			cerr<<"!ERROR: getOppT"<<endl;
			exit(1);
		}
	
		bool isIn( int vt ){
			for( int i=0;i<3;i++ ) if( v[i]==vt ) return true;
			return false;
		}
	
		int v[3];
		ev_triangle * t[3];
	};
	
	
	/**
	 * the visibility nodes
	 */
	class ev_vertex{
	public:
		ev_vertex(vertex * v=NULL){ 
			this->v=v; 
			cusp[0]=cusp[1]=NULL; 
			is_convex[0]=is_convex[1]=false;
			parent=NULL;
			dist=1e15;
		}
	
		void addChild(ev_vertex * c, int id){
			c->cusp[id]=this;
			children[id].push_back(c);
		}
	
		void reset(){
			children[0].clear();
			children[1].clear();
			cusp[0]=cusp[1]=NULL;
		}
	
		vertex * v;
		bool is_convex[2];
		list<ev_vertex *> children[2];
		ev_vertex * cusp[2]; //the cusp when this is created
		ev_vertex * parent;  //final parent in the shortest path
		double dist;
	};
	
	/**
	 * Funnel
	 */
	class ev_funnel{
	public:
	
		ev_funnel(){ cusp=-1; }
		int getVSize(){ return left.size()+right.size()+1; }
		int left_pt(){ return left.front(); } //(left.empty())?cusp:left.front(); }
		int right_pt(){ return right.back(); } //(right.empty())?cusp:right.back(); }
	
		list<int> left;
		list<int> right;
		int cusp; //current cusp
	};

}//end namespace

#endif //EV_DATA_H_

