//------------------------------------------------------------------------------
//  Copyright 2007-2008 by Jyh-Ming Lien and George Mason University
//  See the file "LICENSE" for more information
//------------------------------------------------------------------------------

#ifndef _CD2D_MAIN_H_
#define _CD2D_MAIN_H_

///////////////////////////////////////////////////////////////////////

#define SAVE_DIAGONALS 1 //flag for saving diagonals (cuts)

///////////////////////////////////////////////////////////////////////
// general header files
#include <string>
#include <fstream>
#include <time.h>
using namespace std;

///////////////////////////////////////////////////////////////////////
//GL headers
#include <GL/gli.h>

///////////////////////////////////////////////////////////////////////
#include "acd2d_main_gui.h"
#include "acd2d.h"
#include "acd2d_stat.h"
#include "acd2d_draw.h"
#include "acd2d_drawPS.h"
#include "acd2d_concavity.h"
#include "acd2d_Point.h"

///////////////////////////////////////////////////////////////////////
//#include <gl2ps.h>

//------------------------------------------------------------------------------
//
// Program inputs
//

string g_filename; // filename
string g_concavity_measure; // concavity_measure name
double g_alpha, g_beta; // for selecting cutting direction
double g_tau; //tolerance
bool   g_showGL; 
bool   g_saveDecomposition;
bool   g_savePS;
bool   g_outputCuts;

///////////////////////////////////////////////////////////////////////
// additional global data

#if SAVE_DIAGONALS
cd_2d cd(true);
#else
cd_2d cd;
#endif

cd_state state;
Point2d O;    //center
double box[4]={0,0,0,0};


//-------------------------------------------------------------------
//
// Define polygon related functions
//
//-------------------------------------------------------------------

void load();
void reload();
void save();
void decompose();
void decomposeAll();
void show_normal();
void show_bridge();
void save_PS();
void createPolys(const string& filename, cd_2d& cd);

//-------------------------------------------------------------------
//
// Some common GL functions
//
//-------------------------------------------------------------------

// functions implemented in .cpp
void Display( void ); 
void DisplayForSaving( void );
void Keyboard( unsigned char key, int x, int y ); 
void resetCamera();

//-----------------------------------------------------------------------------
// regular openGL callback functions
bool InitGL()
{
    // *Antialias*
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    // others
    glEnable( GL_DEPTH_TEST);
    //glClearColor( 0,0,0,0 );
    glClearColor( 1,1,1,0 );
    glPointSize(10);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Let's have light!
    GLfloat Diffuse[] =  { 1,1,1, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuse);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat WhiteLight[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0,GL_DIFFUSE,WhiteLight);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,WhiteLight);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    return true;
}


void Reshape( int w, int h)
{
    if(w>h)
        glViewport( 0, 0, (GLsizei)w, (GLsizei)w );
    else
        glViewport( 0, 0, (GLsizei)h, (GLsizei)h );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(box[0],box[1],box[2],box[3], -100, 500000);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

#endif //_CD2D_MAIN_H_

