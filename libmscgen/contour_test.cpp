/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011 Zoltan Turanyi
    Distributed under GNU Affero General Public License.

    Msc-generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Msc-generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <string>
#include "area.h"
#include "contour.h"

unsigned ContourTestDebug = 0;
unsigned ContourTestDebugMinor = 0;


cairo_status_t write_func4test(void * closure, const unsigned char *data, unsigned length)
{
    if (closure==NULL) return CAIRO_STATUS_SUCCESS;
    if (length == fwrite(data, 1, length, (FILE*)closure))
        return CAIRO_STATUS_SUCCESS;
    else
        return CAIRO_STATUS_WRITE_ERROR;
}

class CairoContext 
{
    FILE *outFile;
    cairo_surface_t *surface;
    const double x;
public:
    cairo_t *cr;
    CairoContext(unsigned i, const Block &place, const char *text=NULL, bool two=true, int sub=-1);
    ~CairoContext();
    void Draw(const Contour& area, bool shifted, double r, double g, double b, bool fill);
    void Draw1(const Contour& area, bool other=false) {Draw(area, false, other?1:0, other?0:1, 0, true);}
    void Draw2(const Contour& area) {Draw(area, true, 0,0,1, true);}
};

CairoContext::CairoContext(unsigned i, const Block &pl, const char *text, bool two, int sub) : x(two?pl.x.Spans()+10:0)
{
    const double expand_by = 20;
    const double size_by = 3;
    Block place(pl);
    place.Expand(expand_by);
    static char fileName[40];
    if (sub<0 && i>999) {
        if (i<=9999) {sub = i%10; i/=10;}
        else if (i<=99999) {sub = i%100; i/=100;}
        else if (i<=999999) {sub = i%1000; i/=1000;}
        else if (i<=9999999) {sub = i%10000; i/=10000;}
        else _ASSERT(0);
    }
    if (sub>=0) 
        sprintf(fileName, "test%d_%03d.png", i, sub);
    else
        sprintf(fileName, "test%d.png", i);
    outFile=NULL;
    surface=NULL;
    cr=NULL;
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, int(two?size_by*2*place.x.Spans():size_by*place.x.Spans()), int(size_by*place.y.Spans()));
    cairo_status_t st;
    st = cairo_surface_status(surface);
    if (st != CAIRO_STATUS_SUCCESS) goto error;
    cr = cairo_create (surface);
    st = cairo_status(cr);
    if (st != CAIRO_STATUS_SUCCESS) goto error;
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, int(2*size_by*place.x.Spans()), int(size_by*place.y.Spans()));
    cairo_fill(cr);
    if (text) {
        cairo_set_source_rgb(cr, 0,0,0);
        cairo_move_to(cr, 5,15);
        cairo_show_text(cr, text);
    }
    cairo_translate(cr, 15, 15);
    cairo_scale(cr, size_by, size_by);
    cairo_translate(cr, -place.x.from, -place.y.from);
    outFile = fopen(fileName, "wb");
    if (outFile) return;
        
    printf("Cannot open %s.\n", fileName);
error:
    if (cr) cairo_destroy(cr);
    if (surface) cairo_surface_destroy(surface);
    outFile=NULL;
    surface=NULL;
    cr=NULL;
}

CairoContext::~CairoContext() 
{
    if (cr) cairo_destroy(cr);
    if (outFile && surface) 
        cairo_surface_write_to_png_stream (surface, write_func4test, outFile);
    if (surface) cairo_surface_destroy(surface);
    if (outFile)
        fclose(outFile);
};

void CairoContext::Draw(const Contour& area, bool shifted, double r, double g, double b, bool fill) 
{
    if (!cr) return;
    if (shifted)
        cairo_translate(cr,x,0);
    if (fill) {
        cairo_set_source_rgba(cr, r, g, b, 0.2); 
        area.Fill(cr);
    }
    cairo_set_source_rgb(cr, r, g, b); 
    area.Line2(cr);
    if (shifted)
        cairo_translate(cr,-x,0);
}

void Draw(unsigned i, const Contour area1, const Contour area2, const Contour area3, const char *text=NULL)
{
    Block b = area1.GetBoundingBox();
    b += area2.GetBoundingBox();
    b += area3.GetBoundingBox();
    CairoContext c(i, b, text);
    c.Draw1(area1);
    c.Draw1(area2, true);
    c.Draw2(area3);
}

inline void Draw(unsigned i, const Contour area1, const Contour area2, const char *text=NULL) {Draw(i, area1, Contour(), area2, text);}
inline void Draw(unsigned i, const Contour area1, const char *text=NULL) {Draw(i, Contour(), Contour(), area1, text);}

