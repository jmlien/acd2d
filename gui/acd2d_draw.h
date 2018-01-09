//------------------------------------------------------------------------------
//  Copyright 2007-2008 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _ACD2d_GUI_DRAW_H_
#define _ACD2d_GUI_DRAW_H_

/*
   acd2d_draw.h
   a list of functions that draw path, polygon and vertices using
   opengGL

   copyright (c) 2005, Parasol Lab, Texas A&M University
   
   includes function prototypes for main.cpp
*/

///////////////////////////////////////////////////////////////////////////////
// acd2d headers
#include "acd2d.h"
#include "acd2d_stat.h"
#include "acd2d_bridge.h"

///////////////////////////////////////////////////////////////////////////////
// openggl headers
#include <GL/gli.h>
#include <GL/gliFont.h>

//extern cd_state state;
extern double box[4]; //bbox, defined in acd2d_main_gui.h
int colorid=-1;

inline void drawPoly(const cd_poly& poly) 
{
    //draw vertices
    cd_vertex* ptr=poly.getHead();
    if(ptr==NULL) return;

    glBegin(GL_LINE_LOOP);
    do{
        const Point2d& pt=ptr->getPos();
        glVertex2d(pt[0],pt[1]);
        ptr=ptr->getNext();
    }while( ptr!=poly.getHead() );
    glEnd();
}

void drawPolyNormal(const cd_poly& poly)
{
    //draw normal
    cd_vertex* ptr=poly.getHead();
    if(ptr==NULL) return;

	float length=min(box[1]-box[0],box[3]-box[2])/25;
    glColor3d(0.7,0.5,0);
    glBegin(GL_LINES);
    do{
        const Point2d& pt1=ptr->getPos();
        const Point2d& pt2=ptr->getNext()->getPos();
        double x=(pt1[0]+pt2[0])/2;
        double y=(pt1[1]+pt2[1])/2;
        glVertex2d(x,y);
        const Vector2d& n=ptr->getNormal()*length;
        x=x+n[0];
        y=y+n[1];
        glVertex2d(x,y);
        ptr=ptr->getNext();
    }while( ptr!=poly.getHead() );
    glEnd();
}


inline void drawpolylist(const list<cd_polygon>& pl)
{
    glDisable(GL_LIGHTING);
    list<cd_polygon>::const_iterator ips=pl.begin();
    for( ;ips!=pl.end();ips++ ){
        glPushMatrix();
        const cd_polygon& polys=*ips;
        for( PLYCIT ip=polys.begin();ip!=polys.end();ip++ ){ //for each poly
            drawPoly(*ip);
            glTranslated(0,0,0.01);
        }
        glPopMatrix();
    }   
}

inline void draw(cd_2d& cd2d)
{
    //draw filled polygon
    glDisable(GL_LIGHTING);
    glPushAttrib(GL_CURRENT_BIT);
    //glColor3d(0.95,0.9,0.9);
    glColor3d(0.85,0.85,0.95);
    glCallList(colorid);
    glPopAttrib();

    //draw todo list
    glTranslated(0,0,10);
    glColor3f(0.1f, 0.1f, 0.1f);
    drawpolylist(cd2d.getTodoList());

    //draw done list
    glTranslated(0,0,10);
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0.2f,0.2f,0.2f);
    drawpolylist(cd2d.getDoneList());
    glPopAttrib();

}

inline void drawPolyListNormal(const list<cd_polygon>& pl)
{
    list<cd_polygon>::const_iterator ips=pl.begin();
    for( ;ips!=pl.end();ips++ ){
        glPushMatrix();
        const cd_polygon& polys=*ips;
        for( PLYCIT ip=polys.begin();ip!=polys.end();ip++ ){ //for each poly
            drawPolyNormal(*ip);
            glTranslated(0,0,0.01);
        }
        glPopMatrix();
    }
}

inline void drawNormal(cd_2d& cd2d)
{
    //if( state.show_normal==false ) return;
    drawPolyListNormal(cd2d.getTodoList());
    drawPolyListNormal(cd2d.getDoneList());
}

