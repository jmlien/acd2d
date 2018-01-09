#ifndef _EV_DRAW_H_
#define _EV_DRAW_H_

/*
   ev_draw.h
   a list of functions that draw path, polygon and vertices using
   opengGL

   copyright (c) 2005, Parasol Lab, Texas A&M University
   
   includes function prototypes for ev_main_gui.cpp
*/

///////////////////////////////////////////////////////////////////////////////
// ev headers
#include "ev_data.h"

///////////////////////////////////////////////////////////////////////////////
// openggl headers
#include <GL/glut.h>
#include "GL/gliFont.h"
//#include "acd2d_bridge.h"

///////////////////////////////////////////////////////////////////////////////
inline void drawPath(list<pair<Point2d, Point2d> >& path)
{
    typedef list< pair<Point2d, Point2d> >::iterator PIT;
    glBegin(GL_LINES);
    for( PIT ip=path.begin();ip!=path.end();ip++ ){
        Point2d& p1=ip->first;
        Point2d& p2=ip->second;
        glVertex2d(p1[0],p1[1]);
        glVertex2d(p2[0],p2[1]);
    }
    glEnd();
}

///////////////////////////////////////////////////////////////////////////////
inline void drawVisibility(vertex * v,list<vertex *>& visibleV)
{
    if( v==NULL ) return;
    const Point2d& spt=v->getPos();
    typedef list<vertex *>::iterator VIT;
    glBegin(GL_LINES);
    for( VIT iv=visibleV.begin();iv!=visibleV.end();iv++ ){
        const Point2d& pt=(*iv)->getPos();
        glVertex2d(spt[0],spt[1]);
        glVertex2d(pt[0],pt[1]);
    }//end for
    glEnd();
}//end

///////////////////////////////////////////////////////////////////////////////
// draw a vertex
inline void drawV(vertex * v)
{
    if(v==NULL ) return;
    glBegin( GL_POINTS );
    glVertex2d( v->getPos()[0], v->getPos()[1] );
    glEnd();
}

///////////////////////////////////////////////////////////////////////////////
// draw numbers near vertices
inline void drawNumber(vertex* s,vertex* e)
{
    vertex* ptr=s;
    char str_id[32];
    int id=1;
    do{
        const Point2d& pt=ptr->getPos();
        sprintf(str_id,"%d",id++);
        drawstr(pt[0],pt[1],0,str_id);
        ptr=ptr->getNext();
    }while( ptr!=e );
}

///////////////////////////////////////////////////////////////////////////////
/*
inline void drawBridge(const cd_poly & poly)
{
    list<cd_bridge*> bridges;

    //get bridges
    poly.getBridges(bridges);
    if( bridges.empty() ) return;

    //draw bridges
    typedef list<cd_bridge*>::const_iterator BIT;
    glPushAttrib(GL_CURRENT_BIT);
    {
        glColor3d(1,0.4,0);
        glBegin(GL_LINES);
        for(BIT ib=bridges.begin();ib!=bridges.end();ib++ ){
            const Point2d& p1=(*ib)->v1->getPos();
            const Point2d& p2=(*ib)->v2->getPos();
            glVertex2d(p1[0],p1[1]);
            glVertex2d(p2[0],p2[1]);
        }
        glEnd();
    }

    {
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3d(0,1,0);
        for(BIT ib=bridges.begin();ib!=bridges.end();ib++ ){
            const Point2d& p1=(*ib)->v1->getPos();
            const Point2d& p2=(*ib)->v2->getPos();
            glVertex2d(p1[0],p1[1]);
            glVertex2d(p2[0],p2[1]);
        }
        glEnd();
    }

    { //mark the max concavity notch in the pocket
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3d(1,0,0);
        for(BIT ib=bridges.begin();ib!=bridges.end();ib++ ){
            cd_bridge * b=*ib;
            if( b->max_r==NULL ) continue;
            const Point2d& p1=b->max_r->getPos();
            glVertex2d(p1[0],p1[1]);
        }
        glEnd();
    }

    glPopAttrib();
}
*/

#endif //_EV_DRAW_H_
