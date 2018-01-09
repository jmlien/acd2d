///////////////////////////////////////////////////////////////////////
#include "ev_main_gui.h"
#include "ev_draw.h"
#include "acd2d_draw.h"

/*
   ev_main_gui.cpp

   this file constains functions that create GUIs, use functions
   provided by edge visibility

   copyright (c) 2005, Parasol Lab, Texas A&M University
*/

///////////////////////////////////////////////////////////////////////
#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

///////////////////////////////////////////////////////////////////////
// qt/gl/stl headers
#include <GL/gli.h>
#include <GL/gliCamera.h>
#include <qapplication.h>
#include <fstream>
using namespace std;

///////////////////////////////////////////////////////////////////////
// lib headers
#include "edge_visibility.h"
#include "acd2d_util.h"
#include "acd2d_bridge.h"
#include <Point.h>
using namespace mathtool;

///////////////////////////////////////////////////////////////////////
// global varibles

double R=0; // radius of the minimum enclosin cirle 
poly polygon(poly::POUT); // an "empty" polygon
bool b_showpoly=true;     // flag for polygon display

// used for the "visibility" function in this file
list<vertex *> visibleV;    // all visible vertices from visible_from
vertex * visible_from=NULL; // visible_from is a randomly selected vertex

// used for find path
vertex * max_vertex=NULL;            //the vertex with max concavity
list< pair<Point2d, Point2d> > path; //path to the bridge
cd_bridge * bridge;                  //current bridge

///////////////////////////////////////////////////////////////////////
// main
int main( int argc, char ** argv)
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication app(argc, argv);
    
    MainWin win;

    //check file name
    if( argc==2 )   
        win.Init(argv[1]);
    else 
        win.Init("");

    app.setMainWidget( &win );
    win.show();

    return app.exec();
}

///////////////////////////////////////////////////////////////////////////////
//This class handle opengl functions

EVGLWidge::EVGLWidge(QWidget * parent, const char * name)
: QGLWidget(parent,name)
{ 
    setMinimumSize( 500, 500 ); 
    setFocusPolicy(QWidget::StrongFocus);
}

void EVGLWidge::resetCamera()
{
    Point3d pos(0,0,0);
    gliGetCameraFactory().getCurrentCamera()->setCameraPos(pos);
}

void EVGLWidge::initializeGL()
{
    //Antialias
    glShadeModel( GL_SMOOTH );
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    //others
    glEnable( GL_DEPTH_TEST);
    glClearColor( 1, 1, 1, 1 );
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode( GL_FRONT, GL_FILL );
    glPolygonMode( GL_BACK, GL_LINE );
    glPointSize(5);
}

void EVGLWidge::resizeGL( int w, int h)
{
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -R, R, -R, R,0,200);
}

void EVGLWidge::paintGL()
{
    //Init Draw
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslated(0,0,-100);
    gliDraw();

    //draw model
    if( b_showpoly ){
		//draw filled polygon
		glColor3d(0.92,0.92,0.92);
		glCallList(colorid);

		glTranslated(0,0,1);
        glLineWidth(1);
        glColor3f(0.2f, 0.1f, 0.1f);
        drawPoly(polygon);
    }

	//draw results from the "visibility" function
    glColor3f(0.9f, 0.3f, 0.2f);
    drawVisibility(visible_from,visibleV);

	//draw results from the "findpath" function
    glColor3f(0.1f, 0.3f, 0.9f);
    drawPath(path);
    glColor3f(0.9f, 0.0f, 0.0f);
    glPointSize(4);
    drawV(max_vertex);
    drawBridge(polygon);
}

void EVGLWidge::keyPressEvent ( QKeyEvent * e )
{
    e->ignore(); //not handled
}


void EVGLWidge::mousePressEvent( QMouseEvent * e )
{
    if( gliMP(e) ){ updateGL(); return; }//handled by gli
    updateGL();
}

void EVGLWidge::mouseReleaseEvent( QMouseEvent * e )
{
    if( gliMR(e) ){ updateGL(); return; }//handled by gli
    updateGL();
}

void EVGLWidge::mouseMoveEvent ( QMouseEvent * e )
{
    if( gliMM(e) ){ updateGL(); return; }//handled by gli
    updateGL();
}

///////////////////////////////////////////////////////////////////////////////
//more Qt headers

#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qaction.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h> 
#include <qlayout.h> 

//icons 
#include "icon/fileopen.xpm"
#include "icon/original.xpm"
#include "icon/path.xpm"
#include "icon/visible.xpm"
#include "icon/Camera.xpm"

///////////////////////////////////////////////////////////////////////////////
///This class handle widge events

MainWin::MainWin(QWidget * parent, const char * name)
:QMainWindow(parent, name),m_evgl(NULL){}

bool MainWin::Init( const string& modelName )
{
    this->setIcon(QPixmap(fileopen));
    m_Name=modelName;

    //Create opengl window
    if( (m_evgl=new EVGLWidge(this,"edge visibility"))==NULL ) return false;
    setCentralWidget( m_evgl );

    //Create other GUI
    if( CreateGUI()==false ) return false;

    statusBar()->message("Ready");
    return true;
}

