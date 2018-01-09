//------------------------------------------------------------------------------
//  Copyright 2010-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#if !defined(_Point_H_ACD2D)
#define _Point_H_ACD2D
#pragma once

#include <math.h>
#include "acd2d_Vector.h"

namespace acd2d
{

    //define k-D point
    template<class T, int D=3>
    class Point 
    {
    public:

        ///////////////////////////////////////////////////////////////////////////////////
        //
        //  Constructors
        //
        ///////////////////////////////////////////////////////////////////////////////////

        Point(const T& x=0,const T& y=0,const T& z=0, const T& w=0)
        { 
            set(x,y,z,w); 
        }

        Point(const T V[D])
        {
            set(V);
        }

        Point( const Point & other ) { *this = other; }

        Point( const Vector<T,D>& vec ){ 
            set(vec.get());
        }

        ///////////////////////////////////////////////////////////////////////////////////
        //
        //  Operators
        //
        ///////////////////////////////////////////////////////////////////////////////////
        const Point & operator=( const Point & other ) {
            memcpy(v,other.v,sizeof(T)*D);
            return *this;
        }

        bool operator==( const Point & other ) const {
            for( int i=0;i<D;i++ )
                if( fabs(other.v[i]-v[i])>SMALLNUMBER ) return false;
            return true;
        }

        bool almost_equ( const Point & other ) const {
            for( int i=0;i<D;i++ )
                if( fabs(other.v[i]-v[i])>1e-10 ) return false;
            return true;
        }

        bool operator!=( const Point & other ) const {
            return !(*this==other);
        }

        Vector<T,D> operator-(const Point & other) const {
            Vector<T,D> vec;
            for(int i=0;i<D;i++)
                vec[i]=v[i]-other.v[i];
            return vec;
        }

        Point operator+(const Vector<T,D> & vect) const {
            Point newp;
            for( int i=0;i<D;i++ )
                newp.v[i]=v[i]+vect[i];
            return newp;
        }

        ///////////////////////////////////////////////////////////////////////////////////
        //
        //  Access
        //
        ///////////////////////////////////////////////////////////////////////////////////
        
        void set(const T& x=0,const T& y=0,const T& z=0, const T& w=0) {
            if( D>0 ) v[0]=x; if( D>1 ) v[1]=y;
            if( D>2 ) v[2]=z; if( D>3 ) v[3]=w;
            for(int i=4;i<D;i++) v[i]=0;
        }

        void set(const T other[D]) { memcpy(v,other,sizeof(T)*D); }

        void get(T other[D]) const { memcpy(other,v,sizeof(T)*D); }
        const T * get() const { return v; }
        
        T& operator[]( int i ){ return v[i]; }
        const T& operator[]( int i ) const{ return v[i]; }

    private:
        T v[D];
    };

    template<class T, int D>
    ostream & operator<<(ostream & out, const Point<T,D> & point) {
        for( int d=0;d<D;d++ ) out<<point[d]<<" ";
        return out;
    }

    template<class T, int D>
    istream & operator>>(istream & in, Point<T,D> & point) {
        for( int d=0;d<D;d++ ) 
            in>>point[d];

        return in;
    }

    // Typedef common used vector type
    typedef Point<double,2> Point2d;
    typedef Point<double,3> Point3d;
    typedef Point<double,4> Point4d;
    
}//end of acd2d namespace

#endif // !defined(_Point_ACD2DH_)
