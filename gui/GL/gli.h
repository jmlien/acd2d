/**
  * Interaction class for opengl.
  * OpenGL Interactor. (gliInteractor)
  *
  * 2001/3/19 Jyh-Ming
  */

#ifndef _GLI_INTERACTOR_H_
#define _GLI_INTERACTOR_H_

#ifdef MACOS
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class gli {

    ////////////////////////////////////////////////////////////////////////////////
    //
    //  TypeDefs
    //
    ////////////////////////////////////////////////////////////////////////////////
    
    typedef void (* GLI_DISPLAY_FUNC) ( void );
    typedef void (* GLI_MOUSE_FUNC)   ( int button, int state, int x, int y );
    typedef void (* GLI_MOTION_FUNC)  ( int x, int y );
    typedef void (* GLI_SPECIAL_FUNC)  ( int key, int x, int y );

public:

    ////////////////////////////////////////////////////////////////////////////////
    //
    //  Initializer
    //
    ////////////////////////////////////////////////////////////////////////////////
    
    static void gliInit();
    static void gliMainLoop() { glutMainLoop(); }
    
    ////////////////////////////////////////////////////////////////////////////////
    //
    //  Access Methods
    //
    ////////////////////////////////////////////////////////////////////////////////
    
    static void gliMouseFunc( GLI_MOUSE_FUNC mouse ){
        m_mouseFunc = mouse;
    }
    
    static void gliDisplayFunc( GLI_DISPLAY_FUNC display ){
        m_dislpayFunc = display;
    }

    static void gliMotionFunc( GLI_MOTION_FUNC motion ){
        m_motionFunc = motion;
    }   

    static void gliSpecialKeyFunc(GLI_SPECIAL_FUNC func){
        m_specialFunc=func;
    }

    static void gliTranslate();
    static void gliRotate();

    static void gliDisableMouse(){ m_DisalbeMouseControl=true; }
    static void gliEndMouse(){ m_DisalbeMouseControl=true; }

    static const float * getCameraPos() { return m_CameraPos; }
    static void setCameraPosX(float x) { m_CameraPos[0]=x; }
    static void setCameraPosY(float y) { m_CameraPos[1]=y; }
    static void setCameraPosZ(float z) { m_CameraPos[2]=z; }
    static const float * getWindowX() { return m_WindowX; }
    static const float * getWindowY() { return m_WindowY; }
    static float getAzim() { return m_currentAzim; }
    static float getElev() { return m_currentElev; }
    static float getScale() { return m_Scale*m_deltaScale; }
    static void setScale(float s) { m_Scale=s; m_deltaScale=1; }
    static void setAzim(float v) { m_currentAzim=v; }
    static void setElev(float v) { m_currentElev=v; }
    static void set2DMode(bool flag){ m_2D=flag; }

protected:
    static void gliDisplay( void );
    static void gliMouse( int button, int state, int x, int y );
    static void gliMotion( int x, int y );
    static void gliSpecialFunc(int key, int x, int y);
    static void gliRotateX(float v[3], float degree);
    static void gliRotateY(float v[3], float degree);
    
private:

    static GLI_DISPLAY_FUNC  m_dislpayFunc;
    static GLI_MOUSE_FUNC    m_mouseFunc;   
    static GLI_MOTION_FUNC   m_motionFunc;
    static GLI_SPECIAL_FUNC  m_specialFunc;

    static bool m_DisalbeMouseControl;
    
    static GLfloat m_CameraPos[3];
    static GLfloat m_deltaDis[3];

    static GLfloat m_currentAzim, m_deltaAzim;
    static GLfloat m_currentElev, m_deltaElev;

    static int m_StartX; 
    static int m_StartY; 
    static int m_PressedButton;

    static float m_Scale;
    static float m_deltaScale;

    static float m_WindowX[3];
    static float m_WindowY[3];

    static bool m_2D;
};

#endif //_GLI_INTERACTOR_H_

