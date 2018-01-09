//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#include "acd2d_concavity.h"
#include "acd2d_util.h"
#include "acd2d_edge_visibility.h"	
#include <iostream>
using namespace std;

namespace acd2d
{

	///////////////////////////////////////////////////////////////////////////////
	cd_vertex * StraightLineMeasurement::
	findMaxNotch(cd_vertex * v1, cd_vertex * v2)
	{
		cd_vertex * r=NULL;
		cd_vertex * ptr=v1->getNext();
		Vector2d v=(v2->getPos()-v1->getPos());
		Vector2d n(-v[1],v[0]);
		double norm=n.norm();
		if( norm!=0 ) n=n/norm;
	
		do{
			double c;
			if( norm!=0) c=findDist(n,v1->getPos(),ptr->getPos());
			else c=(v1->getPos()-ptr->getPos()).norm();
			ptr->setConcavity(c);
			if( r==NULL ) r=ptr;
			else if( r->getConcavity()<c ){ r=ptr; }
			ptr=ptr->getNext();
		}while( ptr!=v2 );
		v1->setConcavity(0);
		v2->setConcavity(0);
		return r;
	}
	
	double StraightLineMeasurement::
	findDist(const Vector2d& n, const Point2d& p, const Point2d& qp)
	{
		return (qp-p)*n;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	cd_vertex * ShortestPathMeasurement::
	findMaxNotch(cd_vertex * v1, cd_vertex * v2)
	{
		shortest_path_to_edge(v1,v2);
		cd_vertex * ptr=v1->getNext();
		cd_vertex * max_v=NULL;
		double max_c=-1e20;
		do{
			if( max_c < ptr->getConcavity() ){
				max_c=ptr->getConcavity();
				max_v=ptr;
			}
			ptr=ptr->getNext();
		}while( ptr!=v2 );
		return max_v;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	cd_vertex *
	HybridMeasurement1::
	findMaxNotch(cd_vertex * v1, cd_vertex * v2)
	{
		StraightLineMeasurement sl;
		if( !NeedSP(v1,v2) ) 
			return sl.findMaxNotch(v1,v2);
		ShortestPathMeasurement sp;
		return sp.findMaxNotch(v1,v2);
	}
	
	//check if 
	bool HybridMeasurement1::NeedSP(cd_vertex * v1, cd_vertex * v2)
	{
		Vector2d bn=computeNormal(v2->getPos()-v1->getPos());
		cd_vertex * v=v1;
		do{
			if( v->getNormal()*bn<0 ) return true;
			v=v->getNext();
		}while(v!=v2);
		return false;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	
	cd_vertex *
	HybridMeasurement2::
	findMaxNotch(cd_vertex * v1, cd_vertex * v2)
	{
		if( tau==-1 ){
			cerr<<"HybridMeasurement 2 Error : Tau is not set"<<endl;
			return NULL;
		}
	
		StraightLineMeasurement sl;
		cd_vertex * r=sl.findMaxNotch(v1,v2);
		if( r->getConcavity()>tau ) return r;
		if( !NeedSP(v1,v2) ) return r;
		ShortestPathMeasurement sp;
		return sp.findMaxNotch(v1,v2);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	//
	// The measurement factory 
	//
	///////////////////////////////////////////////////////////////////////////////
	
	IConcavityMeasure * ConcavityMeasureFac::createMeasure( const string& name )
	{
		if( name=="straightline" || name=="sl" || name=="SL" )
			return new StraightLineMeasurement();
		else if( name=="shortestpath" || name=="sp" || name=="SP" ) 
			return new ShortestPathMeasurement();
		else if( name=="hybrid1" )
			return new HybridMeasurement1();
		else if( name=="hybrid2" )
			return new HybridMeasurement2();
		else{
			cerr<<"unknow concavity measurement name : "<<name<<endl;
			return NULL;
		}
	}

} //namespace acd2d
