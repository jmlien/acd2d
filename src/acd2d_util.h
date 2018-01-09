//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2d_UTIL_H_
#define _CD2d_UTIL_H_

#include "acd2d_data.h"
#include "acd2d.h"
#include <fstream>
#include <float.h>

namespace acd2d
{
	
	///////////////////////////////////////////////////////////////////////////
	// count number of vertices from s to e
	inline int count_v( cd_vertex * s, cd_vertex * e ){
		int c=0;
		while( s!=e ){ c++; s=s->getNext(); }
		return c+1;
	}
	
	///////////////////////////////////////////////////////////////////////////
	// Read polygons from *.poly file
	
	/*
	inline void read_poly(istream& fin, cd_2d& cd)
	{
		//throw away lines that are # and empty
		int poly_number=-1; 
		do{
			string comments;
			getline(fin,comments);
			if( !comments.empty() && comments[0]!='#' )
				poly_number=atoi(comments.c_str());
		}while(poly_number<0);
	
		//file opened, read the file
		cd.beginPolys();
		for( int ip=0;ip<poly_number;ip++ ){
			cd_poly poly(cd_poly::UNKNOWN);
			fin>>poly;
			cd.addPolys(poly);
		}//edn for
		cd.endPolys();
	}
	
	inline void read_polys(const string& name, cd_2d& cd)
	{
		ifstream fin(name.c_str());
		if( fin.good()==false ){
			cerr<<"! ERROR: can NOT open file : "<<name<<endl;
			return;
		}
		read_poly(fin,cd);
		//close the file
		fin.close();
	}
	
	inline void read_poly(istream& fin, cd_polygon& polygon)
	{
		//throw away lines that are # and empty
		int poly_number=-1; 
		do{
			string comments;
			getline(fin,comments);
			if( !comments.empty() && comments[0]!='#' )
				poly_number=atoi(comments.c_str());
		}while(poly_number<0);
	
		//file opened, read the file
		for( int ip=0;ip<poly_number;ip++ ){
			cd_poly poly(cd_poly::UNKNOWN);
			fin>>poly;
			polygon.push_back(poly);
		}//edn for
	}
	*/
	
	inline void read_poly(const string& name, cd_polygon& polygon)
	{
		ifstream fin(name.c_str());
		if( fin.good()==false ){
			cerr<<"! ERROR: can NOT open file : "<<name<<endl;
			return;
		}
		fin>>polygon;
		//close the file
		fin.close();
	}
	
	///////////////////////////////////////////////////////////////////////////
	// Save all polygons in acd as *.poly file
	inline void save_polys(ostream& fout, const list<cd_polygon>& polys)
	{
		typedef list<cd_polygon>::const_iterator PIT;
		for( PIT ip=polys.begin();ip!=polys.end();ip++ ) //for each poly
			fout<<*ip;
	}
	
	inline void save_polys(const string& name, const cd_2d& cd)
	{
		//get two list
		const list<cd_polygon>& todo=cd.getTodoList();
		const list<cd_polygon>& done=cd.getDoneList();
	
		//count total polygonal chain
		int totalPC=0;
		typedef list<cd_polygon>::const_iterator PIT;
		{for( PIT ip=todo.begin();ip!=todo.end();ip++ ) totalPC+=ip->size();}
		{for( PIT ip=done.begin();ip!=done.end();ip++ ) totalPC+=ip->size();}
	
		ofstream fout(name.c_str());
		fout<<totalPC<<"\n";
		save_polys(fout,todo);
		save_polys(fout,done);
		fout.close();
	}
	
	///////////////////////////////////////////////////////////////////////////
	//compute the normal vertor to the given vertor v
	inline Vector2d computeNormal(const Vector2d& v)
	{
		Vector2d normal;
		if( v[0]==0 ){
			if(v[1]>0){ normal[0]=1; normal[1]=0; }
			else{ normal[0]=-1; normal[1]=0; }
		}
		else if( v[0]>0 ){
			normal[1]=-1;
			normal[0]=(v[1]/v[0]);
		}
		else{//v[0]<0
			normal[1]=1;
			normal[0]=-(v[1]/v[0]);
		}
		normal=normal.normalize();
		return normal;
	}
	
	
	
	///////////////////////////////////////////////////////////////////////////
	inline void polyBox(const cd_poly& poly, double box[4])
	{
		box[0]=box[2]=FLT_MAX;
		box[1]=box[3]=-FLT_MAX;
	
		cd_vertex * head=poly.getHead();
		cd_vertex * ptr=head;
		do{
			const Point2d& p=ptr->getPos();
			if(p[0]<box[0]) box[0]=p[0];
			if(p[0]>box[1]) box[1]=p[0];
			if(p[1]<box[2]) box[2]=p[1];
			if(p[1]>box[3]) box[3]=p[1];
			ptr=ptr->getNext();
		}
		while(ptr!=head); //end while
	}
	
	inline void polyBox(const cd_2d& cd, double box[4])
	{
		typedef list<cd_polygon>::const_iterator POIT;
		typedef list<cd_poly>::const_iterator PIT;
		const list<cd_polygon>& polys=cd.getTodoList();
		box[0]=box[2]=FLT_MAX;
		box[1]=box[3]=-FLT_MAX;
		
		for( POIT i=polys.begin();i!=polys.end();i++ ){
			for(PIT j=i->begin();j!=i->end();j++){    
				double B[4];
				polyBox(*j,B);
				
				//update the box
				if(B[0]<box[0]) box[0]=B[0];
				if(B[1]>box[1]) box[1]=B[1];
				if(B[2]<box[2]) box[2]=B[2];
				if(B[3]>box[3]) box[3]=B[3];
			}//end j
		}//end i
	}
	
