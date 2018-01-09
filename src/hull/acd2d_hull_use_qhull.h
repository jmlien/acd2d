//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------


#ifndef _CD_HULL_H_
#define _CD_HULL_H_

///////////////////////////////////////////////////////////////////////////////
extern "C"{
#include <stdio.h>
#include "qhull.h"
#include "poly.h"
#include "qset.h"
}

///////////////////////////////////////////////////////////////////////////////

#include <cassert>
using namespace std;

#include <Point.h>
#include <list>
using namespace std;

namespace acd2d
{
	
	///////////////////////////////////////////////////////////////////////////////
	class hull_2d{
	public:
		hull_2d(){ ptsize=0; pt=NULL; }
		~hull_2d(){ free(); }
		
		void reset(){ 
			ptsize=0; //fsize=0; 
		}
		
		bool alloc(int size){
			size=size*2; //for 3 coord
			pt=new coordT[size];
			return (pt!=NULL);
		}
		
		void free(){
			ptsize=0;
			delete [] pt; pt=NULL; 
		}
		
		void addpt(const Point2d& pt){ 
			for( int i=0;i<2;i++ ) this->pt[ptsize*2+i]=pt[i]; 
			ptsize++;
		}
		
		void buildhull(){
			//global varible for qt
			int curlong, totlong;
			
			//using qhull
			static char * options="qhull Qs Qx QJ i Tcv C-0 Pp";
			qh_init_A(stdin, stdout, stderr, 0, NULL);
			qh_initflags(options);
			qh_init_B (pt, ptsize, 2, false);
			qh_qhull();
			qh_check_output();
			
			//find bridge
			findBridges();
			
			//free mem
			qh_freeqhull(!qh_ALL);
			qh_memfreeshort (&curlong, &totlong);
		}
		
		/**
		 * Get Bridge
		 */
		const list< pair<int,int> > & GetBridge() const { return bridge; }
		   
		///////////////////////////////////////////////////////////////////////////
		//Data
	private:
		
		///////////////////////////////////////////////////////////////////////////
		void findBridges()
		{
			vertexT *vertex;
			bridge.clear();
			list<int> ids;
			FORALLvertices {
				ids.push_back(qh_pointid(vertex->point));
			}
			ids.sort();
			int pre_id=-1;
			for(list<int>::iterator i=ids.begin();i!=ids.end();i++ ){
				if( i==ids.begin() ){
					pre_id=*i;
					continue;
				}
				if( (pre_id+1)!=*i ){
					pair<int,int> b(pre_id,*i);
					bridge.push_back(b);
				}
				pre_id=*i;
			}
			
			if( ids.back()!=ptsize-1 || ids.front()!=0 ){
				pair<int,int> b(ids.back(),ids.front());
				bridge.push_back(b);
			}
		}
	
		///////////////////////////////////////////////////////////////////////////
		//Input
		int ptsize;  //number of point
		coordT * pt; //points in the patch (*3 for each coord)
		
		//Output
		list< pair<int,int> > bridge;
	};

}//namespace acd2d

#endif //_CD_HULL_H_

