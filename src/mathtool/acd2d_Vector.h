//------------------------------------------------------------------------------
//  Copyright 2010-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _H_Vector_ACD2D
#define _H_Vector_ACD2D
#pragma once

#include "acd2d_Basic.h"

namespace acd2d
{
    
    /* Vector Descriptions and Operations */
    template<class T, int D=3 >
    class Vector
    {
    public:

        Vector(const T V[D]);
        Vector(const Vector& V);
        Vector(const T& x=0, const T& y=0, const T& z=0, const T& w=0);

        T& operator[](int i){ return v[i]; }
        const T& operator[](int i) const{ return v[i]; }
        
        T norm() const;               // magnitude of vector
        T normsqr() const;            // magnitude squared
        Vector normalize() const;     // normalize
        
        void set(const Vector &other)
        { 
            memcpy(v,other.v,D*sizeof(T));  
        }

        void set(const T other[D])
        { 
            memcpy(v,other,D*sizeof(T)); 
        }

        void set(const T& x=0, const T& y=0, const T& z=0, const T& w=0)
        {
            if( D>0 ) 
                v[0]=x; 
            if( D>1 ) 
                v[1]=y;
            if( D>2 ) 
                v[2]=z; 
            if( D>3 )
                v[3]=w;
        }

        void get(T other[D]) const { memcpy(other,v,D*sizeof(T)); }

        const T* get() const { return v; }

        // Vector operator prototypes
        const Vector& operator=(const Vector& v2); // assignment
        Vector operator-() const;                  // unary negation
        Vector operator+(const Vector& v2) const;  // vector add
        Vector operator-(const Vector& v2) const;  // vector sub
        Vector operator*(const T& s) const;        // scalar multiply
        Vector operator/(const T& s) const;        // division by scalar
        
        Vector operator^(const Vector& v2) const;  // component *
        T operator*(const Vector& v2) const;       // dot product
        Vector operator%(const Vector& v2) const;  // cross product
        bool operator==(const Vector& two) const;  // equality
        
    protected:
        T v[D];
    };
    
    //Implementation
    template<class T, int D >Vector<T,D>::
    Vector(const T V[D])
    {
        set(V);
    }

    template<class T, int D >Vector<T,D>::
    Vector(const T& x, const T& y, const T& z, const T& w)
    {
        set(x,y,z,w);
    }

    template<class T, int D >
    Vector<T,D>::Vector(const Vector<T,D>& V)
    {
        memcpy(v,V.v,sizeof(T)*D);
    }
    
    template<class T, int D >
    T Vector<T,D>::norm() const               // magnitude of vector
    {
        return (T)sqrt(normsqr());
    }
    
    template<class T, int D >
    T Vector<T,D>::normsqr() const            // magnitude squared
    {
        T sumsqr = 0;
        for(int i = 0; i < D; i++)
            sumsqr += sqr(v[i]);
        return sumsqr;
    }
    
    template<class T, int D >
    Vector<T,D> Vector<T,D>::normalize() const     // normalize
    {
        Vector<T,D> newv;
        T magnitude = norm();
        int i;
        for(i=0; i<D; i++){
            newv.v[i] = v[i] / magnitude;
            if(abs(v[i]) > magnitude * HUGENUMBER){
                cerr << "! taking the norm of a zero" << D << " vector" << endl;
                break;
            }
        }
        for(; i < D; i++){
            newv.v[i] = v[i] / magnitude;
        }
        return newv;
    }
    
    template<class T, int D >
    const Vector<T,D>& Vector<T,D>::operator=(const Vector<T,D>& v2)
    {
        memcpy(v,v2.v,D*sizeof(T));
        return *this;
    }
    
    template<class T, int D >
    Vector<T,D> Vector<T,D>::operator-() const
    {
        Vector<T,D> newv(*this);
        for( int i=0;i<D;i++ ) newv.v[i]=-newv.v[i];
        return newv;
    }
    
    template<class T, int D >
    Vector<T,D> Vector<T,D>::operator+(const Vector& v2) const
    {
        Vector<T,D> newv(*this);
        for( int i=0;i<D;i++ ) newv.v[i]+=v2.v[i];
        return newv;
    }
    
    template<class T, int D >
    Vector<T,D> Vector<T,D>::operator-(const Vector& v2) const
    {
        Vector<T,D> newv(*this);
        for( int i=0;i<D;i++ ) newv.v[i]-=v2.v[i];
        return newv;
    }
    
    template<class T, int D >
    Vector<T,D> Vector<T,D>::operator*(const T& s) const
    {
        Vector<T,D> newv(*this);
        for( int i=0;i<D;i++ ) newv.v[i]*=s;
        return newv;
    }
    
    template<class T, int D >
    Vector<T,D> Vector<T,D>::operator/(const T& s) const
    {
        Vector<T,D> newv(*this);
        for( int i=0;i<D;i++ ) newv.v[i]/=s;
        return newv;
    }
    
    template<class T, int D >
    Vector<T,D> Vector<T,D>::operator^(const Vector<T,D>& v2) const
    {
        Vector<T,D> newv(*this);
        for( int i=0;i<D;i++ ) newv.v[i]*=v2.v[i];
        return newv;
    }
    
    template<class T, int D >
    T Vector<T,D>::operator*(const Vector<T,D>& v2) const
    {
        T dot=0;
        for( int i=0;i<D;i++ ) dot+=(v[i]*v2.v[i]);
        return dot;
    }
    
    template<class T, int D >
    Vector<T,D> Vector<T,D>::operator%(const Vector<T,D>& v2) const
    {
        if( D>3 ){
            cerr << "cannot take cross product of " << D << "D Vector";
            exit(1);
        }
        if( D==2 ) return Vector<T,D>(0,0);
        //D==3
        Vector<T,D> newv(*this);
        newv.v[0]=v[1] * v2.v[2] - v[2] * v2.v[1];
        newv.v[1]=v[2] * v2.v[0] - v[0] * v2.v[2];
        newv.v[2]=v[0] * v2.v[1] - v[1] * v2.v[0];
        return newv;
    }
    
    template<class T, int D >
    bool Vector<T,D>::operator==(const Vector<T,D>& other) const
    {
        for(int i=0;i<D;i++) 
            if(v[i]!=other.v[i]) return false;
        return true;
    }

    template<class T, int D >
    Vector<T,D> operator*(T& s, const Vector<T,D>& v)
    {
        return v*s;
    }

    template<class T, int D>
    ostream & operator<<(ostream & out, const Vector<T,D> & vec) {
        for( int d=0;d<D;d++ ) out<<vec[d]<<" ";
        return out;
    }

    template<class T, int D>
    istream & operator>>(istream & in, Vector<T,D> & vec) {
        for( int d=0;d<D;d++ ) 
            in>>vec[d];

        return in;
    }
    
    //Typedef commonly used vector type
    typedef Vector<double,2> Vector2d;
    typedef Vector<double,3> Vector3d;
    typedef Vector<double,4> Vector4d;

} //end of acd2d namespace

#endif