void DrawExpand(unsigned i, EExpandType et, const Contour area1, bool manyfile=true, bool singlefile=true, double step=4)
{
    CairoContext *context;
    if (singlefile) 
        context = new CairoContext(i, area1.GetBoundingBox().CreateExpand(40), false);
    const unsigned NUM=3;
    const double r[NUM] = {1,0,0};
    const double g[NUM] = {0,1,0};
    const double b[NUM] = {0,0,1};
    unsigned num=0;
    double gap = -step;
    bool shrinking = true;
    double max_gap;
    //first we find how small we can shrink it (until it disappears),
    //then we do an expand phase to the same extent
    while(shrinking || gap>=0) {
        Contour a = area1.CreateExpand(gap, et);
        if (a.IsEmpty() && shrinking) {
            max_gap = gap *= -1;
            shrinking = false;
            continue;
        }
        if (singlefile) {
            if (gap==0)
                context->Draw(a, false, 0, 0, 0, false);
            else
                context->Draw(a, false, r[num%NUM], g[num%NUM], b[num%NUM], false);
        }
        num++;
        gap-=step;
    }
    if (singlefile) 
        delete context;
    if (manyfile) {
        unsigned num2=0;
        for (gap = -max_gap; gap<=max_gap; gap+=step) {
            char buff[400];
            sprintf(buff, "Inner expanded by %g", gap);
            CairoContext context2(i, area1.GetBoundingBox().CreateExpand(40), buff, false, int(num2));
            context2.Draw(area1.CreateExpand(gap+step, et), false, r[num2%2],     g[num2%2],     b[num2%2],    true);
            context2.Draw(area1.CreateExpand(gap, et),      false, r[(num2+1)%2], g[(num2+1)%2], b[(num2+1)%2], true);
            num2++;
        }
    }
}

void DrawIsinside(unsigned i, const Contour &a, double gap=5, const char *text=NULL)
{
    Block b = a.GetBoundingBox();
    CairoContext c(i, b, text);
    c.Draw(a, false, 0.5, 0.5, 0.5, true);
    for (double x = b.x.from; x<b.x.till; x+=gap)
        for (double y = b.y.from; y<b.y.till; y+=gap) {
            cairo_arc(c.cr, x, y, std::min(2., gap/2), 0, 2*M_PI);
            switch(a.IsWithin(XY(x,y))) {
            case WI_INSIDE: cairo_set_source_rgba(c.cr, 0, 0, 1, 0.5); break;
            case WI_ON_EDGE: cairo_set_source_rgba(c.cr, 1, 0, 0, 0.5); break;
            case WI_ON_VERTEX: cairo_set_source_rgba(c.cr, 0, 1, 0, 0.5); break;
            case WI_IN_HOLE: cairo_set_source_rgba(c.cr, 0.5, 0.5, 0.5, 0.5); break;
            case WI_OUTSIDE: cairo_set_source_rgba(c.cr, 0, 0, 0, 0.5); break;
            }
            cairo_fill(c.cr);
        }
}

//NOW

