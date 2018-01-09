//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#include <float.h>
#include "acd2d_data.h"
#include "acd2d_util.h"
#include "acd2d_eigen.h"
#include "acd2d_concavity.h"
#include "acd2d_bridge.h"
#include "acd2d_dir.h"

namespace acd2d
{
	
	double cd_vertex::MAX_CONCAVITY=FLT_MAX;
	
	cd_vertex::~cd_vertex()
	{
		removeBridge(this); 
	}
	
	void cd_vertex::computeNormal()
	{
		normal=acd2d::computeNormal(next->pos-pos);
	}
	
	Point2d cd_vertex::computeClosePt(const Point2d& pos)
	{
		Vector2d vec=(next->pos-pos);
		double t= ((pos-this->pos)*vec)/(vec*vec);
		if( t<0 ) t=0;
		else if(t>1) t=1;
		return this->pos+t*vec;
	}
	
	double cd_vertex::computeDist(const Point2d& pos)
	{
		Point2d pt=computeClosePt(pos);
		return (pos-pt).norm();
	}
	
	void cd_vertex::computeReflex()
	{
		Vector3d v1(pos[0]-pre->pos[0],pos[1]-pre->pos[1],0);
		Vector3d v2(next->pos[0]-pos[0],next->pos[1]-pos[1],0);
		Vector3d v3=v1%v2;
		if( v3[2]<-1e-5 ) reflex=true;
		else reflex=false;
	}
	
	void cd_vertex::computeUp(const cd_line& l)
	{
		if( this==l.support ){ up=pre->up; return;}
		double d=(pos-l.origin)*l.normal;
		if( d>1e-10 )
			up=true;
		else if( d<-1e-10 )
			up=false;
		else{
			up=!pre->up;
		}
	}
	
	void cd_vertex::Intersect(const cd_line& l)
	{
		double t=((l.origin-pos)*l.normal)/((next->pos-pos)*l.normal);
		inter.set((1-t)*pos[0]+t*next->pos[0],(1-t)*pos[1]+t*next->pos[1]);
	}
	
	bool cd_vertex::isIntersect(const cd_line& l)
	{
		next->computeUp(l);
		if( up!=next->up ){
			Intersect(l);
			//compute the i
			Vector2d v=inter-l.origin;
			u=(float)(v.norm());
			if(v*l.vec<0) u=-u;
			return true;
		}
		return false;
	}
	
	void cd_vertex::copy(const cd_vertex * other)
	{
		pos=other->pos;
		normal=other->normal;
		reflex=other->reflex;
		concavity=other->concavity;
	}	
	///////////////////////////////////////////////////////////////////////////////
	
	//build from the other
	/*
	cd_poly::cd_poly(const cd_poly& other)
	{
		*this=other;
		type=other.type;
		head=tail=NULL;
		beginPoly();
		cd_vertex * ptr=other.getHead();
		do{
			const Point2d& pos=ptr->getPos();
			addVertex(pos[0],pos[1]);
			ptr=ptr->getNext();
		}while(ptr!=other.getHead());
		endPoly();
	}
	*/
	
	
	// clean up the space allocated
	void cd_poly::destroy()
	{
		if( head==NULL ) return;
		cd_vertex* ptr=head;
		do{
			cd_vertex * n=ptr->getNext();
			delete ptr;
			ptr=n;
		}while( ptr!=head );
		head=tail=NULL;
	}
	
	// Create a empty polygon
	void cd_poly::beginPoly()
	{
		head=tail=NULL;
	}
	
	// Add a vertex to the polygonal chian
	void cd_poly::addVertex( double x, double y )
	{
		cd_vertex * v=new cd_vertex(Point2d(x,y));
		addVertex(v);
	}
	
	// Add a vertex to the polygonal chian
	void cd_poly::addVertex( cd_vertex * v )
	{
		if( tail!=NULL ){
			tail->setNext(v);
		}
		tail=v;
		if( head==NULL ) head=tail;
	}
	
	// finish building the polygon
	void cd_poly::endPoly()
	{
		if( tail!=NULL ) tail->setNext(head);
		doInit();
	}
	
	// Re-count number of vertices in the polygonal chain
	void cd_poly::updateSize()
	{
		cd_vertex* ptr=head;
		size=0;
		do{
			size++;
			ptr=ptr->getNext();
		}while( ptr!=head );
	}
	
	// initialize property of the this polychain
	// Compute normals and find reflective vertices
	void cd_poly::doInit()
	{
		//compute normals
		cd_vertex* ptr=head;
		size=0;
		do{
			size++;
			ptr->computeNormal();
			ptr=ptr->getNext();
		}while( ptr!=head );
	
		//mark the v that is reflect
		ptr=head;
		do{
			if( type==POUT ) ptr->setConcavity(0);
			ptr->computeReflex();
			ptr=ptr->getNext();
		}while( ptr!=head );
	}
	
