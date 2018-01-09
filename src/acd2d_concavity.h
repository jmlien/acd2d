//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------
	
#ifndef _CD2D_CONCAVITY_H_
#define _CD2D_CONCAVITY_H_

#include "acd2d_data.h"
#include <string>
using namespace std;

namespace acd2d
{
	
	///////////////////////////////////////////////////////////////////////////////
	//
	// The interface of concavity measurements
	//
	///////////////////////////////////////////////////////////////////////////////
	 
	class IConcavityMeasure {
	public :
		virtual ~IConcavityMeasure(){}
		virtual cd_vertex * findMaxNotch(cd_vertex * v1, cd_vertex * v2)=0;
	};
	
	///////////////////////////////////////////////////////////////////////////////
	class StraightLineMeasurement : public IConcavityMeasure
	{
	public :
	
		cd_vertex * findMaxNotch(cd_vertex * v1, cd_vertex * v2);
	
	private:
	
		// compute distance from a point, qp, to a segment p1->p2
		double findDist(const Vector2d& n, const Point2d& p, const Point2d& qp);
	};
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	class ShortestPathMeasurement : public IConcavityMeasure
	{
	public:
		cd_vertex * findMaxNotch(cd_vertex * v1, cd_vertex * v2);
	};
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	// check if SP is needed before using SL
	class HybridMeasurement1 : public IConcavityMeasure
	{
	public:
		HybridMeasurement1(){}
		cd_vertex * findMaxNotch(cd_vertex * v1, cd_vertex * v2);
	protected:
		bool NeedSP(cd_vertex * v1, cd_vertex * v2);
	};
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	// use SL if the max concavity is larger than tau
	// if smaller than tau, if NeedSP is checked to see of SP is needed 
	class HybridMeasurement2 : public HybridMeasurement1
	{
	public:
		HybridMeasurement2(){ tau=-1; }
		cd_vertex * findMaxNotch(cd_vertex * v1, cd_vertex * v2);
		void setTau( double t ) { tau=t; }
	private:
		double tau;
	};
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////
	//
	// The measurement factory 
	//
	///////////////////////////////////////////////////////////////////////////////
	
	class ConcavityMeasureFac {
	public:
		static IConcavityMeasure * createMeasure( const string& name );
	};

} //namespace acd2d

#endif //_CD2D_CONCAVITY_H_

