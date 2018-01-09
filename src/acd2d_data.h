//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2D_DATA_H_
#define _CD2D_DATA_H_

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <list>
using namespace std;

#include <acd2d_Point.h>
#include <acd2d_Vector.h>

namespace acd2d
{

	class cd_vertex;
	class cd_bridge;
	class hull_2d;
	class IConcavityMeasure;
	
	///////////////////////////////////////////////////////////////////////////////
	
	struct cd_diagonal{
		cd_diagonal(const Point2d& p1, const Point2d& p2){ v[0]=p1; v[1]=p2; }
		Point2d v[2];
	};
	
	struct cd_line{
		cd_line(){ support=NULL; }
		Vector2d normal;
		Vector2d vec; //the vector along the line
		Point2d origin;
		cd_vertex * support;
	};
	
	/**
	 * Vertex of polygon
	 */
	class cd_vertex
	{
	public:
		
		///////////////////////////////////////////////////////////////////////////
		cd_vertex(){ init(); }
		cd_vertex( const Point2d& p ){ pos=p; init(); }
		~cd_vertex();
		void setNext(cd_vertex * n){next=n; if(n!=NULL) n->pre=this; }
		void setPre(cd_vertex * n){pre=n; if(n!=NULL) n->next=this; }
	
		void computeNormal();
		void computeReflex();
		void computeUp(const cd_line& l); //check if this is above the cut line.
		Point2d computeClosePt(const Point2d& pos);
		double computeDist(const Point2d& pos);
	
		///////////////////////////////////////////////////////////////////////////
		void setPos(const Point2d& p) { pos=p; }
		virtual const Point2d& getPos() const { return pos; }
		virtual cd_vertex * getNext() const { return next; }
		virtual cd_vertex * getPre() const { return pre; }
		bool isReflex() const { return reflex; }
		
		///////////////////////////////////////////////////////////////////////////
		void copy(const cd_vertex * other);
		
		//need this?
		const Vector2d& getNormal() const { return normal; }
	
		///////////////////////////////////////////////////////////////////////////
		// For cutting
		const Point2d& getInterPt() const { return inter; }
		double getU() const { return u; } //the coordinate on cut line
		bool isUp() const { return up; }
		bool isIntersect(const cd_line& l);
		void Intersect(const cd_line& l);
	
		///////////////////////////////////////////////////////////////////////////
		// For measuring concavity
		void setBridge(cd_bridge * b){ 
			bridge=b; 
			//if( b==NULL ) concavity=0; //<--- think a better way to do this
		}
		cd_bridge * getBridge() const { return bridge; }
		double getConcavity() const { return concavity; }
		void setConcavity(double c){ concavity=c; }
	
	private:
	
		void init(){
			next=pre=NULL; 
			u=0; up=false; reflex=false;// iswhite=true;
			bridge=NULL; concavity=MAX_CONCAVITY;
		}
		
		//basic info
		Point2d pos;      //position
		Vector2d normal;  //normal, the segment normal from this v to the next.
		bool reflex;      //true if this vertex is reflex
		cd_vertex * next; //next reflexvertex in the polygon
		cd_vertex * pre;  //previous upvertex in the polygon
	
		//for measuring concavity
		cd_bridge * bridge; //what bridge this vertex is in
		double concavity;
		static double MAX_CONCAVITY;
	
		//for cutting (not copied when copy is called)
		Point2d inter;    //intersect point on the seg from this v to the next.
		double u;         //the coordinate on the cut line
		bool up;          //is this v above cut line
	};
	
	/**
	 * Polygon
	 */
	class cd_poly{
	
		friend class cd_2d;
	
	public:
	
		enum POLYTYPE { UNKNOWN, PIN, POUT };
	
		///////////////////////////////////////////////////////////////////////////
		cd_poly(POLYTYPE t){ head=NULL; type=t; intersect=false; radius=-1; }
		//cd_poly(const cd_poly& other);
		void destroy();
	
		///////////////////////////////////////////////////////////////////////////
		// create cd_poly
		void beginPoly();
		void addVertex( double x, double y );
		void addVertex( cd_vertex * v );
		void endPoly();
		
		///////////////////////////////////////////////////////////////////////////
		void copy(const cd_poly& other);
		
		///////////////////////////////////////////////////////////////////////////
		void scale(float f);
		
