/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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
#include "contour.h"

namespace contour {

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
        else {_ASSERT(0);}
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
    const XY c = area.Centroid();
    cairo_arc(cr, c.x, c.y, 5, 0, 2*M_PI);
    cairo_fill(cr);
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

void Draw(unsigned i, const Contour area1, const Contour area2, const char *text=NULL) {Draw(i, area1, Contour(), area2, text);}
void Draw(unsigned i, const Contour area1, const char *text=NULL) {Draw(i, Contour(), Contour(), area1, text);}

void DrawExpand(unsigned i, EExpandType et, double limit, const Contour area1, bool manyfile=true, bool singlefile=true, double step=4)
{
    CairoContext *context=NULL;
    if (singlefile) 
        context = new CairoContext(i, area1.GetBoundingBox().CreateExpand(100), NULL, false);
    const unsigned NUM=3;
    const double r[NUM] = {1,0,0};
    const double g[NUM] = {0,1,0};
    const double b[NUM] = {0,0,1};
    unsigned num=0;
    double gap = -step;
    bool shrinking = true;
    double max_gap=100;
    //first we find how small we can shrink it (until it disappears),
    //then we do an expand phase to the same extent
    while(shrinking || gap>=0) {
        Contour a = area1.CreateExpand(gap, et, et, limit, limit);
        if (gap<-40 && shrinking) {
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
            context2.Draw(area1.CreateExpand(gap+step, et, et, limit, limit), false, r[num2%2],     g[num2%2],     b[num2%2],    true);
            context2.Draw(area1.CreateExpand(gap,      et, et, limit, limit), false, r[(num2+1)%2], g[(num2+1)%2], b[(num2+1)%2], true);
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
    /* Excluded for speed 
    Contour tri = Contour(XY(50,90), XY(100,60), XY(40,20));
	Draw(100, tri, Contour(30,170,60,70), tri ^ Contour(30,170,60,70));
    Draw(1001, tri, tri.CreateSwapXYd());
    Draw(1002, tri, tri.CreateRotated(30));
    Draw(1003, Contour(XY(-1, 103), XY(25, 37), XY(56, 102)), tri.CreateRotated(30));
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

    Contour variable = boxhole + cooomplex2;
    DrawIsinside(1055, variable);

    
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
	custom.assign(v1);
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
    custom.assign(v2);

    x=220, y=610;
    const XY v3[] = {XY( 50, 200),
	                XY(x, y),
	                XY( 50, 300),
	                XY(x+30, y+40),
	                XY(100, 300),
	                XY(x, y),
                    XY( 90, 300)};
    custom2.assign(v3);
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
    Contour part = cooomplex2[1];
    Contour spart = cooomplex3[1];
    DrawExpand(135, EXPAND_MITER, DBL_MAX, part, false);

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

    variable.ClearHoles();

    const XY forexpbevel[] = {XY(100,100), XY(130, 100), XY(100, 80), XY(150, 80),
        XY(150,160), XY(100,160)};

    DrawExpand(121, EXPAND_MITER, DBL_MAX, Contour(forexpbevel), false);
    DrawExpand(122, EXPAND_ROUND, DBL_MAX, Contour(forexpbevel), false);
    DrawExpand(123, EXPAND_BEVEL, DBL_MAX, Contour(forexpbevel), false);

    Contour box = Contour(10, 110, 10, 110) - Contour(30, 40, 30, 80) - Contour(80, 90, 30, 80);
    DrawExpand(124, EXPAND_MITER, DBL_MAX, box, false, "box with miter");
    DrawExpand(125, EXPAND_ROUND, DBL_MAX, box, false, "box with round");
    DrawExpand(126, EXPAND_BEVEL, DBL_MAX, box, false, "box with bevel");
    DrawExpand(127, EXPAND_MITER, DBL_MAX, variable, false, "boxhole with miter");
    DrawExpand(128, EXPAND_ROUND, DBL_MAX, variable, false, "boxhole with round");
    DrawExpand(129, EXPAND_BEVEL, DBL_MAX, variable, false, "boxhole with bevel");
    DrawExpand(130, EXPAND_MITER, DBL_MAX, later, false, "later with miter");
    DrawExpand(131, EXPAND_BEVEL, DBL_MAX, later, false, "later with bevel");
    DrawExpand(132, EXPAND_ROUND, DBL_MAX, later, false, "later with round");
    DrawExpand(133, EXPAND_MITER, DBL_MAX, cooomplex, false, "complex with miter");
    DrawExpand(134, EXPAND_MITER, DBL_MAX, huhu, false, "huhu with miter");

    DrawExpand(135, EXPAND_MITER, DBL_MAX, part, false, "part with miter");
    DrawExpand(136, EXPAND_ROUND, DBL_MAX, part, false, "part with round");
    DrawExpand(137, EXPAND_BEVEL, DBL_MAX, part, true, "part with bevel");

    DrawExpand(138, EXPAND_MITER, DBL_MAX, spart, false, "spart with round");
    DrawExpand(139, EXPAND_ROUND, DBL_MAX, spart, false, "spart with round");
    DrawExpand(140, EXPAND_BEVEL, DBL_MAX, spart, false, "spart with round");

    DrawExpand(150, EXPAND_MITER, DBL_MAX, cooomplex3, false, "complex3 with miter");
    DrawExpand(151, EXPAND_BEVEL, DBL_MAX, cooomplex3, false, "complex3 with miter");
    DrawExpand(152, EXPAND_ROUND, DBL_MAX, cooomplex3, false, "complex3 with miter");
    DrawExpand(153, EXPAND_MITER, DBL_MAX, cooomplex2, false, "rounded complex3 with miter");
    DrawExpand(154, EXPAND_BEVEL, DBL_MAX, cooomplex2, false, "rounded complex3 with miter");
    DrawExpand(155, EXPAND_ROUND, DBL_MAX, cooomplex2, false, "rounded complex3 with miter");

    Contour form1 = Contour(0, 100, 0, 50) + Contour(XY(0, 25), 10, 25) - Contour(XY(100, 25), 10, 25);
    Contour form2 = Contour(0, 100, 0, 50) - Contour(XY(0, 25), 10, 25) + Contour(XY(100, 25), 10, 25);
    Contour form3 = Contour(0, 100, 0, 50) + Contour(XY(0, 25), 10, 35) - Contour(XY(100, 25), 10, 35);
    Contour form4 = Contour(0, 100, 0, 50) - Contour(XY(0, 25), 10, 35) + Contour(XY(100, 25), 10, 35);
    Contour form5 = Contour(0, 100, 0, 50) - Contour(XY(0, 15), 15) - Contour(XY(0, 40), 10);

    
    DrawExpand(160, EXPAND_MITER, DBL_MAX, form1, false, "pipe with miter");
    DrawExpand(161, EXPAND_MITER, DBL_MAX, form2, false, "reverse pipe with miter");
    DrawExpand(162, EXPAND_MITER, DBL_MAX, form3, false, "pipe with bigger circle with miter");
    DrawExpand(163, EXPAND_MITER, DBL_MAX, form4, false, "reverse pipe with bigger circle with miter");
    DrawExpand(164, EXPAND_MITER, DBL_MAX, form5, false, "two inverse circles with miter");
    DrawExpand(170, EXPAND_BEVEL, DBL_MAX, form1, false, "pipe with miter");
    DrawExpand(171, EXPAND_BEVEL, DBL_MAX, form2, false, "reverse pipe with miter");
    DrawExpand(172, EXPAND_BEVEL, DBL_MAX, form3, false, "pipe with bigger circle with miter");
    DrawExpand(173, EXPAND_BEVEL, DBL_MAX, form4, false, "reverse pipe with bigger circle with miter");
    DrawExpand(174, EXPAND_BEVEL, DBL_MAX, form5, false, "two inverse circles with miter");
    DrawExpand(180, EXPAND_ROUND, DBL_MAX, form1, false, "pipe with miter");
    DrawExpand(181, EXPAND_ROUND, DBL_MAX, form2, false, "reverse pipe with miter");
    DrawExpand(182, EXPAND_ROUND, DBL_MAX, form3, false, "pipe with bigger circle with miter");
    DrawExpand(183, EXPAND_ROUND, DBL_MAX, form4, false, "reverse pipe with bigger circle with miter");
    DrawExpand(184, EXPAND_ROUND, DBL_MAX, form5, false, "two inverse circles with miter");

    DrawExpand(185, EXPAND_MITER_BEVEL, DBL_MAX, form1, false, "pipe with miter");
    DrawExpand(186, EXPAND_MITER_BEVEL, DBL_MAX, form2, false, "reverse pipe with miter");
    DrawExpand(187, EXPAND_MITER_BEVEL, DBL_MAX, form3, false, "pipe with bigger circle with miter");
    DrawExpand(188, EXPAND_MITER_BEVEL, DBL_MAX, form4, false, "reverse pipe with bigger circle with miter");
    DrawExpand(189, EXPAND_MITER_BEVEL, DBL_MAX, form5, false, "two inverse circles with miter");

    DrawExpand(190, EXPAND_MITER_ROUND, DBL_MAX, form1, false, "pipe with miter");
    DrawExpand(191, EXPAND_MITER_ROUND, DBL_MAX, form2, false, "reverse pipe with miter");
    DrawExpand(192, EXPAND_MITER_ROUND, DBL_MAX, form3, false, "pipe with bigger circle with miter");
    DrawExpand(193, EXPAND_MITER_ROUND, DBL_MAX, form4, false, "reverse pipe with bigger circle with miter");
    DrawExpand(194, EXPAND_MITER_ROUND, DBL_MAX, form5, false, "two inverse circles with miter");

    DrawExpand(195, EXPAND_MITER_SQUARE, DBL_MAX, form1, false, "pipe with miter");
    DrawExpand(196, EXPAND_MITER_SQUARE, DBL_MAX, form2, false, "reverse pipe with miter");
    DrawExpand(197, EXPAND_MITER_SQUARE, DBL_MAX, form3, false, "pipe with bigger circle with miter");
    DrawExpand(198, EXPAND_MITER_SQUARE, DBL_MAX, form4, false, "reverse pipe with bigger circle with miter");
    DrawExpand(199, EXPAND_MITER_SQUARE, DBL_MAX, form5, false, "two inverse circles with miter");

    /* end of exlusion */

    Contour lohere1 = Contour(XY(25,25), 25) + Contour(XY(75,25), 25);
    Draw(2181, lohere1);
    lohere1 += Contour(XY(25,75), 25);
    Draw(2182, lohere1);
    lohere1 += Contour(XY(75,75), 25);
    Draw(2183, lohere1);


    Draw(2184, Contour(0,100, 0,100), lohere1, Contour(0,100, 0,100) + lohere1);
    Draw(2185, Contour(0,100, 0,100), lohere1, Contour(0,100, 0,100) - lohere1);

    Contour lohere2 = Contour(XY(100,100), 100) - Contour(XY(140,100), 120);

    Contour lohere3 = Contour(0,100, 0,100) - lohere1;
    Contour lohere4 = Contour(0,100, 0,100) +
                      Contour(XY(0,0), 50) - Contour(XY(50,0), 50) + 
                      Contour(XY(0,50), 50) - Contour(XY(50,50), 50);

    Contour lohere5 = lohere3[0];
    
    Draw(2190, lohere1);
    Draw(2191, Contour(XY(100,100), 100), Contour(XY(140,100), 120), lohere2);
    Draw(2192, lohere3);
    Draw(2193, lohere4);
    Draw(2194, lohere5);

    Contour lohere6 = lohere3[1];
    Draw(2195, lohere6);

    Contour a = lohere3.CreateExpand(4, EXPAND_BEVEL, EXPAND_BEVEL);
    Contour b = a; b.ClearHoles();
    Draw(2196, a, b);
    Draw (2197,b, b - Contour(10,40,10,40));
    
    DrawExpand(250, EXPAND_MITER, DBL_MAX,lohere1, false);
    DrawExpand(251, EXPAND_MITER, DBL_MAX,lohere2, false);
    DrawExpand(252, EXPAND_MITER, DBL_MAX,lohere3, false);
    DrawExpand(253, EXPAND_MITER, DBL_MAX,lohere4, false);
    DrawExpand(254, EXPAND_MITER, DBL_MAX,lohere5, false);
    DrawExpand(255, EXPAND_MITER, DBL_MAX,lohere6, false);

    DrawExpand(260, EXPAND_BEVEL, DBL_MAX,lohere1, false);
    DrawExpand(261, EXPAND_BEVEL, DBL_MAX,lohere2, false);
    DrawExpand(262, EXPAND_BEVEL, DBL_MAX,lohere3, false);
    DrawExpand(263, EXPAND_BEVEL, DBL_MAX,lohere4, false);
    DrawExpand(264, EXPAND_BEVEL, DBL_MAX,lohere5, false);
    DrawExpand(265, EXPAND_BEVEL, DBL_MAX,lohere6, false);

    DrawExpand(270, EXPAND_ROUND, DBL_MAX,lohere1, false);
    DrawExpand(271, EXPAND_ROUND, DBL_MAX,lohere2, false);
    DrawExpand(272, EXPAND_ROUND, DBL_MAX,lohere3, false);
    DrawExpand(273, EXPAND_ROUND, DBL_MAX,lohere4, false);
    DrawExpand(274, EXPAND_ROUND, DBL_MAX,lohere5, false);
    DrawExpand(275, EXPAND_ROUND, DBL_MAX,lohere6, false);

    DrawExpand(350, EXPAND_MITER, DBL_MAX,Contour(0,40, 0,100) + Contour(XY(60,50), 20), false);
    DrawExpand(351, EXPAND_MITER, DBL_MAX,Contour(0,40, 0,100) + Contour(XY(50,50), 20), false);
    DrawExpand(352, EXPAND_MITER, DBL_MAX,Contour(0,40, 0,100) + Contour(XY(50,50), 30,15, 130), false);

    Contour rombusz = Contour(40,80,40,80).CreateRotatedAround(XY(60,60),45);
    DrawExpand(353, EXPAND_MITER, DBL_MAX,Contour(0,40, 0,100) + rombusz, false);
    DrawExpand(354, EXPAND_MITER, DBL_MAX,rombusz +rombusz.CreateShifted(XY(40,0)), false);

    Contour concave = Contour(0,40,0,100) - Contour(0,20,30,60) + Contour(XY(20,45), 15);
    DrawExpand(355, EXPAND_MITER, DBL_MAX,concave, false);

    Contour triangle(XY(0,0), XY(100,10), XY(0,20));
    DrawExpand(356, EXPAND_MITER, DBL_MAX, triangle, false);
    DrawExpand(357, EXPAND_MITER, 1.2, triangle, false);
    DrawExpand(358, EXPAND_MITER, 2, triangle, false);
    DrawExpand(359, EXPAND_MITER, 1, triangle, false);
    DrawExpand(360, EXPAND_MITER, 0, triangle, false);


  
    
    //Test area and circumference
    Contour poly(60,200, 90,110); //30 degree rotated
    for (unsigned i = 0; i<10; i++) {
        const Contour block(0, 65+i*5, 0, 200);
        Contour res = poly - block;
        Contour rot = res.CreateRotatedAround(XY(100,100),29);
        char buff[200];
        sprintf(buff, "Normal Area: %g, Circumference: %g\nRotate Area: %g, Circumference: %g", res.GetArea(), res.GetCircumference(), rot.GetArea(), rot.GetCircumference());
        DrawIsinside(400+i, rot, 10, buff);
    }

    //End of exclusion for speed */

    Contour Circle(XY(100,100), 30, 30, 30); //30 degree rotated
    for (unsigned i = 2; i<10; i++) {
        const Contour block(0, 65+i*5, 0, 200);
        Contour res = Circle - block;
        char buff[200];
        sprintf(buff, "Area: %g, Circumference: %g", res.GetArea(), res.GetCircumference());
        DrawIsinside(500+i, res, 10, buff);
    }

    Contour ell(XY(100,100), 40, 10, 30); //30 degree rotated
    for (unsigned i = 0; i<10; i++) {
        const Contour block(0, 65+i*5, 0, 200);
        Contour res = ell - block;
        char buff[200];
        sprintf(buff, "Area: %g, Circumference: %g", res.GetArea(), res.GetCircumference());
        DrawIsinside(600+i, res, 10, buff);
    }



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

} //namespace