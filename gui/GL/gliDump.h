#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
using namespace std;

#ifndef _WIN32
#define TRUE        1
#define FALSE       0
#endif

#include <GL/gli.h>

typedef unsigned int    uint;
typedef unsigned char   uchar;

//============
// STRUCTURES 
//============

//=====================================================================
// This is the image structure, the raster is contained in here as well
// as all the information needed to display and save it
//---------------------------------------------------------------------
typedef struct
{
    char        filename[256];
    uint        width,height;
    uint        samplesperprimary;
    uint        rgb;
    float       *channel[4];
    uint        *pixels;
    int     comc;
    char        **comv; 
} Image;


uint packcolors(uchar r, uchar g, uchar b);
uchar unpack_red   (uint rgb);
uchar unpack_green (uint rgb);
uchar unpack_blue  (uint rgb);
bool imagemake(int width, int height, Image *image);
void imagesetpixel(uint color, int x, int y, Image *image);
bool imagewrite(const char *filename, Image image);
bool imageread(const char *filename, Image *image);
void myreshape(int iW, int iH);
void mydisplay();
void myidle();
void myvisible(int iState);
bool dump(const char * filename, int w, int h); //dump image to guven filename


