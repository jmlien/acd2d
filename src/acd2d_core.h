//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2D_H_
#define _CD2D_H_

#include <list>
using namespace std;

#include "acd2d_data.h"

namespace acd2d
{

	typedef cd_polygon::iterator PLYIT;
	typedef cd_polygon::const_iterator PLYCIT;
	
	class IConcavityMeasure; //the interface for concavity measurement
	
	class cd_2d
	{
	public:
	
		///////////////////////////////////////////////////////////////////////////
		// constructors
	
		//if store_cut_line is ture, cutlines will be stored.
		cd_2d(bool save_diagonal=false); 
		~cd_2d();
		
		///////////////////////////////////////////////////////////////////////////
		//polygon functions
		void addPolygon(const cd_polygon& poly);
		void destroy(); //remove all polygons from acd	
		
		///////////////////////////////////////////////////////////////////////////
		// Do decomposition once/All
		void decomposeAll(double d, IConcavityMeasure * measure);
		void decompose(double d, IConcavityMeasure * measure);
	
		///////////////////////////////////////////////////////////////////////////
		//access functions
		const list<cd_polygon>& getTodoList() const { return todo_list; }
		const list<cd_polygon>& getDoneList() const { return done_list; }
		const list<cd_diagonal>& getDiagonal() const { return dia_list; }
		void updateCutDirParameters( double a, double b ){ alpha=a; beta=b;  }
	
		///////////////////////////////////////////////////////////////////////////
		//other functions
	
		
	protected:
	
		void decompose(double d, cd_polygon& polys );
		void decompose_OUT(double d, cd_polygon& polys, cd_poly& poly);
		void decompose_IN(double d, cd_polygon& polys, cd_poly& poly);
	
	private:
	
		list<cd_polygon> todo_list;
		list<cd_polygon> done_list;
	
		IConcavityMeasure * m_measure;
	
		double alpha, beta; // for selecting cutting direction
	
		//cut lines
		list<cd_diagonal> dia_list;
		bool store_diagoanls;    //if set, all cut lines will be stored
	};

}

#endif //_CD2D_H_

