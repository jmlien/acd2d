///////////////////////////////////////////////////////////////////////
//
// Approx Convex Decomp (ACD) GUI main
//
///////////////////////////////////////////////////////////////////////

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

///////////////////////////////////////////////////////////////////////
#include "acd2d_main_gui.h"

///////////////////////////////////////////////////////////////////////
void print_usage(char *name);
void print_gui_usage();
void outputDiagonals();
///////////////////////////////////////////////////////////////////////
bool parseARG(int argc, char ** argv)
{
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            switch(argv[i][1]){
                case 't': g_tau=atof(argv[++i]); break;
                case 'm': g_concavity_measure=argv[++i]; break;
                case 'a': g_alpha=atof(argv[++i]); break;
                case 'b': g_beta=atof(argv[++i]); break;
                case 'g': g_showGL=false; break;
                case 's': g_saveDecomposition=true; break;
                case 'p': g_savePS=true; break;
                case 'c': g_outputCuts=true; break;
                default:
                    //simply ignore....
                    break;
            }
        }
        else{
            g_filename=argv[i];
        }
    }

    //check if everything is valid...
    if(g_filename.empty()){
        cerr<<"! ERROR: No input file"<<endl;
        return false;
    }
    
    cd.updateCutDirParameters(g_alpha,g_beta);

    //everything seems fine, output the input values
    {
		int offset=30;
		cout<<"- Input: "<<g_filename<<"\n";
		cout<<right<<setw(offset)<<"concavity tolerance="<<g_tau<<"\n";
		cout<<right<<setw(offset)<<"concavity weight (alpha)="<<g_alpha<<"\n";
		cout<<right<<setw(offset)<<"distance weight (beta)="<<g_beta<<"\n";
	}
	
    return true;
}

///////////////////////////////////////////////////////////////////////
// main
int main( int argc, char ** argv)
{
    //set some initial values...
    g_alpha=0; 
    g_beta=1; 
    g_concavity_measure="hybrid1";
    g_tau=0;
    g_showGL=true;
    g_saveDecomposition=false;
    g_savePS=false;
    g_outputCuts=false;

    //parse the argument
    if(!parseARG(argc,argv)){
        print_usage(argv[0]);
        return 1;
    }
    
	if(g_showGL)
	{
		/////////////////////////////////////////////////////////////////
		//setup glut/gli
		glutInit( &argc, argv );
		glutInitDisplayMode( GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH );
		glutInitWindowSize( 480, 480);
		glutInitWindowPosition( 50, 50 );
		glutCreateWindow( "ACD2d" );
	
		InitGL();
		gli::gliInit();
		gli::gliDisplayFunc(Display);
		gli::set2DMode(true);
		glutReshapeFunc(Reshape);
		glutKeyboardFunc(Keyboard);
	
		/////////////////////////////////////////////////////////////////
		//loading
		load();
		if(cd.getTodoList().empty()) return 1;
		print_gui_usage();
		Fill(cd.getTodoList()); //Fill defined in draw.h
		/////////////////////////////////////////////////////////////////
		gli::gliMainLoop();
    }
    else //no gui, load geometry and decompose ALL
    {
        load();
        if(cd.getTodoList().empty()) return 1;
    	decomposeAll();
    	if(g_saveDecomposition) save();
    	if(g_savePS) save_PS();
#if SAVE_DIAGONALS	
    	if(g_outputCuts) outputDiagonals();
#endif    	
    }
    
    cd.destroy();
    
    return 0;
}

/////////////////////////////////////////////////////////////////////
//
//  polygon manipulation functions
//
/////////////////////////////////////////////////////////////////////

void load()
{
    createPolys(g_filename,cd);
    resetCamera();
}

void reload()
{
    load();
}

void save()
{
	char tmp[32];
	sprintf(tmp,"%5.3f",g_tau);
    string filename=g_filename+"-acd"+tmp+"-"+g_concavity_measure+".poly";
    save_polys(filename,cd);
    cout<<"- Save to file: "<<filename<<endl;
}

void save_PS()
{
    //////////////////////////////////////////////////////////////////////////////
	char tmp[32];
	sprintf(tmp,"%5.3f",g_tau);
    string filename=g_filename+"-acd"+tmp+"-"+g_concavity_measure+".ps";
    save2PS(filename,cd);
    cout<<"- Save to file: "<<filename<<endl;
}

void decompose()
{
    //double min_d=m_mind->getMinDist();
    IConcavityMeasure * measure=
    ConcavityMeasureFac::createMeasure(g_concavity_measure);
    cd.updateCutDirParameters(g_alpha,g_beta);
    if(g_concavity_measure=="hybrid2")
        ((HybridMeasurement2*)measure)->setTau(g_tau);
    clock_t start=clock();
    cd.decompose(g_tau, measure);
    int time=clock()-start;
    cout<<"- Decompose Once Takes "<<((double)(time))/CLOCKS_PER_SEC<<" secs"<<endl;
    delete measure;
}

void decomposeAll()
{
    IConcavityMeasure * measure=
    ConcavityMeasureFac::createMeasure(g_concavity_measure);
    cd.updateCutDirParameters(g_alpha,g_beta);

    if(g_concavity_measure=="hybrid2")
        ((HybridMeasurement2*)measure)->setTau(g_tau);

    clock_t start=clock();
    cd.decomposeAll(g_tau,measure);
    int time=clock()-start;
    cout<<"- Decompose All Takes "<<((double)(time))/CLOCKS_PER_SEC<<" secs"<<endl;
    delete measure;
}

void show_normal()
{
    state.show_normal=!state.show_normal;
}