/////////////////////////////////////////////////////////////////////
//
//  Protected
//
/////////////////////////////////////////////////////////////////////
bool MainWin::CreateGUI()
{
    //create actions
    QAction * fileOpenAct, * polyAct, * pathAct, * visibleAct, * resetAct;

    fileOpenAct=new QAction("Open File",QPixmap(fileopen),"&Open",CTRL+Key_O,this,"open");
    connect(fileOpenAct,SIGNAL(activated()),this,SLOT(load()));

    polyAct=new QAction("Show Polygon",QPixmap(original),"&Polygon",CTRL+Key_P,this,"sp",true);
    connect(polyAct,SIGNAL(activated()),this,SLOT(showpoly()));
	polyAct->setOn(b_showpoly);

    pathAct=new QAction("Compute Path to Edge",QPixmap(icon_path),"&Edge Path",CTRL+Key_E,this,"ep");
    connect(pathAct,SIGNAL(activated()),this,SLOT(findpath()));

    visibleAct=new QAction("Compute Visiblity",QPixmap(icon_visible),"&Visiblity",CTRL+Key_V,this,"v");
    connect(visibleAct,SIGNAL(activated()),this,SLOT(visibility()));

    resetAct=new QAction("Reset Camera",QPixmap(Camera),"&Camera",CTRL+Key_C,this,"camera");
    connect(resetAct,SIGNAL(activated()),this,SLOT(resetCamera()));

    // populate a tool bar with some actions
    QToolBar* toolbar = new QToolBar( this, "operations" );
    if( toolbar==NULL ) return false;
    toolbar->setLabel( tr( "Operations" ) );
    fileOpenAct->addTo(toolbar); 
    polyAct->addTo(toolbar); 
    pathAct->addTo(toolbar);
    visibleAct->addTo(toolbar);
	resetAct->addTo(toolbar);

    return true;
}

void MainWin::keyPressEvent( QKeyEvent * e )
{
    if( e->key()==Qt::Key_Escape )
        exit(0);
}

/////////////////////////////////////////////////////////////////////
//
//  Slots
//
/////////////////////////////////////////////////////////////////////

// create a polygon from a given filename
bool MainWin::createPoly(poly& p,const string& filename)
{
    ifstream fin(filename.c_str());
    //throw away lines that are # and empty
    int psize=-1; 
    do{
        string comments;
        getline(fin,comments);
        if( !comments.empty() && comments[0]!='#' )
            psize=atoi(comments.c_str());
    }while(psize<0);

    fin>>p;
    fin.close();

	if(psize>1) {
		cerr<<"! Error: No holes please."<<endl;
		return false;
	}
	return true;
}

// get a filename, create a polygon
void MainWin::load()
{
    QString fn = QFileDialog::getOpenFileName( QString::null, "*.ply" , this);
    QFileInfo fi(fn);

    if ( !fn.isEmpty() ){
        QString base = fi.baseName()+"."+fi.extension();
        setCaption("Polygon File: "+base);
        statusBar()->message( "File Loaded : "+fn );
        m_Name=fn.latin1();

		//reset things
        polygon.destroy();  //delete old polygon, if any
        visible_from=NULL;
        path.clear();
        max_vertex=NULL;

		//create a new polygon
		//if( !createPoly(polygon,m_Name) ) return; //failed
		createPoly(polygon,m_Name);

		//create filled polygon
		list<cd_polygon> tmp_list;
		cd_polygon tmp_polygon; 
		tmp_polygon.push_back(polygon);
		tmp_list.push_back(tmp_polygon);
		Fill(tmp_list);

		//update the view
		//radius for determin camera pos, no need to be accurate
		R=polyRadius(polygon,Point2d(0,0))+1;
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
	    glOrtho( -R, R, -R, R,0,200);

		m_evgl->resetCamera();
    }
    else statusBar()->message( "Loading aborted" );
}

//toggle the display flag
void MainWin::showpoly()
{
    b_showpoly=!b_showpoly;
    m_evgl->updateGL();
}

//find paths for vertices to the edge
void MainWin::findpath()
{
    vertex * v=polygon.getHead();
    construct_bridges(v,v->getPre());
    if( v->getBridge()!=NULL )
        v=v->getBridge()->v1;
    vertex * head=v;
    max_vertex=NULL;
    double max_con=0;

    do{
        bridge=v->getBridge();
        if(bridge!=NULL){
            shortest_path_to_edge(bridge->v1,bridge->v2,getPath);
            vertex * ptr=v;
            do{
                if( max_con<ptr->getConcavity() ){
                    max_con=ptr->getConcavity();
                    max_vertex=ptr;
                }
                ptr=ptr->getNext();
            }while(ptr!=bridge->v2);
            v=bridge->v2;
        }
        else{
            v=v->getNext();
        }
    }while(v!=head);
    
    m_evgl->updateGL();
}

//?
void MainWin::getPath(ev_vertex * v)
{
    int size=1;
    vertex * ptr=bridge->v1;
    while(ptr!=bridge->v2){ptr=ptr->getNext();size++;}
    Vector2d vec=bridge->v2->getPos()-bridge->v1->getPos();
    double v_2=vec*vec;
    for( int i=0;i<size;i++ ){
        Point2d vpos=v[i].v->getPos();
        if( v[i].parent!=NULL ){
            pair<Point2d,Point2d> pt
            (vpos,v[i].parent->v->getPos());
            path.push_back(pt);
        }else{
            double t=(vpos-bridge->v1->getPos())*vec/v_2;
            Point2d p=bridge->v1->getPos()+t*vec;
            path.push_back(pair<Point2d,Point2d>(vpos,p));
        }
    }//end for
}

// randomly get a vertex v and compute all vertices 
// that are visible from v 
void MainWin::visibility()
{
    visibleV.clear();
    int num=rand();    //may want to divide the size of the poly here
    visible_from=polygon.getHead();
    for( int i=num;i>0;i-- ) 
        visible_from=visible_from->getNext();
	//compute visible vertices
    visible_vertices(visibleV,visible_from);
    m_evgl->updateGL();
}

void MainWin::resetCamera()
{
    if(m_evgl!=NULL) {
        m_evgl->resetCamera();
        m_evgl->updateGL();
    }
}


