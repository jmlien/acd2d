//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2D_CUT_H_
#define _CD2D_CUT_H_

#include "acd2d_data.h"
#include "acd2d_bridge.h"

///////////////////////////////////////////////////////////////////////////////
//
//  Following procedures will cut the polygon into two pieces
//  for a given cut line and reulting polygons will be put into.
//  result list.
//  
//  The main function is : cutPolys
//
///////////////////////////////////////////////////////////////////////////////

namespace acd2d
{
	
	inline void checkDegeneracy(cd_vertex* v1)
	{
		cd_vertex * v=v1;
	   
		if( v->getPos().almost_equ(v->getNext()->getPos()) ){ //dup
			cd_vertex * dup=v->getNext();
			v->setNext(dup->getNext());
			delete dup;
		}
		else v=v->getNext();
		
		if( v->getPos().almost_equ(v->getNext()->getPos()) ){ //dup
			cd_vertex * dup=v->getNext();
			v->setNext(dup->getNext());
			delete dup;
		}
		else v=v->getNext();
		
		if( v->getPos().almost_equ(v->getNext()->getPos()) ){ //dup
			v->getPre()->setNext(v->getNext());
			delete v;
		}
	}
	
	/**
	 * remove duplicate nodes.
	 */
	inline void checkDegeneracy(cd_vertex* v1, cd_vertex * v2)
	{
		checkDegeneracy(v1);
		checkDegeneracy(v2);
	}
	
	/**
	 * update the information, such as normal and check
	 * reflectivity.
	 */
	inline void updateInfo(cd_vertex* v)
	{
		for( int i=0;i<4;i++ ){
			v->computeNormal();
			v->computeReflex();
			if( !v->isReflex() )
				v->setConcavity(0);
			v=v->getNext();
		}
	}
	
	/**
	 * split edges in the cut.
	 */
	inline void addDiagnal( cd_vertex* v1, cd_vertex * v2 )
	{
		cd_vertex * v1n=v1->getNext();
		cd_vertex * v2n=v2->getNext();
		
		cd_vertex * n11=new cd_vertex(v1->getInterPt());
		cd_vertex * n12=new cd_vertex(v1->getInterPt());
		cd_vertex * n21=new cd_vertex(v2->getInterPt());
		cd_vertex * n22=new cd_vertex(v2->getInterPt());
		
		v1->setNext(n11);
		n11->setNext(n22);
		n22->setNext(v2n);
		
		v2->setNext(n21);
		n21->setNext(n12);
		n12->setNext(v1n);
		
		checkDegeneracy(v1,v2);
		updateInfo(v1);
		updateInfo(v2);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	//
	//  For the out most boundary
	//
	///////////////////////////////////////////////////////////////////////////////
	
	
	/**
	 * Find the edges that will split the out most chain
	 */
	inline pair<cd_vertex*,cd_vertex*> 
	FindCut_Out( cd_poly& poly, cd_line& cut_l )
	{
		list<cd_vertex*> coll;
		typedef list<cd_vertex*>::iterator VIT;
		poly.findCollEdges(coll, cut_l);
		
		double min_U=FLT_MAX;
		cd_vertex* closest=NULL;
		for( VIT iv=coll.begin();iv!=coll.end();iv++){
			cd_vertex* cur=*iv;
			if( cur==cut_l.support ) continue;
			if( cur==cut_l.support->getPre() ) continue;
			if( cur->getU()<-1e-5 ) continue;
			if( cur->getU()<min_U ){
				min_U=cur->getU();
				closest=cur;
			}
		}
	
		if(closest!=NULL) 
			return pair<cd_vertex*,cd_vertex*>(cut_l.support,closest);
	
		{//ERROR
			cerr<<"! ERROR: FindCut_Out Error\n";
			for( VIT iv=coll.begin();iv!=coll.end();iv++){
				cd_vertex* cur=*iv;
				cout<<"! ERROR Info: U="<<cur->getU()<<"\n";
			}
			cerr<<"! ERROR Info: Cut line O=("
				<<cut_l.origin[0]<<","<<cut_l.origin[1]<<")\n"
				<<"! ERROR Info: Cut line V=("
				<<cut_l.vec[0]<<","<<cut_l.vec[1]<<")\n";
			cerr<<"! ERROR Info: Polygon=\n"
				<<poly<<endl;
			exit(1);
		}
	}
	
	/**
	 * cut polys using cut_l into two polygons.
	 */
	inline cd_diagonal cutPolys
	(pair<cd_polygon,cd_polygon>& result, cd_poly& poly, cd_line& cut_l)
	{
		//find cuts
		pair<cd_vertex*, cd_vertex*> cut = FindCut_Out(poly,cut_l);
		cd_vertex * v1=cut.first;
		cd_vertex * v2=cut.second;
	
		cd_vertex * nv1=v1->getNext();
		cd_vertex * nv2=v2->getNext();
		UpdateBridge(v1,nv1,v2,nv2);
	
		addDiagnal(v1,v2);
		
		//store polygonal chains
		cd_poly p1(cd_poly::POUT),p2(cd_poly::POUT);
		p1.set(cd_poly::POUT,v1); p2.set(cd_poly::POUT,v2);
		p1.updateSize(); p2.updateSize();
	
		result.first.push_back(p1);
		result.second.push_back(p2); 
	
		return cd_diagonal(v1->getInterPt(),v2->getInterPt());
	}
	
	///////////////////////////////////////////////////////////////////////////////
	//
	//  For the hole boundary
	//
	///////////////////////////////////////////////////////////////////////////////
	
	inline pair<cd_vertex*,cd_vertex*> 
	FindCut_In( cd_poly& out, cd_poly& in, cd_line& cut_l )
	{
		list<cd_vertex*> coll;
		out.findCollEdges(coll, cut_l);
		
		double min_u=FLT_MAX;
		cd_vertex * min_v=NULL;
        for(auto v : coll )
        {
			if( v->getU()<0 ) continue; //not in the right dir
			if( v->getU()<min_u ){ 
				min_u=v->getU();
				min_v=v;
			}//end if
		}//end for
	
		if( min_v!=NULL )
			return pair<cd_vertex*,cd_vertex*> (cut_l.support,min_v);

		cerr<<"! ERROR: FindCut_In Error"<<endl;
		exit(1);
	}
	
	inline cd_diagonal
	mergeHole(cd_poly& out, cd_poly& hole, cd_line& cut_l)
	{
		//find cuts
		cd_vertex * v1, *v2;
		pair<cd_vertex*, cd_vertex*> cut = FindCut_In(out,hole,cut_l);
		v1=cut.first;
		v2=cut.second;
		v1->Intersect(cut_l);
		addDiagnal(v1,v2);
		out.updateSize();
		return cd_diagonal(cut.first->getInterPt(),cut.second->getInterPt());
	}
}//namespace acd2d

#endif //_CD2D_CUT_H_
