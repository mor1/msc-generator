// ContourTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include "contour_area.h"

cairo_status_t write_func4test(void * closure, const unsigned char *data, unsigned length)
{
    if (closure==NULL) return CAIRO_STATUS_SUCCESS;
    if (length == fwrite(data, 1, length, (FILE*)closure))
        return CAIRO_STATUS_SUCCESS;
    else
        return CAIRO_STATUS_WRITE_ERROR;
}

class Context 
{
    FILE *outFile;
    cairo_surface_t *surface;
    cairo_t *cr;
    const double x;
    void Draw(const Area& area, bool shifted, int other=0);
public:
    Context(unsigned i, const XY &size);
    ~Context();
    void Draw1(const Area& area, bool other=false) {Draw(area, false, other?1:0);}
    void Draw2(const Area& area) {Draw(area, true, 2);}
};

Context::Context(unsigned i, const XY &size) : x(size.x)
{
    static char fileName[40];
    sprintf_s(fileName, "test%03d.png", i);
    outFile=NULL;
    surface=NULL;
    cr=NULL;
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, int(2*size.x), int(size.y));
    cairo_status_t st;
    st = cairo_surface_status(surface);
    if (st != CAIRO_STATUS_SUCCESS) goto error;
    cr = cairo_create (surface);
    st = cairo_status(cr);
    if (st != CAIRO_STATUS_SUCCESS) goto error;
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, int(2*size.x), int(size.y));
    cairo_fill(cr);
    if (!fopen_s(&outFile, fileName, "wb")) return;
        
    printf((std::string("Cannot open ")+fileName+".\n").c_str());
error:
    if (cr) cairo_destroy(cr);
    if (surface) cairo_surface_destroy(surface);
    outFile=NULL;
    surface=NULL;
    cr=NULL;
}

Context::~Context() 
{
    if (cr) cairo_destroy(cr);
    if (outFile && surface) 
        cairo_surface_write_to_png_stream (surface, write_func4test, outFile);
    if (surface) cairo_surface_destroy(surface);
    if (outFile)
        fclose(outFile);
};

void Context::Draw(const Area& area, bool shifted, int other) 
{
    if (!cr) return;
    if (shifted)
        cairo_translate(cr,x,0);
    switch (other) {
        case 0: cairo_set_source_rgba(cr, 1, 0, 0, 0.2); break;
        case 1: cairo_set_source_rgba(cr, 0, 1, 0, 0.2); break;
        case 2: cairo_set_source_rgba(cr, 0, 0, 1, 0.2); break;
    }
    area.Fill(cr);
    switch (other) {
        case 0: cairo_set_source_rgb(cr, 1, 0, 0); break;
        case 1: cairo_set_source_rgb(cr, 0, 1, 0); break;
        case 2: cairo_set_source_rgb(cr, 0, 0, 1); break;
    }
    area.Line2(cr);
    if (shifted)
        cairo_translate(cr,-x,0);
}

void Draw(unsigned i, const Area area1, const Area area2, const Area area3)
{
    Block b = area1.GetBoundingBox();
    b += area2.GetBoundingBox();
    b += area3.GetBoundingBox();
    Context c(i, b.LowerRight()+XY(10,10));
    c.Draw1(area1);
    c.Draw1(area2, true);
    c.Draw2(area3);
}

inline void Draw(unsigned i, const Area area1, const Area area2) {Draw(i, area1, Area(), area2);}
inline void Draw(unsigned i, const Area area1) {Draw(i, Area(), Area(), area1);}

//NOW

