//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _EV_FUNCTION_H_
#define _EV_FUNCTION_H_

#include "acd2d_ev_data.h"

namespace acd2d
{
	//return the triangle contains s and e
	ev_triangle * triangulate( ev_vertex * pts, int polysize );
	
	//delete all triangles
	void free_triangle( ev_triangle * t );
}

#endif //_EV_FUNCTION_H_

