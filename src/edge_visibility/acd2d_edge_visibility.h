//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _EV_H_
#define _EV_H_

namespace acd2d
{
	class ev_vertex;
	class cd_vertex;
	typedef cd_vertex vertex;
	typedef void (*P2E_CALLBACK)(ev_vertex *);
	
	void shortest_path_to_edge(vertex * s, vertex * e,P2E_CALLBACK p2ec=NULL);
	
	void visible_vertices( list<vertex *>& visibleV, vertex * s);
	
	struct triangle { Point2d pt[3]; };
}

#endif //_EV_H_

