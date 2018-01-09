
/**
 * This code is adapted from Jave SimplePostscript by Marius Watz
 */

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class SimplePSinC {

public:

    SimplePSinC() {
    }

    virtual ~SimplePSinC() {
        close();
    }

    /**
     * Opens a new Postscript file. (x1,y1) - (x2,y2) are the coordinates that
     * define the bounding box. PostScript interpreters might choose to ignore
     * drawing outside this area.
     *
     * @param filename const string& - name of file to be opened.
     * @param x1 float
     * @param y1 float
     * @param x2 float
     * @param y2 float
     * @return success - True if successful, false otherwise.
     */
    bool open(const string& filename, float x1, float y1, float x2, float y2)
    {
        if (psout.is_open())
            psout.close();

        psout.open(filename.c_str());

        //check if everything is fine
        if (psout.good() == false) {
            cerr << "- Error: SimplePSinC failed to open file: " << filename
                    << endl;
            psout.close();
            return false;
        }

        psout << "%!PS-Adobe-3.0 EPSF-3.0\n";
        psout << "%%Creator: SimplePSinC (MASC@GMU), adapted from SimplePostscript (Marius Watz) \n";
        psout << "%%BoundingBox: " << x1 << " " << y1 << " " << x2 << " " << y2<< "\n";
        psout << "%%HiResBoundingBox: " << x1 << " " << y1 << " " << x2 << " "<< y2 << "\n";

        return true;
    }

	//close the file
    void close() 
    {
        if (psout.is_open()) {
            psout << "%%EOF\n";
            psout.flush();
            psout.close();
        }
    }

	//draw line segment
    void line(float a[2], float b[2])
    {
        moveto(a[0], a[1]);
        lineto(b[0], b[1]);
    }

    void line(float a_x, float a_y, float b_x, float b_y)
    {
        moveto(a_x, a_y);
        lineto(b_x, b_y);
    }

    void circle(float x, float y, float rad) {
        moveto(x + rad, y);
        arc(x, y, rad, 0, 360);
    }
    
    // (x1,y1) is the lower left corner
    void square(float x1, float y1, float size)
    {
    	if(size<0) size=-size;
        float x2=x1+size;
        float y2=y1+size;
        rect(x1,y1,x2,y2);
    }
    
    //(x1,y1) and (x2,y2) are diagonal corners
    void rect(float x1, float y1, float x2, float y2)
    {
        if (x1 > x2) {
            float temp = x1;
            x1 = x2;
            x2 = temp;
        }

        if (y1 > y2) {
            float temp = y1;
            y1 = y2;
            y2 = temp;
        }

        moveto(x1, y1);
        lineto(x2, y1);
        lineto(x2, y2);
        lineto(x1, y2);
        lineto(x1, y1);
    }

    void ellipse(float a, float b, float c, float d) {
        float x = a, y = b, w = c, h = d;
        float kappa = 0.5522847498f;

        if (w < 0) { // undo negative width
            x += w;
            w = -w;
        }

        if (h < 0) { // undo negative height
            y += h;
            h = -h;
        }

        y = y;

        moveto(w + x, 0 + y);
        curveto(w + x, kappa * h + y, kappa * w + x, h + y, 0 + x, h + y);
        curveto(-kappa * w + x, h + y, -w + x, kappa * h + y, -w + x, 0 + y);
        curveto(-w + x, -kappa * h + y, -kappa * w + x, -h + y, 0 + x, -h + y);
        curveto(kappa * w + x, -h + y, w + x, -kappa * h + y, w + x, 0 + y);
    }

    void setlinewidth(float w) {
#if DEBUG 
        assert(psout.is_open());
#endif 
        psout << w << " setlinewidth\n";
    }

    void setlinejoin(int c) {
#if DEBUG 
        assert(psout.is_open());
#endif
        psout << c << " setlinejoin\n";
    }

    void setlinecap(int c) {
#if DEBUG
        assert(psout.is_open());
#endif
        psout << c << " setlinecap\n";
    }

    void setdash(const string& str) {
#if DEBUG 
        assert(psout.is_open());
#endif
        psout << str << " setdash\n";
    }

    void setgray(float g) {
#if DEBUG 
        assert(psout.is_open());
#endif
        psout << g << " setgray\n";
    }

    void setstrokegray(float g)
    {
        stringstream ss((stringstream::in | stringstream::out));
        ss<< g << " setgray\n";
        stroke_color=ss.str();
    }

    void setfilledgray(float g)
    {
        stringstream ss((stringstream::in | stringstream::out));
        ss<< g << " setgray\n";
        filled_color=ss.str();
    }

    void setcmyk(float c, float m, float y, float k) {
#if DEBUG 
        assert(psout.is_open());
#endif
        psout << c << " " << m << " " << y << " " << k << " setcmykcolor\n";
    }

    //r, g, b values are between 0 and 255
    void setrgb(int r, int g, int b) {
#if DEBUG 
        assert(psout.is_open());
#endif 
        psout << ((float) r) / 255 << " " << ((float) g) / 255 << " " << ((float) b) / 255 << " setrgbcolor\n";
    }

    //r, g, b values are between 0 and 1
    void setrgb(float r, float g, float b) {
#if DEBUG 
        assert(psout.is_open());
#endif
        psout << r << " " << g << " " << b << " setrgbcolor\n";
    }

    //r, g, b values are between 0 and 1
    void setstrokergb(float r, float g, float b)
    {
        stringstream ss((stringstream::in | stringstream::out));
        ss << r << " " << g << " " << b << " setrgbcolor\n";
    	stroke_color=ss.str();
    }

    //r, g, b values are between 0 and 1
    void setfilledrgb(float r, float g, float b)
    {
        stringstream ss((stringstream::in | stringstream::out));
        ss << r << " " << g << " " << b << " setrgbcolor\n";
        filled_color=ss.str();
    }

    void setfont(const string& fontname) {
#if DEBUG 
        assert(psout.is_open());
#endif 
        psout << "/" << fontname << " findfont setfont\n";
    }

    void setfont(const string& fontname, float pt)
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        psout << "/" << fontname << " findfont " << pt << " scalefont setfont\n";
    }

    void moveto(float x, float y)
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        psout << x << " " << y << " moveto\n";
    }

    void lineto(float x, float y)
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        psout << x << " " << y << " lineto\n";
    }

    void arc(float x, float y, float rad, float deg1, float deg2)
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        psout << x << " " << y << " " << rad << " " << deg1 << " " << deg2
                << " arc\n";
    }

    void arcNegative(float x, float y, float rad, float deg1, float deg2)
    {
#if DEBUG 
        assert(psout.is_open());
#endif
        psout << x << " " << y << " " << rad << " " << deg1 << " " << deg2
                << " arcn\n";
    }

    void curveto(float x1, float y1, float x2, float y2, float x3, float y3)
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        psout << x1 << " " << y1 << " " << x2 << " " << y2 << " " << x3 << " "
                << y3 << " curveto\n";
    }

    void fill()
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        if(filled_color.empty()==false) psout<<filled_color<<"\n";
        psout << "fill\n";
    }

    void fillclosepath()
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        if(filled_color.empty()==false) psout<<filled_color<<"\n";
        psout << "closepath fill\n";
    }

    void stroke()
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        if(stroke_color.empty()==false) psout<<stroke_color<<"\n";
        psout << "stroke\n";
    }

    void strokeclosepath()
    {
#if DEBUG 
        assert(psout.is_open());
#endif 
        if(stroke_color.empty()==false) psout<<stroke_color<<"\n";
        psout << "closepath stroke\n";
    }

    void fillstroke()
    {
        gsave();
        fill();
        grestore();
        stroke();
    }

    void fillstrokeclosepath()
    {
        gsave();
        strokeclosepath();
        grestore();
        fillclosepath();
    }

    void rotate(float deg)
    {
#if DEBUG
        assert(psout.is_open());
#endif
        psout << deg << " rotate\n";
    }

    void scale(float x, float y)
    {
#if DEBUG
        assert(psout.is_open());
#endif
        psout << x << " " << y << " scale\n";
    }

    void translate(float x, float y)
    {
#if DEBUG 
        assert(psout.is_open());
#endif
        psout << x << " " << -y << " translate\n";
    }

    void gsave()
    {
#if DEBUG
        assert(psout.is_open());
#endif
        psout << "gsave\n";
    }

    void grestore()
    {
#if DEBUG
        assert(psout.is_open());
#endif
        psout << "grestore\n";
    }

    void text(float x, float y, const string& s)
    {
        moveto(x, y);
        psout << "(" << s << ") show\n";
    }

    //convert to centimeter units
    void toCM() {
#if DEBUG
        assert(psout.is_open());
#endif
        psout << "matrix currentmatrix /originmat exch def\n";
        psout << "/umatrix {originmat matrix concatmatrix setmatrix} def\n";
        psout << "[28.3465 0 0 28.3465 10.5 100.0] umatrix\n";
    }

private:

    string filled_color;
    string stroke_color;
    ofstream psout;
};
