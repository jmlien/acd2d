#include "gliTexture.h"
#include <iostream>

unsigned int CreateTexture( const string& file )
{   
    unsigned int tid=-1;
/*    QImage texture, buf;

    if ( !buf.load( file.c_str() ) ) {  // Load first image from file
        cerr<<"! Warning: CreateTexture : "
            <<"Could not read image file, using single-color instead.";
        QImage dummy( 128, 128, 32 );
        dummy.fill( Qt::green.rgb() );
        buf = dummy;
    }
    texture = QGLWidget::convertToGLFormat( buf );  // flipped 32bit RGBA
    
    ////
    glGenTextures(1,&tid);
    glBindTexture(GL_TEXTURE_2D,tid);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,texture.width(), texture.height(),
                      GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());*/
    return tid;
}


