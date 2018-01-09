//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2D_CONVEXITY_H_

namespace acd2d
{
	
	///////////////////////////////////////////////////////////////////////////
	// measure convexity
	
	inline double A_hull(const cd_polygon& polygon)
	{
		typedef cd_polygon::const_iterator PIT;
		list<cd_vertex*> hull;
	
		for(PIT i=polygon.begin();i!=polygon.end();i++){
			if( i->getType()==cd_poly::PIN ) continue;
			//build hull
			hull2d(i->getHead(),i->getHead()->getPre(),hull);
			//compute hull area
			return A_poly(hull);
		}//end for
	
		cerr<<"! ERROR:hullArea"<<endl;
		exit(1);
	}
	
	inline double convexity(const cd_2d& cd)
	{
		double hullArea=0;
		double polyArea=0;
		
		const list<cd_polygon>& todo=cd.getTodoList();
		const list<cd_polygon>& done=cd.getDoneList();
		typedef list<cd_polygon>::const_iterator PIT;
	
		{//todo list
			for(PIT ip=todo.begin();ip!=todo.end();ip++){
				polyArea+=A_polygon(*ip);
				hullArea+=A_hull(*ip);
			}//end for
		}
	
		{//done list
			for(PIT ip=done.begin();ip!=done.end();ip++){
				polyArea+=A_polygon(*ip);
				hullArea+=A_hull(*ip);
			}//end for
		}
	
		return polyArea/hullArea;
	}
	
	inline double convexity(const cd_polygon& polygon){
		return A_polygon(polygon)/A_hull(polygon);
	}

} //namespace acd2d


#endif //