	/**
	 * Find the notch that is intersecting with cut_l
	 */ 
	void cd_poly::findCollEdges
	(list<cd_vertex*>& coll, cd_line& cut_l)
	{   
		cd_vertex* first=head;
		if( first==cut_l.support ) first=first->getNext();
		first->computeUp(cut_l);
		cd_vertex* ptr=first;
		do{
			if( ptr->isIntersect(cut_l) ){
				coll.push_back(ptr);
				intersect=true;
			}
			ptr=ptr->getNext();
		}while( ptr!=first );
	}
	
	pair<cd_vertex*,cd_vertex*>& cd_poly::findCW(IConcavityMeasure * measure)
	{
		m_CW.first=m_CW.second=NULL;
		if( type==POUT )
			findMaxNotch(measure);
		else
			findHoleCW();
		return m_CW;
	}
	
	/**
	 * Find the notch with maximum concavity
	 */
	void cd_poly::findMaxNotch(IConcavityMeasure * measure)
	{   
		if( type!=POUT ) cerr<<"! ERROR: findMaxNotch Error"<<endl;
		///////////////////////////////////////////////////////////////////////////
		construct_bridges(head,tail);
		///////////////////////////////////////////////////////////////////////////
		list<cd_bridge*> bridges;
		typedef list<cd_bridge*>::iterator BIT;
		getBridges(bridges);
		cd_vertex * r=NULL;
	
		for( BIT ib=bridges.begin();ib!=bridges.end(); ib++ ){
			cd_bridge * b=*ib;
			if( b->max_r==NULL ) b->FindMaxR(measure); //find max_r
			if( b->max_r==NULL ) continue; //?? wierd, still null...:-<
			if( r==NULL ) r=b->max_r;
			else if(r->getConcavity()<b->max_r->getConcavity())
				r=b->max_r;
		}
		m_CW.first=m_CW.second=r;
	}
	
	// Find hole a pair of concavity witness.
	// Using PA to find it.
	void cd_poly::findHoleCW() 
	{
		if( type!=PIN ) cerr<<"! ERROR: findHoleCW Error"<<endl;
		///////////////////////////////////////////////////////////////////////////
		Point2d com; //center of mass
		vector<Point2d> pts; pts.reserve(size);
		
		//compute com and the point list
		cd_vertex* ptr=head;
		do{
			const Point2d& pt=ptr->getPos();
			pts.push_back(pt);
			com[0]+=pt[0]; com[1]+=pt[1];
			ptr=ptr->getNext();
		}while( ptr!=head );
		com[0]/=size; com[1]/=size;
	
		//compute priciple line
		Vector2d v=PC(pts,com);
	
		//find concavity witness
		double max_u=-1e10, min_u=1e10;
		ptr=head;
		do{
			double u=(ptr->getPos()-com)*v;
			if( u<min_u ){ min_u=u; m_CW.first=ptr; }
			if( u>max_u ){ max_u=u; m_CW.second=ptr; }
			ptr=ptr->getNext();
		}while(ptr!=head); 
	}//end findHoleCW
	
	/**
	 * Get all bridges associated with vertices in the polyline
	 * Take O(n) time.
	 */
	void cd_poly::getBridges( list<cd_bridge*>& bridges ) const
	{
		if( type==PIN ) return; //no bridge for holes
		if( head==NULL ) return;
		cd_vertex * ptr=head;
		do{
			cd_bridge * b=ptr->getBridge();
			if( b==NULL ) ptr=ptr->getNext();
			else{ //in the bridge
				if(!bridges.empty()){ if(bridges.front()==b) break; } //visited
				bridges.push_back(b);
				ptr=b->v2;
			}
		}while(ptr!=head);
	}
	
	void cd_poly::scale(float f)
	{
		cd_vertex * ptr=head;
		do{
			const Point2d& p = ptr->getPos();
			Point2d np(p[0]*f,p[1]*f);
			ptr->setPos(np);
			ptr=ptr->getNext();
		}while(ptr!=head); //end while
	
		radius=-FLT_MAX;
	}
	
