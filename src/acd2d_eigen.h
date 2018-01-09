//------------------------------------------------------------------------------
//  Copyright 2007-2012 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#include <acd2d_Point.h>
#include <acd2d_Vector.h>

#include <vector>
using namespace std;

namespace acd2d
{
	
	/*******************************************************************************
	
	  Adapted from RAPID
	  
	*******************************************************************************/
	
	//compute eigenvector and eigenvalue.
	//vout is eigenvectors and dout is eigenvalues and a is matrix
	
	#define rfabs(x) ((x < 0) ? -x : x)
	#define ROT(a,i,j,k,l) g=a[i][j]; h=a[k][l]; a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau);
	
	inline void
	McM(double Mr[2][2], double M[2][2])
	{
		Mr[0][0] = M[0][0];  Mr[0][1] = M[0][1];
		Mr[1][0] = M[1][0];  Mr[1][1] = M[1][1];
	}
	
	inline void
	VcV(double Vr[2], double V[2])
	{
		Vr[0] = V[0];  Vr[1] = V[1];
	}
	
	int inline
	Meigen(double vout[2][2], double dout[2], double a[2][2])
	{
		int i;
		double tresh,theta,tau,t,sm,s,h,g,c;
		int nrot;
		double b[2];
		double z[2];
		double v[2][2];
		double d[2];
		
		v[0][0] = v[1][1] = 1.0;
		v[0][1] = v[1][0] = 0.0;
		
		b[0] = a[0][0]; d[0] = a[0][0]; z[0] = 0.0;
		b[1] = a[1][1]; d[1] = a[1][1]; z[1] = 0.0;
		
		nrot = 0;
		
		for(i=0; i<50; i++)
		{
			
			sm=fabs(a[0][1]);
			if (sm == 0.0) { McM(vout,v); VcV(dout,d); return i; }
			
			if (i < 2) tresh=0.2*sm/(2*2); else tresh=0.0;
			
			{
				g = 100.0*rfabs(a[0][1]);  
				if (i>3 && rfabs(d[0])+g==rfabs(d[0]) && rfabs(d[1])+g==rfabs(d[1]))
					a[0][1]=0.0;
				else if (rfabs(a[0][1])>tresh)
				{
					h = d[1]-d[0];
					if (rfabs(h)+g == rfabs(h)) t=(a[0][1])/h;
					else
					{
						theta=0.5*h/(a[0][1]);
						t=1.0/(rfabs(theta)+sqrt(1.0+theta*theta));
						if (theta < 0.0) t = -t;
					}
					c=1.0/sqrt(1+t*t); s=t*c; tau=s/(1.0+c); h=t*a[0][1];
					z[0] -= h; z[1] += h; d[0] -= h; d[1] += h;
					a[0][1]=0.0;
					ROT(v,0,0,0,1); ROT(v,1,0,1,1);
					nrot++;
				}
			}
			
			b[0] += z[0]; d[0] = b[0]; z[0] = 0.0;
			b[1] += z[1]; d[1] = b[1]; z[1] = 0.0;      
		}
		
		fprintf(stderr, "eigen: too many iterations in Jacobi transform (%d).\n", i);
		
		return i;
	}
	
	inline 
	double Covariance( const vector<Point2d>& pt, const Point2d& com, int j, int k )
	{
		double r=0;
		int size=pt.size();
		for( int i=0;i<size;i++ )
			r+=((pt[i][j]-com[j])*(pt[i][k]-com[k]));
		
		return r/size;
	}
	
	inline
	void Covariance( double c[2][2], const vector<Point2d>& pt, const Point2d& com )
	{
		for( int j=0;j<2;j++ ){
			for( int k=0;k<2;k++ ){
				c[j][k]=Covariance(pt,com,j,k);
			}
		}
	}
	
	inline
	Vector2d LargestEigenVector(double c[2][2])
	{
		double evec[2][2]={{0,0},{0,0}}; 
		double eval[2]={0,0};
		Vector2d v;
		Meigen(evec,eval,c);
		if( eval[0]>eval[1] )
		{ v[0]=evec[0][0]; v[1]=evec[1][0]; }
		else
		{ v[0]=evec[0][1]; v[1]=evec[1][1]; }
		
		return v;
	}
	
	inline 
	Vector2d PC(const vector<Point2d>& pt, const Point2d& com)
	{
		double c[2][2];
		Covariance(c,pt,com);
		return LargestEigenVector(c);
	}

} //namespace acd2d