		///////////////////////////////////////////////////////////////////////////
		typedef pair<cd_vertex*,cd_poly*> IV;   //intersecting vertex
		void findCollEdges( list<cd_vertex*>& coll, cd_line& cut_l);
		pair<cd_vertex*,cd_vertex*>&
		findCW(IConcavityMeasure * measure); //compute concavity witness
		void updateSize();
		cd_vertex* computeClosePt(const Point2d& pos);
	
		///////////////////////////////////////////////////////////////////////////
		// Access
		cd_vertex * getHead() const { return head; }
		POLYTYPE getType() const { return type; }
		void set(POLYTYPE t,cd_vertex * h){ 
			type=t; head=h; 
			if(h!=NULL){ tail=h->getPre(); }
			else{ tail=NULL; }
		}
		bool isInterset() const { return intersect; }
		int getSize() const { return size; }
		void getBridges( list<cd_bridge*>& bridges ) const;
		const pair<cd_vertex*,cd_vertex*>& getCW() const { return m_CW; }
		int number_of_notches() const {
			int count=0;
			cd_vertex * ptr=head;
			do{
				if(ptr->isReflex()) count++;
				ptr=ptr->getNext();
			}
			while(ptr!=head);
			return count;
		}
		
		
		float getRadius();
		const Point2d& getCenter();
		
		///////////////////////////////////////////////////////////////////////////
		// Operator
	
		//check if give poly line is the same as this
		bool operator==( const cd_poly& other ){ return other.head==head; }
		friend istream& operator>>( istream&, cd_poly& );
		friend ostream& operator<<( ostream&, const cd_poly& );
	
	protected:
	
		///////////////////////////////////////////////////////////////////////////
		void doInit(); /*return # of vertice in this poly*/
		void findHoleCW(); //for hole boundary
		void findMaxNotch(IConcavityMeasure * measure); //for out most chain
	
	private:
	
		cd_vertex * head; //the head of vertex list
		cd_vertex * tail; //end of the vertex list
		int size;  //is this used???
	
		//additional info
		Point2d center;
		float radius;
		
		//In, out or unknow.
		POLYTYPE type;
	
		//true if this poly intersects given line, init in findCollEdges.
		bool intersect;
	
		pair<cd_vertex*,cd_vertex*> m_CW; // concavity witness
	};
	
	///////////////////////////////////////////////////////////////////////////////
	//Polygon is a list if poly
	class cd_polygon : public list<cd_poly>{
	
	public: 
		cd_polygon(){}
		void buildDependency();
		cd_poly next(); //get the next polychain to be resolved
		cd_poly& outmost();
		void scale(float factor);
		void normalize();
		bool valid() const; //check if this is a valid polygon
		void copy(const cd_polygon& other);
		void destroy();
		
		friend ostream& operator<<( ostream&, const cd_polygon& );
		friend istream& operator>>( istream&, cd_polygon& );
		
	private:
	
		// Dep_El
		// Data structure for storing decomposition orders
		struct Dep_El{
			Dep_El():m_host(cd_poly::PIN){}
			void remove(Dep_El * dep);
			cd_poly m_host;
			list<Dep_El*> m_depend_on;
			list<Dep_El*> m_depend_by;
		};
	
		//Compute the better cocavity of a hole bounday, p must be a hole
		//better means closer to the out most bounday
		pair<Point2d,Point2d> computeCW(cd_poly& p);
		
		// check if the line segment p1p2 collide with other holes
		// these intersecting holes must be resolved before this...
		void findDependency(const Point2d& p1,const Point2d& p2,Dep_El* pDG);
	
		// delete this element e and its host from this
		cd_poly eraseElement(Dep_El * e);
	
		// Store information of which hole should be decomposed first
		typedef list<Dep_El*> DEL;
		typedef DEL::iterator DIT; 
		DEL m_DependList; //this decides the order of dependency
	};
	
	///////////////////////////////////////////////////////////////////////////////
	//find the out most boudnary in this given polygon
	inline cd_poly& findOutMost(cd_polygon& poly){
		typedef list<cd_poly>::iterator PIT;
		for( PIT ip=poly.begin();ip!=poly.end();ip++ )
			if( ip->getType()==cd_poly::POUT ) return *ip;
		cerr<<"findOutMost: Error: Can't Find outmost polygonal chain"<<endl;
		exit(1);
	}

} //namespace acd2d

#endif