void contour_test(void)
{
    Contour tri = Contour(XY(50,90), XY(100,60), XY(40,20));
	Draw(100, tri, Contour(30,170,60,70), tri ^ Contour(30,170,60,70));
    tri +=  Contour(30,70,60,70);

    Draw(101, tri, tri.CreateShifted(XY(15, 15)), tri ^ tri.CreateShifted(XY(15, 15)));

	Contour boxhole = Contour(130,170,60,70);
	boxhole += Contour(160,170,60,140);
	boxhole += Contour(130,140,60,140);
	boxhole += Contour(130,170,130,140);
    Draw(102, boxhole, Contour(148,153, 85, 115), boxhole + Contour(148,153, 85, 115));
    boxhole+=Contour(148,153, 85, 115);

	Contour cooomplex;
    Draw(103, boxhole, tri, boxhole + tri);
    cooomplex=boxhole+tri;

	Contour cooomplex2 = Contour(110, 200, 80, 120);
    Draw(104, cooomplex2, Contour(120, 190, 90, 110), cooomplex2 - Contour(120, 190, 90, 110));
    cooomplex2 -= Contour(120, 190, 90, 110);

    Draw(1051, boxhole, cooomplex2, boxhole + cooomplex2, "ADD");
    Draw(1052, boxhole, cooomplex2, boxhole * cooomplex2, "MUL");
    Draw(1053, boxhole, cooomplex2, boxhole ^ cooomplex2, "XOR");
    Draw(1054, boxhole, cooomplex2, boxhole - cooomplex2, "SUB");

    DrawIsinside(1051, boxhole + cooomplex2);
    
    Draw(106, cooomplex2, cooomplex, cooomplex2 + cooomplex);
	cooomplex2 += cooomplex;
    const Contour later = cooomplex2;

	Contour custom = cooomplex2;
	Draw(107, cooomplex2, cooomplex2.CreateShifted(XY(15,15)), cooomplex2 ^ cooomplex2.CreateShifted(XY(15,15)));
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
    Draw(108, cooomplex);
	
    cooomplex2.ClearHoles();

	Draw(109, cooomplex2, Contour(XY(300,101), 100, 50), cooomplex2 ^ Contour(XY(300,101), 100, 50));
    DrawIsinside(1091, cooomplex2 ^ Contour(XY(300,101), 100, 50), 5);

    Draw(110, tri, tri.CreateSwapXYd()); 
    Draw(1101, boxhole, boxhole.CreateSwapXYd()); 
    Draw(1102, cooomplex, cooomplex.CreateSwapXYd()); 
    Draw(1103, Contour(XY(300,101), 100, 50), Contour(XY(300,101), 100, 50).CreateSwapXYd());
    Draw(1104, cooomplex2 ^ Contour(XY(300,101), 100, 50), (cooomplex2 ^ Contour(XY(300,101), 100, 50)).CreateSwapXYd());
    
    cooomplex2 *= Contour(XY(300,101), 100, 50);
	Contour cooomplex3 = cooomplex2;

    double x=150, y=250;
    const XY v1[] = {XY( 50, 200),
	                XY(x, y),
	                XY( 50, 300),
	                XY(x+30, y+40),
	                XY(100, 300),
	                XY(x, y),
                    XY( 90, 300)};
	custom = v1;
    Draw(111, custom);

	Contour custom2;

    x=214, y=610;
    const XY v2[] = {XY( 50, 200),
	                XY(x, y),
	                XY( 50, 300),
	                XY(x+30, y+40),
	                XY(100, 300),
	                XY(x, y),
                    XY( 90, 300)};
    custom = v2;

    x=220, y=610;
    const XY v3[] = {XY( 50, 200),
	                XY(x, y),
	                XY( 50, 300),
	                XY(x+30, y+40),
	                XY(100, 300),
	                XY(x, y),
                    XY( 90, 300)};
    custom2 = v3;
    Draw(112, custom, custom2);

	Contour circle = Contour(XY(200, 200), 60, 30, 30);
    Draw(113, circle, Contour(200,300, 170,190), circle + Contour(200,300, 170,190));
	circle += Contour(200,300, 170,190);
    
	Contour circle2= Contour(XY(x, y), 60, 30, abs(x-y));
    Contour circle3 = circle2;
    Draw(114, circle2, Contour(x,x+100, y+15,y+30), circle2 + Contour(x,x+100, y+15,y+30));
    circle2 += Contour(x,x+100, y+15,y+30);

	Contour boxhole2 = Contour(110, 200, 80, 120);
	boxhole2 -= Contour(120, 190, 90, 110);
	Contour huhu = boxhole2;
	huhu.ClearHoles();
    Draw(115, huhu, Contour(XY(130,101), 30,20), huhu + Contour(XY(130,101), 30,20));
    
    Draw(116, huhu, Contour(XY(130,101), 30,20), huhu * Contour(XY(130,101), 30,20));
	huhu *= Contour(XY(130,101), 30,20);
    DrawIsinside(1161, huhu, 2);
    DrawIsinside(1162, huhu.CreateSwapXYd(), 2);

    DrawIsinside(1163, (boxhole2 - Contour(XY(130,101), 30,20)).CreateSwapXYd(), 2);


	cooomplex2 = cooomplex3;
	cooomplex2.RotateAround(XY(350,100), 39);
    Contour part = cooomplex2.GetNth(1);
    Contour spart = cooomplex3.GetNth(1);
    DrawExpand(135, EXPAND_MITER, part, false);

    Draw(117, cooomplex3, cooomplex2, "SwapXY");
    Draw(1171, cooomplex2, cooomplex2.CreateSwapXYd(), "Rotated ellipse swapXY()");
    DrawIsinside(1172, cooomplex2, 2, "Rotated ellipse is inside");
    DrawIsinside(1173, cooomplex2.CreateSwapXYd(), 2, "Rotated ellipse swapXY()");
    DrawIsinside(1174, part, 2);
    DrawIsinside(1175, spart, 2);
    Contour partxy = part.CreateSwapXYd();
    partxy.IsSane();
    Contour spartxy = spart.CreateSwapXYd();
    spartxy.IsSane();
    DrawIsinside(1176, partxy, 2, "Rotated");
    DrawIsinside(1177, spartxy, 2, "Rotated");

    Contour box = Contour(10, 110, 10, 110) - Contour(30, 40, 30, 80) - Contour(80, 90, 30, 80);
    DrawExpand(129, EXPAND_MITER, box, false);
    DrawExpand(130, EXPAND_MITER, later, false);
    DrawExpand(131, EXPAND_BEVEL, later, false);
    DrawExpand(132, EXPAND_ROUND, later, false);
    DrawExpand(133, EXPAND_MITER, cooomplex, false);
    DrawExpand(134, EXPAND_MITER, huhu, false);
    DrawExpand(135, EXPAND_MITER, cooomplex3, false);
    DrawExpand(136, EXPAND_MITER, cooomplex2, false);

    DrawExpand(137, EXPAND_ROUND, part, false);

    //ContourTestDebug = 200;
    //Draw(1181, part.CreateExpand(-8));


    Contour form1 = Contour(0, 100, 0, 50) + Contour(XY(0, 25), 10, 25) - Contour(XY(100, 25), 10, 25);
    Contour form2 = Contour(0, 100, 0, 50) - Contour(XY(0, 25), 10, 25) + Contour(XY(100, 25), 10, 25);
    Contour form3 = Contour(0, 100, 0, 50) + Contour(XY(0, 25), 10, 35) - Contour(XY(100, 25), 10, 35);
    Contour form4 = Contour(0, 100, 0, 50) - Contour(XY(0, 25), 10, 35) + Contour(XY(100, 25), 10, 35);
    Contour form5 = Contour(0, 100, 0, 50) - Contour(XY(0, 15), 15) - Contour(XY(0, 40), 10);

    DrawExpand(140, EXPAND_MITER, form1, false);
    DrawExpand(141, EXPAND_MITER, form2, false);
    DrawExpand(142, EXPAND_MITER, form3, false);
    DrawExpand(143, EXPAND_MITER, form4, false);
    DrawExpand(144, EXPAND_MITER, form5, false);
    
    Contour lohere1 = Contour(XY(0,0), 50) + Contour(XY(50,0), 50) + 
                      Contour(XY(0,50), 50) + Contour(XY(50,50), 50);

    Contour lohere2 = Contour(0,100, 0,100) + lohere1;
    Contour lohere3 = Contour(0,100, 0,100) - lohere1;
    Contour lohere4 = Contour(0,100, 0,100) +
                      Contour(XY(0,0), 50) - Contour(XY(50,0), 50) + 
                      Contour(XY(0,50), 50) - Contour(XY(50,50), 50);

    DrawExpand(150, EXPAND_MITER, lohere1, false);
    DrawExpand(151, EXPAND_MITER, lohere2, false);
    DrawExpand(152, EXPAND_MITER, lohere3, false);
    DrawExpand(153, EXPAND_MITER, lohere4, false);
    DrawExpand(160, EXPAND_BEVEL, lohere1, false);
    DrawExpand(161, EXPAND_BEVEL, lohere2, false);
    DrawExpand(162, EXPAND_BEVEL, lohere3, false);
    DrawExpand(163, EXPAND_BEVEL, lohere4, false);
    DrawExpand(170, EXPAND_ROUND, lohere1, false);
    DrawExpand(171, EXPAND_ROUND, lohere2, false);
    DrawExpand(172, EXPAND_ROUND, lohere3, false);
    DrawExpand(173, EXPAND_ROUND, lohere4, false);
    

    DrawExpand(180, EXPAND_MITER, Contour(0,40, 0,100) + Contour(XY(60,50), 20), false);
    DrawExpand(181, EXPAND_MITER, Contour(0,40, 0,100) + Contour(XY(50,50), 20), false);
    DrawExpand(182, EXPAND_MITER, Contour(0,40, 0,100) + Contour(XY(50,50), 30,15, 130), false);

    Contour rombusz = Contour(40,80,40,80).CreateRotatedAround(XY(60,60),45);
    DrawExpand(183, EXPAND_MITER, Contour(0,40, 0,100) + rombusz, false);
    DrawExpand(184, EXPAND_MITER, rombusz +rombusz.CreateShifted(XY(40,0)), false);

    Contour concave = Contour(0,40,0,100) - Contour(0,20,30,60) + Contour(XY(20,45), 15);
    DrawExpand(185, EXPAND_MITER, concave, false);

    //Contour aaa(cooomplex2.begin()->GetHoles());
    //Contour bexp=aaa, bexp2;
    //auto i = aaa.begin();
    //bexp += *i;
    //bexp += *++i;
    //bexp += *++i;
    //bexp += *++i;
    //bexp += *++i;
    //bexp += *++i;
    //bexp += *++i;
    //bexp += *++i;
    //bexp += *++i;
    //bexp += *++i;
    //bexp2 += *++i;
    //bexp.RotateAround(XY(300,100), x-y);
    //bexp2.RotateAround(XY(300,100), x-y);
    //Contour bexpp = bexp.CreateExpand((x-y)/10.).Shift(XY(0,100));
    //Contour bexpp2 = bexp2.CreateExpand((x-y)/10.).Shift(XY(0,100));
    //bexp += bexp2;
    //bexp.Shift(XY(0,100));
	//bexpp.Line2(cr);
	//bexpp2.Line2(cr);
    //bexpp += bexpp2;
	
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
}