void show_hulls()
{
    state.show_hull=!state.show_hull;
}

void show_bridge()
{
    state.show_bridge=!state.show_bridge;
}

void resetCamera()
{
    //reset camera
    gli::setScale(1.25);
    gli::setCameraPosZ(1000);
    gli::setCameraPosX(0);
    gli::setCameraPosY(0);
    gli::setAzim(0);
    gli::setElev(0);
}

void createPolys(const string& filename, cd_2d& cd)
{
    //read polygon
    {
		cd_polygon poly;
		ifstream fin(filename.c_str());
		if( fin.good()==false ){
			cerr<<"! ERROR: can NOT open file : "<<filename<<endl;
			return;
		}
		fin>>poly;
		//close the file
		fin.close();
		
		poly.normalize();
		cd.addPolygon(poly);
    }
    
    //there is nothing todo...
    if(cd.getTodoList().empty()) return;
    
	const cd_poly& poly1=*cd.getTodoList().begin()->begin();
    polyBox(poly1,box);
	O=polyCenter(box);
	double R= sqr(O[0]-box[0])+sqr(O[1]-box[2]);
    R=sqrt((float)R);
    
    //rebuild box
    box[0]=O[0]-R;
    box[1]=O[0]+R;
    box[2]=O[1]-R;
    box[3]=O[1]+R;
}

//-----------------------------------------------------------------------------
// Open GL display
void Display( void )
{
    //Init Draw
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho((box[0]-O[0])*gli::getScale(),
            (box[1]-O[0])*gli::getScale(),
            (box[2]-O[1])*gli::getScale(),
            (box[3]-O[1])*gli::getScale(),
            -100, 500000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gli::gliTranslate();

    glTranslatef(-O[0],-O[1],0);

    //draw model
    glLineWidth(1);
    glColor3f(0.2f, 0.1f, 0.1f);
    draw(cd);
    if(state.show_normal) drawNormal(cd);
    if(state.show_bridge) drawBridge(cd);
    // if(state.show_hull) drawHulls(cd);
    drawTextInfo(cd);
}

void Keyboard( unsigned char key, int x, int y )
{
    switch( key ){
        case 27: exit(0);
        //case 'b': show_bridge(); break;
        case 'n': show_normal(); break;
        case 'h': show_hulls(); break;
        case 'r': resetCamera(); break;
        case 'd': decompose(); break;
        case 'D': decomposeAll(); break;
        case 's': save(); break;
        case 'p': save_PS(); break;
        case ' ': reload(); break;
        case '+' : gli::setScale(gli::getScale()*0.95);
                    break;
        case '-' : gli::setScale(gli::getScale()*1.05);
                    break;
    }

    glutPostRedisplay();
}

//
// print gui key usage
//
void print_gui_usage()
{
    int offset=20;
    cout<<"GUI Usage:\n";
	//cout<<left<<setw(offset)<<"b:"<<"show/hide bridges\n";
	cout<<left<<setw(offset)<<"d:"<<"decompose once\n";
	cout<<left<<setw(offset)<<"D:"<<"decompose all\n";
	cout<<left<<setw(offset)<<"n:"<<"show/hide normal direction \n";
	cout<<left<<setw(offset)<<"h:"<<"show/hide convex hulls\n";	
	cout<<left<<setw(offset)<<"r:"<<"reset camera\n";
	cout<<left<<setw(offset)<<"space bar:"<<"reload polygon\n";
	cout<<left<<setw(offset)<<"s:"<<"save decomposition\n";
	cout<<left<<setw(offset)<<"p:"<<"save rendering to PS file\n";
	cout<<left<<setw(offset)<<"+/-:"<<"zoom in/out\n";
	cout<<left<<setw(offset)<<"arrow keys:"<<"translate\n";
	cout<<flush;
}

void print_usage(char * name)
{
    int offset=20;
	cout<<"Usage: "<<name<<" [-tmabgs] *.poly"<<endl;
	cout<<left<<setw(offset)<<"-t value:"<<"tolerance\n";
	cout<<left<<setw(offset)<<"-m value:"<<"methods: shortestpath (sp), straightline (sl), hybrid1, hybrid2 \n";
	cout<<left<<setw(offset)<<"-a value:"<<"alpha: weight for concavity \n";
	cout<<left<<setw(offset)<<"-b value:"<<"beta: weight for distance \n";
	cout<<left<<setw(offset)<<"-g:"<<"disable OpenGL \n";
	cout<<left<<setw(offset)<<"-s:"<<"save decomposition (when GUI is disabled) \n";
	cout<<left<<setw(offset)<<"-ps:"<<"save decomposition to postscript (PS) file (when GUI is disabled) \n";
#if SAVE_DIAGONALS	
	cout<<left<<setw(offset)<<"-cut:"<<"print diagonals (cuts) to the standard output (works only with -g) \n";
#endif	
	cout<<"\n";
	cout<<left<<setw(offset)<<"Report bugs to: Jyh-Ming Lien jmlien@cs.gmu.edu\n";
	cout<<flush;
}

void outputDiagonals()
{
	const list<cd_diagonal>& diagonals = cd.getDiagonal();
	typedef list<cd_diagonal>::const_iterator IT;

	string segFilename = g_filename + ".seg.acd";
	cout<<"cuts saved to "<<segFilename<<"\n";

	ofstream fout;
	fout.open(segFilename.c_str());

	fout<<"1\n"<<diagonals.size()<<" 3\n";
	for(IT i=diagonals.begin();i!=diagonals.end();i++)
	{
		fout<<i->v[0]<<i->v[1]<<"\n";
	}

	fout.close();
}

