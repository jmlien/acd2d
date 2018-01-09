//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// This convex hull implemetation realizes the idea from 
// A. Melkman, "On-line construction of the convex hull of a simple polygon", 
// Info. Proc. Letters 25, 11-12 (1987)
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

#include <cassert>
using namespace std;

#include "acd2d_bridge.h"
#include "acd2d_bridge.h"

namespace acd2d
{
	///////////////////////////////////////////////////////////////////////////////
	inline cd_vertex * next(cd_vertex * v)
	{
		if( v->getBridge()==NULL ) 
			return v->getNext();
		else
			return v->getBridge()->v2;
	}
	
	inline bool turn_left
	(const Point2d& p1,const Point2d& p2,const Point2d& p3)
	{
		Vector2d v1=p2-p1;
		Vector2d v2=p3-p2;
		return ((v1[0]*v2[1]-v2[0]*v1[1])>0);
	}
	
	//update the top of the hull
	inline void updateHullTop(cd_vertex * v,list<cd_vertex*>& hull)
	{
		typedef list<cd_vertex*>::iterator VIT;
		if( hull.size()==1 ) return;
		VIT l1=hull.end(); l1--;
		VIT l2=l1; l2--;
		if( turn_left((*l2)->getPos(),(*l1)->getPos(),v->getPos()) )
			return;
		hull.pop_back();
		updateHullTop(v,hull);
	}
	
	//update the bottom of the hull
	inline void updateHullBot(cd_vertex * v,list<cd_vertex*>& hull)
	{
		typedef list<cd_vertex*>::iterator VIT;
		if( hull.size()==1 ) return;
		VIT b1=hull.begin();
		VIT b2=b1; b2++;
		if( turn_left((*b1)->getPos(),(*b2)->getPos(),v->getPos()) )
			return;
		hull.pop_front();
		updateHullBot(v,hull);
	}
	
	//check if v is inside the hull
	inline bool inHull(cd_vertex * v,list<cd_vertex*>& hull )
	{
		typedef list<cd_vertex*>::iterator VIT;
		VIT t1=hull.end(); t1--;
		VIT t2=t1; t2--;
		if( !turn_left((*t2)->getPos(),(*t1)->getPos(),v->getPos()) )
			return false;
	
		VIT b1=hull.begin();
		VIT b2=b1; b2++;
		return turn_left((*b1)->getPos(),(*b2)->getPos(),v->getPos());
	}
	
	//e mush be reachable from s
	void hull2d(cd_vertex * s, cd_vertex * e, list<cd_vertex*>& hull )
	{
		//avoid letting s and e in the bridge
		if( s->getBridge()!=NULL ) s=s->getBridge()->v2;
		if( e->getBridge()!=NULL ) e=e->getBridge()->v1;
		cd_vertex * ne=next(e);
	
		//init the hull
		hull.push_back(s);
		hull.push_back(next(s));
		hull.push_front(hull.back());
	
		//incrementally create the hull
		cd_vertex * ptr=next(hull.back());
		while(ptr!=ne){
			//check if the ptr is contained in the hull
			if( !inHull(ptr,hull) ){
				updateHullTop(ptr,hull);
				updateHullBot(ptr,hull);
				hull.push_back(ptr);
				hull.push_front(ptr);
			}
			ptr=next(ptr);
		}//while(ptr!=ne);
		hull.pop_back();
	}

}//namespace acd2d

