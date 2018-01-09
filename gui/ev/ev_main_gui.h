ifndef _EV_MAIN_GUI_H_
#define _EV_MAIN_GUI_H_

/*
   main.h
   an user interface for the ev lib

   copyright (c) 2005, Parasol Lab, Texas A&M University
   
   includes function prototypes for main.cpp

   use ev_data.h for main.cpp
*/


///////////////////////////////////////////////////////////////////////////////
// qt/stl headers
#include <qmainwindow.h>
#include <qgl.h>
#include <string>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
#include "ev_data.h"

///////////////////////////////////////////////////////////////////////////////
//This class handle opengl features
class EVGLWidge : public QGLWidget
{
public:

    EVGLWidge( QWidget * parent=0, const char * name=0 );
    void resetCamera();

protected:

    void initializeGL();
    void resizeGL( int, int );
    void paintGL();
    void keyPressEvent ( QKeyEvent * e );


    void mousePressEvent( QMouseEvent * e );
    void mouseReleaseEvent( QMouseEvent * e );
    void mouseMoveEvent ( QMouseEvent * e );
};

///////////////////////////////////////////////////////////////////////////////
//This class handle widge events
class MainWin : public QMainWindow 
{
    Q_OBJECT

public:

    MainWin(QWidget * parent=0, const char * name=0);

    // initialize the obj: create gl view and other gui
    bool Init( const string& modelName );

protected:

    /// create a tool bar
    bool CreateGUI(); 
    
    /// handle key strokes
    virtual void keyPressEvent ( QKeyEvent * e ); 

    /// create a polygon from a file
    bool createPoly(poly& p,const string& filename); 

    //?
    static void getPath(ev_vertex * v);

/// These are qt callback functions
private slots:

    void load();         // load a polygon from a file
    void showpoly();     // enable/disable displaying of the polygon
    void findpath();     // find path from all vertices to an edge
    void visibility();   // compute all visible vertices from a random vertex
	void resetCamera();

/// private data
private:

    string m_Name;          // filename of the polygon
    EVGLWidge * m_evgl;     // open gl gui
};

#endif //_CD2D_MAIN_H_