int _tmain(int argc, _TCHAR* argv[])
{
    Area tri = Contour(XY(50,90), XY(100,60), XY(40,20));
	Draw(0, tri, Contour(30,50,60,70), tri + Contour(30,50,60,70));
    tri +=  Contour(30,50,60,70);

	Area boxhole = Contour(130,170,60,70);
	boxhole += Contour(160,170,60,140);
	boxhole += Contour(130,140,60,140);
	boxhole += Contour(130,170,130,140);
    Draw(1, boxhole, Contour(148,153, 85, 115), boxhole + Contour(148,153, 85, 115));
    boxhole+=Contour(148,153, 85, 115);

	Area cooomplex;
    Draw(2, boxhole, tri, boxhole + tri);
    cooomplex=boxhole+tri;

	Area cooomplex2 = Contour(110, 200, 80, 120);
    Draw(3, cooomplex2, Contour(120, 190, 90, 110), cooomplex2 - Contour(120, 190, 90, 110));
    cooomplex2 -= Contour(120, 190, 90, 110);

    Draw(4, cooomplex2, cooomplex, cooomplex2 + cooomplex);
	cooomplex2 += cooomplex;

	Area custom = cooomplex2;
	Draw(5, cooomplex2, cooomplex2.CreateShifted(XY(15,15)), cooomplex2 + cooomplex2.CreateShifted(XY(15,15)));
    cooomplex2 += cooomplex2.CreateShifted(XY(15,15));
	
    cooomplex2.Shift(XY(200,0));
	custom. Shift(XY(200,0));
    
    cooomplex.clear();
	const int num_x = 10;
	const int num_y = 10;
	for (int i=0; i<num_x; i++)
		cooomplex += Contour(200+i*20, 215+i*20, 200, 190+num_y*20);
	for (int j=0; j<num_y; j++)
		cooomplex += Contour(200, 190+num_x*20, 200+j*20, 215+j*20);
    Draw(6, cooomplex);
	
    cooomplex2.ClearHoles();

	Draw(7, cooomplex2, Contour(XY(300,101), 100, 50), cooomplex2 * Contour(XY(300,101), 100, 50));
    cooomplex2 *= Contour(XY(300,101), 100, 50);
	Area cooomplex3 = cooomplex2;

    double x=150, y=250;
	std::vector<XY> v;
	v.push_back(XY( 50, 200));
	v.push_back(XY(x, y));
	v.push_back(XY( 50, 300));
	v.push_back(XY(x+30, y+40));
	v.push_back(XY(100, 300));
	v.push_back(XY(x, y));
	v.push_back(XY( 90, 300));
	custom = v;
    Draw(8, custom);

	Area custom2;

    x=214, y=610;
	v.clear();
	v.push_back(XY( 50, 200));
	v.push_back(XY(x, y));
	v.push_back(XY( 50, 300));
	v.push_back(XY(x+30, y+40));
	v.push_back(XY(100, 300));
	v.push_back(XY(x, y));
	v.push_back(XY( 90, 300));
    custom = v;

    x=220, y=610;
	v.clear();
	v.push_back(XY( 50, 200));
	v.push_back(XY(x, y));
	v.push_back(XY( 50, 300));
	v.push_back(XY(x+30, y+40));
	v.push_back(XY(100, 300));
	v.push_back(XY(x, y));
	v.push_back(XY( 90, 300));
    custom2 = v;
    Draw(9, custom, custom2);

	Area circle = Contour(XY(200, 200), 60, 30, 30);
	Area circle2= Contour(XY(x, y), 60, 30, abs(x-y));
	//Area circle2= Contour(XY(339, 103), 60, 30, 150);
	//Area circle2= Contour(XY(337, 103), 60, 30, 150);
	//Contour circle2(XY(200,200), 60, 30, 150);
	//Contour box(x-30, x+30, y-20, y+20);
	//circle += Contour(200,300, 170,190);
	//circle2 += Contour(x,x+100, y+15,y+30);
//
//	Area boxhole2 = Contour(110, 200, 80, 120);
//	boxhole2 -= Contour(120, 190, 90, 110);
//	//boxhole2.Shift(XY(x-110, y-80));
//	boxhole2.Shift(XY(0, 100));
//
//	//circle2.Line(cr);
//	cooomplex2 = cooomplex3;
//	//cooomplex2.RotateAround(XY(350,100), (x-y)/(M_PI*180));
//	//cooomplex2.Line2(cr);
//
//	//custom.Line2(cr);
//	//boxhole.Line2(cr);
//
//	//cooomplex.Line2(cr);
//	//custom.Line2(cr);
//	//boxhole2.Line(cr);
//	//Area(Contour(XY(130,201), 30, 20)).Line2(cr);
//	Area huhu = boxhole2;
//	//huhu.ClearHoles();
//	//huhu *= Contour(XY(130,201), 30,20);
//	//huhu.Line2(cr);
//    //x=363; y=188;
//    cooomplex2.RotateAround(XY(300,100), x-y);
//	Area bexp = cooomplex2.CreateExpand((x-y)/10).CreateShifted(XY(0,100));
//    //Area aaa(cooomplex2.begin()->GetHoles());
//    //Area bexp=aaa, bexp2;
//    //auto i = aaa.begin();
//    //bexp += *i;
//    //bexp += *++i;
//    //bexp += *++i;
//    //bexp += *++i;
//    //bexp += *++i;
//    //bexp += *++i;
//    //bexp += *++i;
//    //bexp += *++i;
//    //bexp += *++i;
//    //bexp += *++i;
//    //bexp2 += *++i;
//    //bexp.RotateAround(XY(300,100), x-y);
//    //bexp2.RotateAround(XY(300,100), x-y);
//    //Area bexpp = bexp.CreateExpand((x-y)/10.).Shift(XY(0,100));
//    //Area bexpp2 = bexp2.CreateExpand((x-y)/10.).Shift(XY(0,100));
//    //bexp += bexp2;
//    //bexp.Shift(XY(0,100));
//	//bexpp.Line2(cr);
//	//bexpp2.Line2(cr);
//    //bexpp += bexpp2;
//	bexp.Line2(cr);
//
//    //double offset = cooomplex3.OffsetBelow(bexp);
//    //bexp.Shift(XY(0,-offset));
//    //cooomplex3.Line2(cr);
//	//bexp.Line2(cr);
//	//bexpp.Line2(cr);
//
//	cairo_set_source_rgb(cr, 1, 0, 0);
//	//(boxhole + boxhole2).Line2(cr); 
//	//(cooomplex2 * circle2).Line2(cr);
//	cairo_set_source_rgb(cr, 0, 0, 1);
//	//(cooomplex * circle2).Fill(cr);
//	cairo_set_source_rgb(cr, 0, 0, 0);
////	(pl4 * circle2).Fill(cr);
//
//	//double r[4];
//	//int a = quartic_solve(1, 0, 0, 0, 0, r);
//	//int b = cubic_solve(1, -6, 23, -6, r);
//	//for (int i = 0; i<b; i++) {
//	//	double k = cubic_substitute(1, -6, 23, -6, r[i]);
//	//	k = k;
//	//}
//	//int c = cubic_solve(1, -6, 11, -6, r);
//
//
//	//XY off(150, 30);
//	//Edge e1(XY(x-75, y));
//	//Edge e2(XY(200,200), 50, 100, 30);
//	//Edge e3(XY(x, y), 100, 50, 60);
//
//	//cairo_move_to(cr, e3.GetStart().x, e3.GetStart().y);
//	//e3.Path(cr, e3.GetStart()+off.Rotate90CW());
//	//cairo_stroke(cr);
//	//cairo_move_to(cr, e2.GetStart().x, e2.GetStart().y);
//	//e2.Path(cr, e2.GetStart()+off);
//	//cairo_stroke(cr);
//	//
//	//(Contour(XY(200,200), 50, 100, 30) * Contour(XY(x, y), 100, 50, 60)).Fill(cr);
//
//	//XY xy[4];
//	//double pos1[4], pos2[4];
//	//int num = Edge::Crossing(e3, e3.GetStart()+off.Rotate90CW(), e2, e2.GetStart()+off, xy, pos1, pos2);
//	//for (int i=0; i<num; i++) {
//	//	cairo_set_source_rgba(cr, 0, 1, 0, 0.8);
//	//	cairo_arc(cr, xy[i].x, xy[i].y, 5, 0, 2*3.14);
//	//	cairo_close_path(cr);
//	//	cairo_fill(cr);
//
//	//	char buff[200];
//	//	sprintf(buff, "%d: pos1:%f, pos2:%f", i, pos1[i], pos2[i]);
//	//	cairo_move_to(cr, xy[i].x+10, xy[i].y);
//	//	cairo_show_text(cr, buff);
//	//}
	return 0;
}

