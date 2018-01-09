#include "gliFont.h"
#include <string.h>
GLvoid *font_style = GLUT_BITMAP_HELVETICA_12;

/**
* Draw a string on given (x,y)
*/
void 
drawstr(GLfloat x, GLfloat y, GLfloat z, const char* str)
{
    const char *s;
    
    glRasterPos3f(x, y, z);
    for(s = str; *s; s++)
        glutBitmapCharacter(font_style, *s);
}

/**
* Set font type
*/
void
setfont(const char* name, int size)
{
    font_style = GLUT_BITMAP_HELVETICA_10;
    if (strcmp(name, "helvetica") == 0) {
        if (size == 12) 
            font_style = GLUT_BITMAP_HELVETICA_12;
        else if (size == 18)
            font_style = GLUT_BITMAP_HELVETICA_18;
    } else if (strcmp(name, "times roman") == 0) {
        font_style = GLUT_BITMAP_TIMES_ROMAN_10;
        if (size == 24)
            font_style = GLUT_BITMAP_TIMES_ROMAN_24;
    } else if (strcmp(name, "8x13") == 0) {
        font_style = GLUT_BITMAP_8_BY_13;
    } else if (strcmp(name, "9x15") == 0) {
        font_style = GLUT_BITMAP_9_BY_15;
    }
}



