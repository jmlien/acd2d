//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2D_CUT_DIR_H_
#define _CD2D_CUT_DIR_H_

#include "acd2d_data.h"
#include "acd2d_edge_visibility.h"
 
namespace acd2d
{
	
	///////////////////////////////////////////////////////////////////////////////
	// For genereal
	inline
	void setupCutLine(cd_vertex * r,cd_vertex * r2,cd_line& line)
	{
		Vector2d dir;
		if( r2==NULL )
			dir=-(r->getNormal()+r->getPre()->getNormal());
		else 
			dir=r2->getPos()-r->getPos();
		line.vec=dir.normalize();
		line.support=r;
		line.origin=r->getPos();
		line.normal.set(-line.vec[1],line.vec[0]);
	}
	
	inline bool isInBoundaries(cd_vertex * v, cd_vertex * v_, bool exclusive = false)
	{
		Vector2d v1 = v->getNext()->getPos() - v->getPos();
		Vector2d v2 = v->getPre()->getPos() - v->getPos();

		Vector2d vec=v_->getPos()-v->getPos();
		double svecv1=(vec[0]*v1[1]-vec[1]*v1[0]);
		double svecv2=(vec[0]*v2[1]-vec[1]*v2[0]);

		bool v1v2Convex = v1[0]*v2[1]-v1[1]*v2[0] > 0;
		// if (v->getPos()[0] == 10 && abs(v->getPos()[1] + 20.8) < 0.1 || v_->getPos()[0] == 10 && abs(v_->getPos()[1] + 20.8) < 0.1) {
		// 	printf("v %lf %lf; v_ %lf %lf\n", v->getPos()[0], v->getPos()[1], v_->getPos()[0], v_->getPos()[1]);
		// 	printf("v1: %lf %lf, v2: %lf %lf, vec: %lf %lf, convex: %d, svecv1: %lf, svecv2: %lf result %d\n",
		// 			v1[0], v1[1], v2[0], v2[1], vec[0], vec[1], (int)v1v2Convex, svecv1, svecv2, 
		// 			(int)(!exclusive ? v1v2Convex ? svecv1 <= 0 && svecv2 >= 0 : !(svecv1 > 0 && svecv2 < 0)
		// 							 : v1v2Convex ? svecv1 < 0 && svecv2 > 0 : !(svecv1 >= 0 && svecv2 <= 0)));
		// 	printf("isInBoundaries\n");
		// }
		
		//TODO: possible problems with colinear lines
		//      see how BarbedThing.poly behaves when using exclusive intervals
		//      ie. svecv1 < 0 && svecv2 > 0 : !(svecv1 >= 0 && svecv2 <= 0)
		return !exclusive ? v1v2Convex ? svecv1 <= 0 && svecv2 >= 0 : !(svecv1 > 0 && svecv2 < 0)
						  : v1v2Convex ? svecv1 < 0 && svecv2 > 0 : !(svecv1 >= 0 && svecv2 <= 0);
	}

	//check if vv_ can resolve v
	inline bool isResolved(cd_vertex * v, cd_vertex * v_)
	{
		static Vector2d v1;
		static Vector2d v2;
		static cd_vertex * oldV=NULL;
		static double c=0;
	
		if( oldV!=v ){
			oldV=v;
			v1=v->getPos()-v->getPre()->getPos();
			v2=v->getPos()-v->getNext()->getPos();
			c=(v1[1]*v2[0]-v1[0]*v2[1]);
		}
	
		if (c > 0)
			return isInBoundaries(v, v_, true);
	
		//check if tmp can resolve v
		Vector2d vec=v_->getPos()-v->getPos();
		double a=(vec[0]*v2[1]-vec[1]*v2[0]);
		double b=(vec[0]*v1[1]-vec[1]*v1[0]);
		return ((a*c)<0) && ((b*c)>0);

	}
	
	///////////////////////////////////////////////////////////////////////////////
	// For out most
	
	inline cd_vertex * 
	cut_dir_heuristic( cd_vertex * v, double alpha, double beta )
	{
		Vector2d nv= (v->getNormal()+v->getPre()->getNormal()).normalize();
	
		double max_score=0;
		cd_vertex * bestV=NULL;
		cd_vertex * ptr=v->getNext();
		
		do{
			Vector2d vec=ptr->getPos()-v->getPos();
			double score= (1+alpha*ptr->getConcavity())/(beta*vec.norm());
	
			if( score>max_score ){
				//check if it is facing the diff side
				Vector2d nvis= (ptr->getNormal()+ptr->getPre()->getNormal()).normalize();
				//see if this can resolve the notch
				if( isResolved(v,ptr) ){
					max_score=score;
					bestV=ptr;
				}
			}//end if score
			ptr=ptr->getNext();
		}while(ptr!=v);
	
		return bestV;
	}//end
	
	inline void find_a_good_cutline
	(cd_line& line, cd_vertex * r, double alpha, double beta)
	{
		cd_vertex * r2=cut_dir_heuristic(r,alpha,beta);
		setupCutLine(r,r2,line);
	}
	
	///////////////////////////////////////////////////////////////////////////////
	// For hole
	
	inline
	Point2d find_MP(cd_vertex * v, cd_poly& poly)
	{
		cd_vertex * head, *cur;
		head=cur=poly.getHead();
		Point2d best;
		double min_dist=1e10;
		
		do{
			Point2d pt=cur->computeClosePt(v->getPos());
			double dist=(pt-v->getPos()).normsqr();
			if( dist<min_dist ){
				cd_vertex tmp=*cur;
				tmp.setPos(pt);
				if( isResolved(v,&tmp) ){   
					min_dist=dist;
					best=pt;
				}
			}
			
			cur=cur->getNext();
		}
		while(cur!=head);
		
		if( fabs(min_dist-1e10)<1e-10 ){
			Vector2d dir=-(v->getNormal()+v->getPre()->getNormal());
			best=v->getPos()+dir;
		}
		
		return best;
	}
	
	inline void find_a_good_cutline_for_hole
	(cd_line& line, cd_vertex * v, cd_poly& poly)
	{
		Point2d pt=find_MP(v,poly);
		cd_vertex tmp(pt);
		setupCutLine(v,&tmp,line);
	}

} //namespace acd2d

#endif //_CD2D_CUT_DIR_H_

