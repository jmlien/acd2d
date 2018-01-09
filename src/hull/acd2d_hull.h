//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2D_HULL_H_
#define _CD2D_HULL_H_

///////////////////////////////////////////////////////////////////////////////
// This convex hull implemetation realizes the idea from 
// A. Melkman, "On-line construction of the convex hull of a simple polygon", 
// Info. Proc. Letters 25, 11-12 (1987)
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

#include <cassert>
using namespace std;

#include "acd2d_data.h"

namespace acd2d
{	
	//e mush be reachable from s
	void hull2d(cd_vertex * s, cd_vertex * e, list<cd_vertex*>& hull );
	
}//namespace acd2d

#endif //_CD2D_HULL_H_

