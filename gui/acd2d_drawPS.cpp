#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

//-----------------------------------------------------------------------------
// acd2d headers
#include "acd2d_stat.h"
#include "acd2d_drawPS.h"
#include "SimplePSinC.h"

//-----------------------------------------------------------------------------

//extern cd_state state;
extern double box[4]; //bbox, defined in acd2d_main_gui.h

//-----------------------------------------------------------------------------

//desired page width 600
const int PS_page_width=600;
float PS_page_scale=1; //calculated based on box width

inline float X(float x){return (x-box[0])*PS_page_scale;}
inline float Y(float y){return (y-box[2])*PS_page_scale;}
inline float convert(float v){return v*PS_page_scale;}
inline Point2d convert(const Point2d& p) {  return Point2d(X(p[0]),Y(p[1])); }

void draw_PS_ply(SimplePSinC *ps, const cd_poly& ply)
{
	cd_vertex * ptr=ply.getHead();
    Point2d pos=convert(ptr->getPos());
	ps->moveto(pos[0],pos[1]);

	do{
		const Point2d& pos=convert(ptr->getPos());
		ps->lineto(pos[0],pos[1]);
		ptr=ptr->getNext();
	}while(ptr!=ply.getHead());
	
	ps->fillclosepath();
	ps->strokeclosepath();
}

void draw_PS_ply(SimplePSinC *ps, const cd_polygon& polygon)
{
	for(cd_polygon::const_iterator i=polygon.begin();i!=polygon.end();i++)
	{
		if(i->getType()==cd_poly::POUT)
		{
			draw_PS_ply(ps,*i);
		}
		else{
			ps->setfilledrgb(1,1,1);
			ps->setstrokegray(0.5);
			draw_PS_ply(ps,*i);
		}
	}
}


void draw_PS_line(SimplePSinC *ps, const Point2d& s, const Point2d& t)
{
    Point2d p1=convert(s);
    Point2d p2=convert(t);
    ps->line(p1[0],p1[1],p2[0],p2[1]);
    ps->stroke();
}



void draw_PS_circle(SimplePSinC *ps, const Point2d& o, float r)
{
    Point2d s1=convert(o);
    ps->circle(s1[0],s1[1],r);
    ps->fillstroke();
}

inline void draw_PS_polylist(SimplePSinC *ps, const list<cd_polygon>& pl)
{
    list<cd_polygon>::const_iterator ips=pl.begin();
    for( ;ips!=pl.end();ips++ ){
        const cd_polygon& polys=*ips;
        for( PLYCIT ip=polys.begin();ip!=polys.end();ip++ ){ //for each poly
            draw_PS_ply(ps, *ip);
        }
    }   
}

void drawALL(SimplePSinC *ps, cd_2d& cd2d)
{
    //draw todo list
    ps->setfilledrgb(0.3, 0.3, 0.3);
	ps->setstrokegray(0.5);
	draw_PS_polylist(ps, cd2d.getTodoList());

    //draw done list
    ps->setfilledrgb(0.7, 0.7, 0.7);
	ps->setstrokegray(0.5);
    draw_PS_polylist(ps, cd2d.getDoneList()); 
}


void save2PS(const string& name, cd_2d& cd2d) 
{	
	SimplePSinC PS;
	PS.open(name,0,0,PS_page_width,PS_page_width);
    PS.setlinejoin(1);

	drawALL(&PS,cd2d);
	
	PS.close();
	
    cout<<"- Saved screen to "<<name<<endl;
}