	///////////////////////////////////////////////////////////////////////////
	//compute the center of the poly chain
	inline Point2d polyCenter(double box[4])
	{
		return Point2d((box[0]+box[1])/2,(box[2]+box[3])/2);
	}
	
	///////////////////////////////////////////////////////////////////////////
	//compute the center of the poly chain
	inline Point2d polyCenter(const cd_poly& poly)
	{
		Point2d com;
		int vsize=0;
	
		cd_vertex * head=poly.getHead();
		cd_vertex * ptr=head;
		do{
			com[0]+=ptr->getPos()[0];
			com[1]+=ptr->getPos()[1];
			vsize++;
			ptr=ptr->getNext();
		}while(ptr!=head); //end while
		com[0]/=vsize;
		com[1]/=vsize;
		return com;
	}
	
	///////////////////////////////////////////////////////////////////////////
	//compute the Radius of the poly chain
	inline double polyRadius(const cd_poly& poly, const Point2d& O)
	{
		double R=0;
		cd_vertex * head=poly.getHead();
		cd_vertex * ptr=head;
		do{
			double d=(O-ptr->getPos()).normsqr();
			if(d>R) R=d;
			ptr=ptr->getNext();
		}while(ptr!=head); //end while
		return sqrt(R);
	}
	
	inline double polyRadius
	(const cd_2d& cd, bool useOrigin=false)
	{
		typedef list<cd_polygon>::const_iterator POIT;
		typedef list<cd_poly>::const_iterator PIT;
		const list<cd_polygon>& polys=cd.getTodoList();
		
		double R=0;
		Point2d O(0,0);
	
		for( POIT i=polys.begin();i!=polys.end();i++ ){
			for(PIT j=i->begin();j!=i->end();j++){
				if(!useOrigin)
					O=polyCenter(*j);
				double r=polyRadius(*j,O);
				if(r>R) R=r;
			}//end j
		}//end i
	
		return R;
	}
	
	///////////////////////////////////////////////////////////////////////////
	//count total number of vertices
	inline int countVertices(const cd_polygon& poly)
	{
		int size=0;
		for( PLYCIT ip=poly.begin();ip!=poly.end();ip++ )
			size+=ip->getSize();
		return size;
	}
	
	inline int countVertices(const list<cd_polygon>& poly)
	{
		typedef list<cd_polygon>::const_iterator PIT;
		int size=0;
		for( PIT l=poly.begin();l!=poly.end();l++ )
			size+=countVertices(*l);
		return size;
	}
	
	inline int countVertices(const cd_2d& cd)
	{
		const list<cd_polygon>& todo=cd.getTodoList();
		const list<cd_polygon>& done=cd.getDoneList();
		return countVertices(todo)+countVertices(done);
	}
	
	///////////////////////////////////////////////////////////////////////////
	//count total number of notches
	inline int countNotches(const list<cd_polygon>& poly)
	{
		typedef list<cd_polygon>::const_iterator PIT;
		int size=0;
		for( PIT l=poly.begin();l!=poly.end();l++ ){
			for( PLYCIT ip=l->begin();ip!=l->end();ip++ )
				size+=ip->number_of_notches();
		}
		return size;
	}
	
	inline int countNotches(const cd_2d& cd)
	{
		const list<cd_polygon>& todo=cd.getTodoList();
		const list<cd_polygon>& done=cd.getDoneList();
		return countNotches(todo)+countNotches(done);
	}
	
	///////////////////////////////////////////////////////////////////////////
	// Compute Area of the polygon
	
	inline double A_tri(const Point2d& p1, const Point2d& p2,const Point2d& p3)
	{
		return ((p2[0]-p1[0])*(p3[1]-p1[1])-(p3[0]-p1[0])*(p2[1]-p1[1]))/2;
	}
	
	inline double A_poly(const list<cd_vertex*>& poly)
	{
		typedef list<cd_vertex*>::const_iterator VIT;
		double Area=0;
	
		for(VIT i=poly.begin();i!=poly.end();i++){
			VIT ni=i; ni++;
			if(ni==poly.end())
				ni=poly.begin();
			Area+=A_tri(Point2d(0,0),(*i)->getPos(),(*ni)->getPos());
		}//end for
		return Area;
	}
	
	inline double A_poly(const cd_poly& poly)
	{
		cd_vertex * ptr=poly.getHead();
		cd_vertex * head=ptr;
		double Area=0;
		do{
			cd_vertex * n=ptr->getNext();
			Area+=A_tri(Point2d(0,0),ptr->getPos(),n->getPos());
			ptr=n;
		}while( ptr!=head);
	
		return Area;
	}
	
	inline double A_polygon(const cd_polygon& polygon)
	{
		typedef cd_polygon::const_iterator PIT;
		double Area=0;
		for(PIT i=polygon.begin();i!=polygon.end();i++){
			Area+=A_poly(*i);
		}//end for
		return Area;
	}

}//end namespace acd2d

#endif //_CD2d_UTIL_H_