	float cd_poly::getRadius()
	{
		if(radius<0) getCenter();
	
		if(radius==0){
			cd_vertex * ptr=head;
			do{
				float d=(center-ptr->getPos()).normsqr();
				if(d>radius) radius=d;
				ptr=ptr->getNext();
			}while(ptr!=head); //end while
			radius=sqrt(radius);
		}
	
		return radius;
	}
	
	
	const Point2d& cd_poly::getCenter()
	{
		if(radius<0){
			center.set(0,0);
			cd_vertex * ptr=head;
			const Point2d& first=ptr->getPos();
			unsigned int size=0;
			do{
				size++;
				Vector2d v=ptr->getPos()-first;
				center[0]+=v[0];
				center[1]+=v[1];
				ptr=ptr->getNext();
			}while(ptr!=head); //end while
			center[0]=(center[0]/size)+first[0];
			center[1]=(center[1]/size)+first[1];
	
			radius=0;
		}
	
		return center;
	}
	
	
	//copy from the given ply
	void cd_poly::copy(const cd_poly& other)
	{
		destroy();//detroy myself first
	
		cd_vertex* ptr=other.head;
		beginPoly();
		do{
			cd_vertex * v=new cd_vertex();
			assert(v); //check for memory
			v->copy(ptr);
			addVertex(v);
			ptr=ptr->getNext();
		}while( ptr!=other.head );
	
		//endPoly();
		//finish up
		tail->setNext(head);
	
		//copy extra info
		center=other.center;
		radius=other.radius;
		type=other.type;
		size=other.size;
		intersect=other.intersect;
		m_CW=other.m_CW;
	}
	
	istream& operator>>( istream& is, cd_poly& poly)
	{
		int vsize; string str_type;
		is>>vsize>>str_type;
		
		if( str_type.find("out")!=string::npos )
			poly.type=cd_poly::POUT;
		else poly.type=cd_poly::PIN;
		
		poly.beginPoly();
		//read in all the vertices
		int iv;
		vector< pair<double,double> > pts; pts.reserve(vsize);
		for( iv=0;iv<vsize;iv++ ){
			double x,y;
			is>>x>>y;
			pts.push_back(pair<double,double>(x,y));
			//double d=x*x+y*y;
		}
		int id;
		for( iv=0;iv<vsize;iv++ ){
			is>>id; id=id-1;
			poly.addVertex(pts[id].first,pts[id].second);
		}
		
		poly.endPoly();
		return is;
	}
	
	ostream& operator<<( ostream& os, const cd_poly& p)
	{
		os<<p.size<<" "<<((p.type==cd_poly::PIN)?"in":"out")<<"\n";
		cd_vertex * ptr=p.head;
		do{
			os<<ptr->getPos()[0]<<" "<<ptr->getPos()[1]<<"\n";
			ptr=ptr->getNext();
		}while(ptr!=p.head);
		
		for(int i=0;i<p.size;i++) os<<i+1<<" ";
		os<<"\n";
		return os;
	}
	
	///////////////////////////////////////////////////////////////////////////////
	//
	// cd_polygon
	//
	///////////////////////////////////////////////////////////////////////////////
	
	//build the decomposing ordering
	void cd_polygon::buildDependency()
	{
		typedef list<cd_poly>::iterator PIT;
	
		//init m_DependList
		for( PIT ip=begin();ip!=end();ip++ ){
			if( ip->getType()==cd_poly::POUT ) continue;
			Dep_El * pDG=new Dep_El();
			pDG->m_host=*ip;
			m_DependList.push_back(pDG);
		}
	
		//for each hole
		for(DIT id=m_DependList.begin();id!=m_DependList.end();id++ ){
			//find its CW
			pair<Point2d,Point2d> line=computeCW( (*id)->m_host );
			findDependency(line.first,line.second,*id);
		}//end for
	}
	
	//get the next polygonal chain to be decomposed
	cd_poly cd_polygon::next()
	{
		for( DIT i=m_DependList.begin();i!=m_DependList.end();i++ ){
			//this poly chain depends no one, ready to be decomposed
			if( (*i)->m_depend_on.empty() ){
				Dep_El * e=*i;
				m_DependList.erase(i);
				return eraseElement(e); //return the vertex
			}
		}//end for
	
		if( m_DependList.empty() )
			return findOutMost(*this);
		cerr<<"! Error: cd_poly: Dependcy Error Found"<<endl;
		exit(1);
	}
	
	void cd_polygon::findDependency
	(const Point2d& p1,const Point2d& p2,Dep_El * pDG)
	{
		typedef list<cd_vertex*>::iterator VIT;
	
		cd_line line;
		line.origin=p1;
		line.vec=(p1-p2).norm();;
		line.normal.set(-line.vec[1],line.vec[0]);
	
		for( DIT id=m_DependList.begin();id!=m_DependList.end();id++ ){
			if( (*id)==pDG ) continue;
			list<cd_vertex*> cd;
			(*id)->m_host.findCollEdges(cd,line);
			if( cd.empty() ) continue;
			for(VIT i=cd.begin();i!=cd.end();i++){
				if( (*i)->getU()>0 ){
					pDG->m_depend_on.push_back(*id);
					(*id)->m_depend_by.push_back(pDG);
					break;
				}
			}//end for
		}
	}
	
