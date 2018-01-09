//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2d_BRIDGE_H_
#define _CD2d_BRIDGE_H_

#include "acd2d_data.h"
#include "acd2d_util.h"
#include "acd2d_hull.h"

namespace acd2d
{
	
	///////////////////////////////////////////////////////////////////////////////
	// Bridge
	class cd_bridge
	{
	public:
		
		cd_bridge(){ v1=NULL; v2=NULL; max_r=NULL; }
		void FindMaxR(IConcavityMeasure * measure);
		
		//data
		cd_vertex * v1, * v2; //note that the bridge of v2 is not this bridge
		cd_vertex * max_r;
	};
	
	///////////////////////////////////////////////////////////////////////////////
	//
	//  Function for manipulating bridge
	//
	///////////////////////////////////////////////////////////////////////////////
	
	//set s->e 's bridge to b.
	inline void setBridge( cd_bridge * b, cd_vertex * s, cd_vertex * e )
	{
		while(s!=e){ s->setBridge(b); s=s->getNext(); }
	}
	
	inline void construct_bridges( cd_vertex * s, cd_vertex * e)
	{
		///////////////////////////////////////////////////////////////////////////
		list<cd_vertex*> hull;
		hull2d(s,e,hull);
		
		///////////////////////////////////////////////////////////////////////////
		//find birdges
	
		typedef list<cd_vertex*>::iterator VIT;
	
		for( VIT it=hull.begin();it!=hull.end();it++ ){
			cd_vertex* v=*it;
			if( v->getBridge()!=NULL ) continue;
			//get start and end of the associate pocket
			VIT nit=it; nit++;
			if( nit==hull.end() ) nit=hull.begin();
			if( v->getNext()==(*nit) ) continue;
			cd_bridge * b=new cd_bridge();
			b->v1=v; b->v2=*nit;
			setBridge(b,b->v1,b->v2);
		}
	}
	
	inline void removeBridge(cd_vertex* v)
	{
		if( v->getBridge()!=NULL ){
			cd_bridge * b=v->getBridge();
			setBridge(NULL,b->v1,b->v2);
			delete b;
		}
	}
	
	inline void UpdateBridge(cd_vertex* v1, cd_vertex* nv1,cd_vertex* v2,cd_vertex* nv2)
	{
		removeBridge(v1);
		removeBridge(v2);
		removeBridge(nv1);
		removeBridge(nv2);
	}
	
}//namespace acd2d

#endif //_CD2d_BRIDGE_H_