inline void drawPolyText(cd_2d& cd2d)
{
    char value[128];
    
    //////////////////////////////////////////////
    glTranslated(0,-0.5,0);
    glColor3f(0.2f,0.2f,0.5f);
    sprintf(value,"%d",countVertices(cd2d));
    drawstr(0.2f,0,0,"Number of Vertices: ");
    glColor3f(1,0,0);
    drawstr(5,0,0,value);

    //////////////////////////////////////////////
    glTranslated(0,-0.5,0);
    glColor3f(0.2f,0.2f,0.5f);
    sprintf(value,"%d",countNotches(cd2d));
    drawstr(0.2f,0,0,"Number of Notches: ");
    glColor3f(1,0,0);
    drawstr(5,0,0,value);

    //////////////////////////////////////////////
    glTranslated(0,-0.5,0);
    glColor3f(0.2f,0.2f,0.5f);
    sprintf(value,"%d",(int)cd2d.getDoneList().size());
    drawstr(0.2f,0,0,"Convex Components: ");
    glColor3f(1,0,0);
    drawstr(5,0,0,value);
}

inline void drawTextInfo(cd_2d& cd2d)
{
    glPushAttrib(GL_CURRENT_BIT);

    //draw reference axis
    glMatrixMode(GL_PROJECTION); //change to Ortho view
    glPushMatrix(); 
    glLoadIdentity();
    gluOrtho2D(0,20,0,20);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    
    //Draw text
    glTranslated(0,20,0.1);
    drawPolyText(cd2d);
    
    glPopMatrix();

    //pop GL_PROJECTION
    glMatrixMode(GL_PROJECTION); //change to Pers view
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}

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

inline void drawBridge(cd_2d& cd2d)
{
    glTranslated(0,0,1);
    const list<cd_polygon>& todo=cd2d.getTodoList();    
    typedef list<cd_polygon>::const_iterator PLIT;
    typedef cd_polygon::const_iterator PIT;
    glColor3f(0.9f,0.5f,0.1f);
    {for( PLIT ipl=todo.begin();ipl!=todo.end();ipl++ ){
        const cd_polygon& pg=*ipl;
        for(PIT ip=pg.begin();ip!=pg.end();ip++){
            if( ip->getType()==cd_poly::POUT ) 
                drawBridge(*ip);
        }
    }}
}

///////////////////////////////////////////////////////////////////////////
#ifdef WIN32
extern "C"{
#include "triangulate.h"
}
#else 
#include "triangulate.h"
#endif

void drawFill(const cd_polygon& pl)
{
    typedef cd_polygon::const_iterator   PIT;
    int ringN=pl.size();           //number of rings
    int * ringVN=new int[ringN];     //number of vertices for each ring
    
    int vN=0;             //total number of vertices
    {   int i=0;
    for(PIT ip=pl.begin();ip!=pl.end();ip++,i++){
        vN+=ip->getSize();
        ringVN[i]=ip->getSize();
    }
    }
    
    if( vN<3 ) return;
    int tN=(vN-2)+2*(ringN-1);       //(n-2)+2*(#holes)
    double * V=new double[vN*2];     //to hole vertices pos
    int *T=new int[3*tN];            //to hole resulting triangles
    
    //copy vertices
    {   int i=0;
        for(PIT ip=pl.begin();ip!=pl.end();ip++){
            cd_vertex * ptr=ip->getHead();
            do{
                Point2d pt=ptr->getPos();
                V[i*2]=pt[0];
                V[i*2+1]=pt[1];
                ptr=ptr->getNext();
                i++;
            }while( ptr!=ip->getHead() );
        }
    }
    
    FIST_PolygonalArray(ringN, ringVN, (double (*)[2])V, &tN, (int (*)[3])T);
    {
        glBegin(GL_TRIANGLES);
        for(int i=0;i<tN;i++){
            for(int j=0;j<3;j++){
                int tid=T[i*3+j];
                glVertex2d(V[tid*2],V[tid*2+1]);
            }
        }
        glEnd();
    }
}

void Fill(const list<cd_polygon>& pl)
{
    typedef list<cd_polygon>::const_iterator PIT;
    
    if(colorid>=0) glDeleteLists(colorid,1);
    colorid=glGenLists(1);
    glNewList(colorid,GL_COMPILE);
    for( PIT ip=pl.begin();ip!=pl.end();ip++){
        drawFill(*ip);
    }
    glEndList();


}

#endif //_ACD2d_DRAW_H_