	pair<Point2d,Point2d> cd_polygon::computeCW(cd_poly& p)
	{
		//find its CW
		pair<cd_vertex*,cd_vertex*>& cw=p.findCW(NULL);
	
		//find a better CW
		cd_poly& Outmost=findOutMost(*this);
		Point2d MP1=find_MP(cw.first,Outmost);   //find merge point
		Point2d MP2=find_MP(cw.second,Outmost);  //find merge point
	
		//compute distance
		double dist1=(MP1-cw.first->getPos()).normsqr();
		double dist2=(MP2-cw.second->getPos()).normsqr();
	
		//swap if necessary
		if( dist1>dist2 ){ //make cw.1st better than cw.2nd
			cd_vertex * tmp=cw.first;
			cw.first=cw.second;
			cw.second=tmp;
			return pair<Point2d,Point2d>(cw.first->getPos(),MP2);
		}
		return pair<Point2d,Point2d>(cw.first->getPos(),MP1);
	}
	
	// delete this element e and its host from this
	cd_poly cd_polygon::eraseElement(Dep_El * e)
	{
		typedef list<cd_poly>::iterator PIT;
	
		//remove the depend, since this is going to be solved
		DEL& cgl=e->m_depend_by;
		for( DIT j=cgl.begin();j!=cgl.end();j++ )
			(*j)->remove(e);
	
		//retrive the vertex and remove it from the list
		cd_poly p=e->m_host;
		for( PIT i=begin();i!=end();i++ ){
			if( (*i)==p ){
				erase(i);
				break;
			}
		}
	
		delete e;
		return p;
	}
	
	void cd_polygon::Dep_El::remove(Dep_El * dep)
	{
		//DIT i=find(m_depend_on.begin(),m_depend_on.end(),dep);
		//int size=m_depend_on.size();
		DIT i;
		for( i=m_depend_on.begin();i!=m_depend_on.end();i++){
			if( *i==dep )
				break;
		}
	
		if( i!=m_depend_on.end() )
			m_depend_on.erase(i);
		else
			cerr<<"cd_polygon::Dep_El::remove ERROR"<<endl;
	}
	
	void cd_polygon::scale(float factor)
	{
		for(iterator i=begin();i!=end();i++)
			i->scale(factor);
	}
	
	void cd_polygon::normalize()
	{
		float r=front().getRadius();
		scale(1.0/r);
	}
	
	bool cd_polygon::valid() const //check if this is a valid polygon
	{
		if(empty()) return false;
		if(front().getType()!=cd_poly::POUT) return false;
		for(const_iterator i=++begin();i!=end();i++) if(i->getType()!=cd_poly::PIN) return false;
	
		return true;
	}
	
	//copy from the given polygon
	void cd_polygon::copy(const cd_polygon& other)
	{
		//destroy myself
		destroy();
	
		for(const_iterator i=other.begin();i!=other.end();i++){
			cd_poly p(cd_poly::UNKNOWN);
			p.copy(*i);
			push_back(p);
		}
	}
	
	void cd_polygon::destroy()
	{
		for(iterator i=begin();i!=end();i++){
			i->destroy();
		}
		clear(); //remove all ply from this list
	}
	
	
	ostream& operator<<( ostream& out, const cd_polygon& p)
	{
		typedef list<cd_poly>::const_iterator PIT;
		for(PIT i=p.begin();i!=p.end();i++)
			out<<*i;
		return out;
	}
	
	
	istream& operator>>( istream& is, cd_polygon& p)
	{
		//remove header comments
		do{
			char tmp[1024];
			char c=is.peek();
			if(isspace(c)) is.get(c); //eat it
			else if(c=='#') {
				is.getline(tmp,1024);
			}
			else break;
		}while(true);
	
		//start reading
		unsigned int size;
		is>>size;
		unsigned int vid=0;
		for(unsigned int i=0;i<size;i++){
			cd_poly poly(cd_poly::UNKNOWN);
			is>>poly;
			p.push_back(poly);
		}
		return is;
	}
	
	
	///////////////////////////////////////////////////////////////////////////////
	//
	// cd_bridge
	//
	///////////////////////////////////////////////////////////////////////////////
	
	void cd_bridge::FindMaxR(IConcavityMeasure * measure)
	{
		max_r=measure->findMaxNotch(v1,v2);
	}
	
}//namespace acd2d
