/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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
/** @file contour_test.cpp Defines contour_test().
 * @ingroup contour_files
 */
#include <string>
#include "contour.h"
#include "contour_bezier.h"

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
    void Draw(const Contour& area, bool shifted, double r, double g, double b, bool fill, double center = 5, int vertices = 0, const std::map<size_t, XY> &cps = std::map<size_t, XY>());
    void Draw1(const Contour& area, bool other=false) {Draw(area, false, other?1:0, other?0:1, 0, true);}
    void Draw2(const Contour& area) {Draw(area, true, 0,0,1, true);}
};

CairoContext::CairoContext(unsigned i, const Block &pl, const char *text, bool two, int sub) : x(two?pl.x.Spans()+10:0)
{
    const double expand_by = 20;
    const double size_by = 8;
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
        sprintf(fileName, "test%u_%03u.png", i, sub);
    else
        sprintf(fileName, "test%u.png", i);
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

void CairoContext::Draw(const Contour& area, bool shifted, double r, double g, double b, bool fill, 
                        double center, int vertices, const std::map<size_t, XY> &cps)
{
    if (!cr) return;
    if (shifted)
        cairo_translate(cr, x, 0);
    if (fill) {
        cairo_set_source_rgba(cr, r, g, b, 0.2);
        area.Fill(cr);
    }
    cairo_set_source_rgb(cr, r, g, b);
    area.Line2(cr);
    if (center) {
        const XY c = area.Centroid();
        cairo_arc(cr, c.x, c.y, center, 0, 2*M_PI);
        cairo_fill(cr);
    }
    cairo_set_font_size(cr, 1);
    if (vertices) {
        for (unsigned u = 0; u<area[0].size(); u++) {
            cairo_new_sub_path(cr);
            cairo_arc(cr, area[0][u].GetStart().x, area[0][u].GetStart().y, 1, 0, 2*M_PI);
            cairo_close_path(cr);
        }
        cairo_stroke(cr);
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
        if (vertices==2) {
            cairo_set_font_size(cr, 1);
            for (unsigned u = 0; u<area[0].size(); u++) {
                cairo_move_to(cr, area[0][u].GetStart().x+1, area[0][u].GetStart().y-1);
                char buff[100];
                sprintf(buff, "%u: %lg; %lg", u, area[0][u].GetStart().x, area[0][u].GetStart().y);
                cairo_show_text(cr, buff);
            }
        }
        for (auto &e: cps) {
            cairo_new_sub_path(cr);
            cairo_arc(cr, e.second.x, e.second.y, 1, 0, 2*M_PI);
            cairo_close_path(cr);
            cairo_stroke(cr);
            cairo_move_to(cr, e.second.x+1, e.second.y+1);
            char buff[100];
            sprintf(buff, "(%u): %lg; %lg", e.first, e.second.x, e.second.y);
            cairo_show_text(cr, buff);
        }
    }
    if (shifted)
        cairo_translate(cr, -x, 0);
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

void DrawExpand(unsigned i, EExpandType et, double limit, const Contour area1, unsigned method=0, const char *text=NULL)
{
    CairoContext *context=NULL;
    if (method == 0) 
        context = new CairoContext(i, area1.GetBoundingBox().CreateExpand(100), text, false);
    const unsigned NUM=3;
    const double r[NUM] = {1,0,0};
    const double g[NUM] = {0,1,0};
    const double b[NUM] = {0,0,1};
    unsigned num=0;
    const double step = 4;
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
        if (method == 0) {
            if (gap==0)
                context->Draw(a, false, 0, 0, 0, false);
            else
                context->Draw(a, false, r[num%NUM], g[num%NUM], b[num%NUM], false);
        }
        num++;
        gap-=step;
    }
    if (method == 0) 
        delete context;
    else {
        unsigned num2=0;
        for (gap = -max_gap; gap<=max_gap; gap+=step) {
            char buff[4000];
            sprintf(buff, "Inner expanded by %g %s", gap, text ? text : "");
            CairoContext context2(i, area1.GetBoundingBox().CreateExpand(40), buff, false, int(num2));
            context2.Draw(area1.CreateExpand(gap+step, et, et, limit, limit), false, r[(num2+1)%2], g[(num2+1)%2], b[(num2+1)%2], true, 0);
            context2.Draw(area1.CreateExpand(gap, et, et, limit, limit), false, r[num2%2], g[num2%2], b[num2%2], true, 0);
            if (method==2) {
                expand_debug = 1;
                cairo_set_line_width(context2.cr, 0.5);
                context2.Draw(area1.CreateExpand(gap, et, et, limit, limit), false, 0, 0, 0, false, 0, 2, expand_debug_cps);
                expand_debug = 0;
                cairo_set_line_width(context2.cr, 2);
            }
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

void DrawRelation(unsigned i, const Contour &c1, const Contour &c2, const char *text="")
{
    static const char relnames[][20] = {"OVERLAP", "A_IS_EMPTY", "B_IS_EMPTY", "BOTH_EMPTY", 
                                        "A_INSIDE_B", "B_INSIDE_A", "SAME", "APART", 
                                        "A_IN_HOLE_OF_B", "B_IN_HOLE_OF_A", "IN_HOLE_APART"};
    Block b = c1.GetBoundingBox();
    b += c2.GetBoundingBox();
    b += XY(100,100);
    std::string s = std::string("'A' is red: ") + relnames[(unsigned)c1.RelationTo(c2, false)] + ", should be: " + text;
    CairoContext c(i, b, s.c_str(), false);
    c.Draw(c1, false, 1, 0, 0, true, 0);
    c.Draw(c2, false, 0, 1, 0, true, 0);
};

void DrawDistance(unsigned i, const Contour &c1, const Contour &c2, const char *text="")
{
    Block b = c1.GetBoundingBox();
    b += c2.GetBoundingBox();
    const DistanceType dist = c1.Distance(c2);
    b += XY(100,100);
    if (dist.IsValid()) 
        b += XY(fabs(dist.Distance())+20, fabs(dist.Distance()));
    char uhh[1000];
    const bool bad = !dist.IsZero() && !test_equal(fabs(dist.OnMe().Distance(dist.OnOther())), fabs(dist.Distance()));
    sprintf(uhh, "Distance is %g. %s %s", dist.Distance(), bad ? "BAAAAD" : "", text);
    CairoContext c(i, b, uhh, false);
    if (dist.IsValid() && dist.Distance()>=0) {
        c.Draw(c1.CreateExpand(dist.Distance(), contour::EXPAND_ROUND), false, 1, 0.9, 0.9, true);
        c.Draw(c2.CreateExpand(dist.Distance(), contour::EXPAND_ROUND), false, 0.9, 1, 0.9, true);
    }
    c.Draw(c1, false, 1, 0, 0, true, 0);
    c.Draw(c2, false, 0, 1, 0, true, 0);
    if (dist.IsValid() && dist.Distance()<0) {
        c.Draw(c1.CreateExpand(dist.Distance(), contour::EXPAND_ROUND), false, 1, 0.9, 0.9, true);
        c.Draw(c2.CreateExpand(dist.Distance(), contour::EXPAND_ROUND), false, 0.9, 1, 0.9, true);
    }
    if (dist.IsValid()) {
        cairo_set_source_rgb(c.cr, 0,0,0);
        cairo_move_to(c.cr, 10, 20);
        cairo_line_to(c.cr, 10, 30);
        cairo_new_sub_path(c.cr);
        cairo_move_to(c.cr, fabs(dist.Distance())+10, 20);
        cairo_line_to(c.cr, fabs(dist.Distance())+10, 30);
        cairo_new_sub_path(c.cr);
        cairo_move_to(c.cr, 10, 25);
        cairo_line_to(c.cr, fabs(dist.Distance())+10, 25);
        cairo_set_line_width(c.cr, 2);
    }
    if (dist.OnMe().test_equal(dist.OnOther())) {
        cairo_arc(c.cr, dist.OnMe().x, dist.OnMe().y, 5, 0, 2*M_PI);
        cairo_fill(c.cr);
    } else if (!dist.IsZero()) {
        cairo_move_to(c.cr, dist.OnMe().x, dist.OnMe().y);
        cairo_line_to(c.cr, dist.OnOther().x, dist.OnOther().y);
        cairo_stroke(c.cr);
    }
};

void DrawCut(unsigned i, const Contour &c1, unsigned num, const XY s[], const XY e[])
{
    Block b = c1.GetBoundingBox();
    b.Expand(10);
    const XY origo = b.UpperLeft();
    b += XY(100,100);
    b += XY(0,0);
    CairoContext c(i, b, "", false);
    c.Draw(c1, false, 1, 0, 0, true, 0);
    for (unsigned u=0; u<num; u++) {
        const Range r = c1.Cut(s[u]+origo, e[u]+origo);
        XY start = origo + s[u], end = origo + e[u];
        const XY from = origo + s[u] + (e[u]-s[u])*r.from;
        const XY till = origo + s[u] + (e[u]-s[u])*r.till;
        if (r.from<0) start = from;
        if (r.till>1) end = till;
        cairo_move_to(c.cr, start.x, start.y);
        cairo_line_to(c.cr, end.x, end.y);
        cairo_set_source_rgb(c.cr, 0.5, 1, 0.5);
        cairo_stroke(c.cr);
        cairo_set_source_rgb(c.cr, 0, 1, 0);
        cairo_arc(c.cr, s[u].x + origo.x, s[u].y + origo.y, 3, 0, 2*M_PI);
        cairo_fill(c.cr);
        cairo_arc(c.cr, e[u].x + origo.x, e[u].y + origo.y, 3, 0, 2*M_PI);
        cairo_fill(c.cr);
        if (r.IsInvalid()) continue;
        cairo_set_source_rgb(c.cr, 0, 0, 1);
        cairo_arc(c.cr, from.x, from.y, 3, 0, 2*M_PI);
        cairo_fill(c.cr);
        cairo_arc(c.cr, till.x, till.y, 3, 0, 2*M_PI);
        cairo_fill(c.cr);
    }
}

void DrawExpand2D(unsigned i, const Contour &c1, const XY &gap, const char *text="")
{
    const Contour c2 = c1.CreateExpand2D(gap);
    Block b = c1.GetBoundingBox();
    b += c2.GetBoundingBox();
    b += XY(100,100);
    CairoContext c(i, b, text, false);
    c.Draw(c1, false, 1, 0, 0, true, 0);
    c.Draw(c2, false, 0.5, 1, 0.5, true, 0);
    cairo_set_source_rgb(c.cr, 0, 0, 0);
    cairo_rectangle(c.cr, c1[0][0].GetStart().x, c1[0][0].GetStart().y, gap.x, gap.y);
    cairo_stroke(c.cr);
    const unsigned edge = 3;
    if (c1[0].size()>edge) 
        cairo_rectangle(c.cr, c1[0][edge].GetStart().x, c1[0][edge].GetStart().y, gap.x, -gap.y);
    cairo_stroke(c.cr);
};

void DrawTangent(unsigned i, const Contour &c1, const Contour &c2, const char *text="")
{
    Block b = c1.GetBoundingBox();
    b += c2.GetBoundingBox();
    XY C[2], CC[2];
    bool was = c1.TangentFrom(c2, C, CC);
    CairoContext c(i, b, text, false);
    c.Draw(c1, false, 1, 0, 0, true, 0);
    c.Draw(c2, false, 0, 1, 0, true, 0);
    if (was) {
        cairo_set_source_rgb(c.cr, 0,0,0);
        cairo_move_to(c.cr, C[0].x, C[0].y);
        cairo_line_to(c.cr, C[1].x, C[1].y);
        cairo_stroke(c.cr);
        cairo_arc(c.cr, C[0].x, C[0].y, 5, 0, 2*M_PI);
        cairo_fill(c.cr);
        cairo_arc(c.cr, C[1].x, C[1].y, 5, 0, 2*M_PI);
        cairo_fill(c.cr);
        cairo_move_to(c.cr, CC[0].x, CC[0].y);
        cairo_line_to(c.cr, CC[1].x, CC[1].y);
        cairo_stroke(c.cr);
        cairo_arc(c.cr, CC[0].x, CC[0].y, 5, 0, 2*M_PI);
        cairo_stroke(c.cr);
        cairo_arc(c.cr, CC[1].x, CC[1].y, 5, 0, 2*M_PI);
        cairo_stroke(c.cr);
    }
};



namespace generated_forms {
    Contour tri, boxhole, cooomplex, cooomplex2, cooomplex3;
    Contour variable, variable_clear, custom, later, raster;
    Contour circle, circle2, circle3, huhu, boxhole2;
    Contour part, spart, partxy, spartxy, forexpbevel;
    Contour lohere1, lohere2, lohere3, lohere4, lohere5, lohere6;
    Contour rombusz, concave, triangle, box_circle;
    bool done = false;
    Contour ChoppedBox;
    Contour form1, form2, form3, form4, form5;
} //namespace local

void generate_forms() 
{
    using namespace generated_forms;
    if (done) return;
    done = true;
    tri = Contour(XY(50,90), XY(100,60), XY(40,20));
    tri +=  Contour(30,70,60,70);

	boxhole = Contour(130,170,60,70);
	boxhole += Contour(160,170,60,140);
	boxhole += Contour(130,140,60,140);
	boxhole += Contour(130,170,130,140);
    boxhole += Contour(148,153, 85, 115);

    cooomplex=boxhole+tri;

	cooomplex2 = Contour(110, 200, 80, 120);
    cooomplex2 -= Contour(120, 190, 90, 110);

    variable = boxhole + cooomplex2;
	cooomplex2 += cooomplex;
    later = cooomplex2;

	custom = cooomplex2;
    cooomplex2 += cooomplex2.CreateShifted(XY(15,15));

    cooomplex2.Shift(XY(200,0));
	custom. Shift(XY(200,0));
    
	const int num_x = 10;
	const int num_y = 10;
	for (int i=0; i<num_x; i++)
		raster += Contour(200+i*20, 215+i*20, 200, 190+num_y*20);
	for (int j=0; j<num_y; j++)
		raster += Contour(200, 190+num_x*20, 200+j*20, 215+j*20);

    cooomplex2.ClearHoles();

    cooomplex2 *= Contour(XY(300,101), 100, 50);

    circle = Contour(XY(200, 200), 60, 30, 30);
	circle += Contour(200,300, 170,190);

    double x=220, y=610;
	circle2= Contour(XY(x, y), 60, 30, abs(x-y));
    circle3 = circle2;
    circle2 += Contour(x,x+100, y+15,y+30);

	boxhole2 = Contour(110, 200, 80, 120);
	boxhole2 -= Contour(120, 190, 90, 110);
	huhu = boxhole2;
	huhu.ClearHoles();
	huhu *= Contour(XY(130,101), 30,20);

    cooomplex3 = cooomplex2;
	cooomplex2 = cooomplex3;
	cooomplex2.RotateAround(XY(350,100), 39);
    part = cooomplex2[1];
    spart = cooomplex3[1];
    partxy = part.CreateSwapXYd();
    partxy.IsSane();
    spartxy = spart.CreateSwapXYd();
    spartxy.IsSane();

    variable.ClearHoles();

    const XY forexpbevel_points[] = {XY(100,100), XY(130, 100), XY(100, 80), XY(150, 80),
        XY(150,160), XY(100,160)};
    forexpbevel.assign(forexpbevel_points);


    lohere1 = Contour(XY(25,25), 25) + Contour(XY(75,25), 25);
    lohere1 += Contour(XY(25,75), 25);
    lohere1 += Contour(XY(75,75), 25);
    lohere2 = Contour(XY(100,100), 100) - Contour(XY(140,100), 120);
    lohere3 = Contour(0,100, 0,100) - lohere1;
    lohere4 = Contour(0,100, 0,100) +
                      Contour(XY(0,0), 50) - Contour(XY(50,0), 50) + 
                      Contour(XY(0,50), 50) - Contour(XY(50,50), 50);
    lohere5 = lohere3[0];
    lohere6 = lohere3[1];

    rombusz = Contour(40,80,40,80).CreateRotatedAround(XY(60,60),45);
    concave = Contour(0,40,0,100) - Contour(0,20,30,60) + Contour(XY(20,45), 15);
    triangle = Contour(XY(0,0), XY(100,10), XY(0,20));

    ChoppedBox = Contour(100, 200, 100, 200) - Contour(100, 110, 100, 110);
    variable_clear = variable;
    variable_clear.ClearHoles();
    box_circle = Contour(10, 110, 10, 110) - Contour(30, 40, 30, 80) - Contour(80, 90, 30, 80);


    form1 = Contour(0, 100, 0, 50) + Contour(XY(0, 25), 10, 25) - Contour(XY(100, 25), 10, 25);
    form2 = Contour(0, 100, 0, 50) - Contour(XY(0, 25), 10, 25) + Contour(XY(100, 25), 10, 25);
    form3 = Contour(0, 100, 0, 50) + Contour(XY(0, 25), 10, 35) - Contour(XY(100, 25), 10, 35);
    form4 = Contour(0, 100, 0, 50) - Contour(XY(0, 25), 10, 35) + Contour(XY(100, 25), 10, 35);
    form5 = Contour(0, 100, 0, 50) - Contour(XY(0, 15), 15) -Contour(XY(0, 40), 10);

}


void contour_test_basic(void)
{
    Contour tri = Contour(XY(50, 90), XY(100, 60), XY(40, 20));
    Draw(100, tri, Contour(30, 170, 60, 70), tri ^ Contour(30, 170, 60, 70));
    Draw(1001, tri, tri.CreateSwapXYd());
    Draw(1002, tri, tri.CreateRotated(30));
    Draw(1003, Contour(XY(-1, 103), XY(25, 37), XY(56, 102)), tri.CreateRotated(30));
    tri += Contour(30, 70, 60, 70);

    Draw(101, tri, tri.CreateShifted(XY(15, 15)), tri ^ tri.CreateShifted(XY(15, 15)));

    Contour boxhole = Contour(130, 170, 60, 70);
    boxhole += Contour(160, 170, 60, 140);
    Draw(1020, boxhole);
    boxhole += Contour(130, 140, 60, 140);
    Draw(1021, boxhole);
    boxhole += Contour(130, 170, 130, 140);
    Draw(1022, boxhole, Contour(148, 153, 85, 115), boxhole + Contour(148, 153, 85, 115));
    boxhole += Contour(148, 153, 85, 115);

    Contour cooomplex;
    Draw(103, boxhole, tri, boxhole + tri);
    cooomplex = boxhole+tri;

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
    Draw(107, cooomplex2, cooomplex2.CreateShifted(XY(15, 15)), cooomplex2 ^ cooomplex2.CreateShifted(XY(15, 15)));
    cooomplex2 += cooomplex2.CreateShifted(XY(15, 15));

    cooomplex2.Shift(XY(200, 0));
    custom.Shift(XY(200, 0));

    cooomplex.clear();
    const int num_x = 10;
    const int num_y = 10;
    for (int i = 0; i<num_x; i++)
        cooomplex += Contour(200+i*20, 215+i*20, 200, 190+num_y*20);
    for (int j = 0; j<num_y; j++)
        cooomplex += Contour(200, 190+num_x*20, 200+j*20, 215+j*20);
    Draw(108, cooomplex);

    cooomplex2.ClearHoles();


    Draw(1091, Contour(100, 300, 60, 70), Contour(XY(300, 100), 100, 50), Contour(100, 300, 60, 70) + Contour(XY(300, 100), 100, 50));
    Draw(1092, Contour(100, 300, 60, 70), Contour(XY(300, 100), 100, 50), Contour(100, 300, 60, 70) * Contour(XY(300, 100), 100, 50));
    Draw(1093, Contour(100, 300, 60, 70), Contour(XY(300, 100), 100, 50), Contour(100, 300, 60, 70) ^ Contour(XY(300, 100), 100, 50));
    Draw(1094, Contour(100, 300, 60, 70), Contour(XY(300, 100), 100, 50), Contour(100, 300, 60, 70) - Contour(XY(300, 100), 100, 50));

    Draw(109, cooomplex2, Contour(XY(300, 101), 100, 50), cooomplex2 ^ Contour(XY(300, 101), 100, 50));
    DrawIsinside(1091, cooomplex2 ^ Contour(XY(300, 101), 100, 50), 5);

    Draw(110, tri, tri.CreateSwapXYd());
    Draw(1101, boxhole, boxhole.CreateSwapXYd());
    Draw(1102, cooomplex, cooomplex.CreateSwapXYd());
    Draw(1103, Contour(XY(300, 101), 100, 50), Contour(XY(300, 101), 100, 50).CreateSwapXYd());
    Draw(1104, cooomplex2 ^ Contour(XY(300, 101), 100, 50), (cooomplex2 ^ Contour(XY(300, 101), 100, 50)).CreateSwapXYd());

    cooomplex2 *= Contour(XY(300, 101), 100, 50);

    Contour circle = Contour(XY(200, 200), 60, 30, 30);
    Draw(113, circle, Contour(200, 300, 170, 190), circle + Contour(200, 300, 170, 190));
    circle += Contour(200, 300, 170, 190);

    double x = 220, y = 610;
    Contour circle2 = Contour(XY(x, y), 60, 30, abs(x-y));
    Contour circle3 = circle2;
    Draw(114, circle2, Contour(x, x+100, y+15, y+30), circle2 + Contour(x, x+100, y+15, y+30));
    circle2 += Contour(x, x+100, y+15, y+30);

    Contour boxhole2 = Contour(110, 200, 80, 120);
    boxhole2 -= Contour(120, 190, 90, 110);
    Contour huhu = boxhole2;
    huhu.ClearHoles();
    Draw(115, huhu, Contour(XY(130, 101), 30, 20), huhu + Contour(XY(130, 101), 30, 20));

    Draw(116, huhu, Contour(XY(130, 101), 30, 20), huhu * Contour(XY(130, 101), 30, 20));
    huhu *= Contour(XY(130, 101), 30, 20);
    DrawIsinside(1161, huhu, 2);
    DrawIsinside(1162, huhu.CreateSwapXYd(), 2);

    DrawIsinside(1163, (boxhole2 - Contour(XY(130, 101), 30, 20)).CreateSwapXYd(), 2);

    Contour cooomplex3 = cooomplex2;
    cooomplex2 = cooomplex3;
    cooomplex2.RotateAround(XY(350, 100), 39);
    Contour part = cooomplex2[1];
    Contour spart = cooomplex3[1];

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
}


void contour_test_expand(void)
{
    using namespace generated_forms;
    generate_forms();

    DrawExpand(120, EXPAND_MITER, CONTOUR_INFINITY, ChoppedBox, 0);

    DrawExpand(121, EXPAND_MITER, CONTOUR_INFINITY, forexpbevel, 0);
    DrawExpand(122, EXPAND_ROUND, CONTOUR_INFINITY, forexpbevel, 0);
    DrawExpand(123, EXPAND_BEVEL, CONTOUR_INFINITY, forexpbevel, 0);

    DrawExpand(124, EXPAND_MITER, CONTOUR_INFINITY, box_circle, 0, "box with miter");
    DrawExpand(125, EXPAND_ROUND, CONTOUR_INFINITY, box_circle, 0, "box with round");
    DrawExpand(126, EXPAND_BEVEL, CONTOUR_INFINITY, box_circle, 0, "box with bevel");
    DrawExpand(127, EXPAND_MITER, CONTOUR_INFINITY, variable_clear, 0, "boxhole with miter");
    DrawExpand(128, EXPAND_ROUND, CONTOUR_INFINITY, variable_clear, 0, "boxhole with round");
    DrawExpand(129, EXPAND_BEVEL, CONTOUR_INFINITY, variable_clear, 0, "boxhole with bevel");

    DrawExpand(130, EXPAND_MITER, CONTOUR_INFINITY, later, 0, "later with miter");
    DrawExpand(131, EXPAND_BEVEL, CONTOUR_INFINITY, later, 0, "later with bevel");
    DrawExpand(132, EXPAND_ROUND, CONTOUR_INFINITY, later, 0, "later with round");
    DrawExpand(133, EXPAND_MITER, CONTOUR_INFINITY, cooomplex, 0, "complex with miter");
    DrawExpand(134, EXPAND_MITER, CONTOUR_INFINITY, huhu, 0, "huhu with miter");

    DrawExpand(135, EXPAND_MITER, CONTOUR_INFINITY, part, 0, "part with miter");
    DrawExpand(136, EXPAND_ROUND, CONTOUR_INFINITY, part, 0, "part with round");
    DrawExpand(137, EXPAND_BEVEL, CONTOUR_INFINITY, part, 0, "part with bevel");

    DrawExpand(138, EXPAND_MITER, CONTOUR_INFINITY, spart, 0, "spart with round");
    DrawExpand(139, EXPAND_ROUND, CONTOUR_INFINITY, spart, 0, "spart with round");
    DrawExpand(140, EXPAND_BEVEL, CONTOUR_INFINITY, spart, 0, "spart with round");

    DrawExpand(150, EXPAND_MITER, CONTOUR_INFINITY, cooomplex3, 0, "complex3 with miter");
    DrawExpand(151, EXPAND_BEVEL, CONTOUR_INFINITY, cooomplex3, 0, "complex3 with miter");
    DrawExpand(152, EXPAND_ROUND, CONTOUR_INFINITY, cooomplex3, 0, "complex3 with miter");
    DrawExpand(153, EXPAND_MITER, CONTOUR_INFINITY, cooomplex2, 0, "rounded complex3 with miter");
    DrawExpand(154, EXPAND_BEVEL, CONTOUR_INFINITY, cooomplex2, 0, "rounded complex3 with miter");
    DrawExpand(155, EXPAND_ROUND, CONTOUR_INFINITY, cooomplex2, 0, "rounded complex3 with miter");
    
    DrawExpand(160, EXPAND_MITER, CONTOUR_INFINITY, form1, 0, "pipe with miter");
    DrawExpand(161, EXPAND_MITER, CONTOUR_INFINITY, form2, 0, "reverse pipe with miter");
    DrawExpand(162, EXPAND_MITER, CONTOUR_INFINITY, form3, 0, "pipe with bigger circle with miter");
    DrawExpand(163, EXPAND_MITER, CONTOUR_INFINITY, form4, 0, "reverse pipe with bigger circle with miter");
    DrawExpand(164, EXPAND_MITER, CONTOUR_INFINITY, form5, 0, "two inverse circles with miter");
    DrawExpand(170, EXPAND_BEVEL, CONTOUR_INFINITY, form1, 0, "pipe with miter");
    DrawExpand(171, EXPAND_BEVEL, CONTOUR_INFINITY, form2, 0, "reverse pipe with miter");
    DrawExpand(172, EXPAND_BEVEL, CONTOUR_INFINITY, form3, 0, "pipe with bigger circle with miter");
    DrawExpand(173, EXPAND_BEVEL, CONTOUR_INFINITY, form4, 0, "reverse pipe with bigger circle with miter");
    DrawExpand(174, EXPAND_BEVEL, CONTOUR_INFINITY, form5, 0, "two inverse circles with miter");
    DrawExpand(180, EXPAND_ROUND, CONTOUR_INFINITY, form1, 0, "pipe with miter");
    DrawExpand(181, EXPAND_ROUND, CONTOUR_INFINITY, form2, 0, "reverse pipe with miter");
    DrawExpand(182, EXPAND_ROUND, CONTOUR_INFINITY, form3, 0, "pipe with bigger circle with miter");
    DrawExpand(183, EXPAND_ROUND, CONTOUR_INFINITY, form4, 0, "reverse pipe with bigger circle with miter");
    DrawExpand(184, EXPAND_ROUND, CONTOUR_INFINITY, form5, 0, "two inverse circles with miter");

    DrawExpand(185, EXPAND_MITER_BEVEL, CONTOUR_INFINITY, form1, 0, "pipe with miter");
    DrawExpand(186, EXPAND_MITER_BEVEL, CONTOUR_INFINITY, form2, 0, "reverse pipe with miter");
    DrawExpand(187, EXPAND_MITER_BEVEL, CONTOUR_INFINITY, form3, 0, "pipe with bigger circle with miter");
    DrawExpand(188, EXPAND_MITER_BEVEL, CONTOUR_INFINITY, form4, 0, "reverse pipe with bigger circle with miter");
    DrawExpand(189, EXPAND_MITER_BEVEL, CONTOUR_INFINITY, form5, 0, "two inverse circles with miter");

    DrawExpand(190, EXPAND_MITER_ROUND, CONTOUR_INFINITY, form1, 0, "pipe with miter");
    DrawExpand(191, EXPAND_MITER_ROUND, CONTOUR_INFINITY, form2, 0, "reverse pipe with miter");
    DrawExpand(192, EXPAND_MITER_ROUND, CONTOUR_INFINITY, form3, 0, "pipe with bigger circle with miter");
    DrawExpand(193, EXPAND_MITER_ROUND, CONTOUR_INFINITY, form4, 0, "reverse pipe with bigger circle with miter");
    DrawExpand(194, EXPAND_MITER_ROUND, CONTOUR_INFINITY, form5, 0, "two inverse circles with miter");

    DrawExpand(195, EXPAND_MITER_SQUARE, CONTOUR_INFINITY, form1, 0, "pipe with miter");
    DrawExpand(196, EXPAND_MITER_SQUARE, CONTOUR_INFINITY, form2, 0, "reverse pipe with miter");
    DrawExpand(197, EXPAND_MITER_SQUARE, CONTOUR_INFINITY, form3, 0, "pipe with bigger circle with miter");
    DrawExpand(198, EXPAND_MITER_SQUARE, CONTOUR_INFINITY, form4, 0, "reverse pipe with bigger circle with miter");
    DrawExpand(199, EXPAND_MITER_SQUARE, CONTOUR_INFINITY, form5, 0, "two inverse circles with miter");
};

void contour_test_lohere(void) 
{
    generate_forms();
    Draw(2181, Contour(XY(25, 25), 25) + Contour(XY(75, 25), 25));
    Draw(2182, Contour(XY(25,25), 25) + Contour(XY(75,25), 25) + Contour(XY(25,75), 25));
    Draw(2183, generated_forms::lohere1);

    Draw(2184, Contour(0,100, 0,100), generated_forms::lohere1, Contour(0,100, 0,100) + generated_forms::lohere1);
    Draw(2185, Contour(0,100, 0,100), generated_forms::lohere1, Contour(0,100, 0,100) - generated_forms::lohere1);

    Draw(2190, generated_forms::lohere1);
    Draw(2191, Contour(XY(100,100), 100), Contour(XY(140,100), 120), generated_forms::lohere2);
    Draw(2192, generated_forms::lohere3);
    Draw(2193, generated_forms::lohere4);
    Draw(2194, generated_forms::lohere5);

    Draw(2195, generated_forms::lohere6);

    Contour a = generated_forms::lohere3.CreateExpand(4, EXPAND_BEVEL, EXPAND_BEVEL);
    Contour b = a; b.ClearHoles();
    Draw(2196, a, b);
    Draw (2197,b, b - Contour(10,40,10,40));
    
    DrawExpand(250, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::lohere1, 0);
    DrawExpand(251, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::lohere2, 0);
    DrawExpand(252, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::lohere3, 0);
    DrawExpand(253, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::lohere4, 0);
    DrawExpand(254, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::lohere5, 0);
    DrawExpand(255, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::lohere6, 0);

    DrawExpand(260, EXPAND_BEVEL, CONTOUR_INFINITY, generated_forms::lohere1, 0);
    DrawExpand(261, EXPAND_BEVEL, CONTOUR_INFINITY, generated_forms::lohere2, 0);
    DrawExpand(262, EXPAND_BEVEL, CONTOUR_INFINITY, generated_forms::lohere3, 0);
    DrawExpand(263, EXPAND_BEVEL, CONTOUR_INFINITY, generated_forms::lohere4, 0);
    DrawExpand(264, EXPAND_BEVEL, CONTOUR_INFINITY, generated_forms::lohere5, 0);
    DrawExpand(265, EXPAND_BEVEL, CONTOUR_INFINITY, generated_forms::lohere6, 0);

    DrawExpand(270, EXPAND_ROUND, CONTOUR_INFINITY, generated_forms::lohere1, 0);
    DrawExpand(271, EXPAND_ROUND, CONTOUR_INFINITY, generated_forms::lohere2, 0);
    DrawExpand(272, EXPAND_ROUND, CONTOUR_INFINITY, generated_forms::lohere3, 0);
    DrawExpand(273, EXPAND_ROUND, CONTOUR_INFINITY, generated_forms::lohere4, 0);
    DrawExpand(274, EXPAND_ROUND, CONTOUR_INFINITY, generated_forms::lohere5, 0);
    DrawExpand(275, EXPAND_ROUND, CONTOUR_INFINITY, generated_forms::lohere6, 0);

    DrawExpand(350, EXPAND_MITER, CONTOUR_INFINITY,Contour(0,40, 0,100) + Contour(XY(60,50), 20), 0);
    DrawExpand(351, EXPAND_MITER, CONTOUR_INFINITY,Contour(0,40, 0,100) + Contour(XY(50,50), 20), 0);
    DrawExpand(352, EXPAND_MITER, CONTOUR_INFINITY,Contour(0,40, 0,100) + Contour(XY(50,50), 30,15, 130), 0);

    DrawExpand(353, EXPAND_MITER, CONTOUR_INFINITY,Contour(0,40, 0,100) + generated_forms::rombusz, 0);
    DrawExpand(354, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::rombusz + generated_forms::rombusz.CreateShifted(XY(40,0)), 0);

    DrawExpand(355, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::concave, 0);

    DrawExpand(356, EXPAND_MITER, CONTOUR_INFINITY, generated_forms::triangle, 0);
    DrawExpand(357, EXPAND_MITER, 1.2, generated_forms::triangle, 0);
    DrawExpand(358, EXPAND_MITER, 2, generated_forms::triangle, 0);
    DrawExpand(359, EXPAND_MITER, 1, generated_forms::triangle, 0);
    DrawExpand(360, EXPAND_MITER, 0, generated_forms::triangle, 0);
};

void contour_test_assign(unsigned num)
{
    Contour custom;
    double x=150, y=250;
    const XY v1[] = {XY( 50, 200),
	                XY(x, y),
	                XY( 50, 300),
	                XY(x+30, y+40),
	                XY(100, 300),
	                XY(x, y),
                    XY( 90, 300)};
	custom.assign(v1);
    Draw(num, custom);

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
    Draw(num+1, custom, custom2);
};


void contour_test_area(unsigned num)
{
    //Test area and circumference
    Contour poly(60,200, 90,110); //30 degree rotated
    for (unsigned i = 0; i<10; i++) {
        const Contour block(0, 65+i*5, 0, 200);
        Contour res = poly - block;
        Contour rot = res.CreateRotatedAround(XY(100,100),29);
        char buff[200];
        sprintf(buff, "Normal Area: %g, Circumference: %g\nRotate Area: %g, Circumference: %g", res.GetArea(), res.GetCircumference(), rot.GetArea(), rot.GetCircumference());
        DrawIsinside(num+i, rot, 10, buff);
    }

    //End of exclusion for speed */

    Contour Circle(XY(100,100), 30, 30, 30); //30 degree rotated
    for (unsigned i = 2; i<10; i++) {
        const Contour block(0, 65+i*5, 0, 200);
        Contour res = Circle - block;
        char buff[200];
        sprintf(buff, "Area: %g, Circumference: %g", res.GetArea(), res.GetCircumference());
        DrawIsinside(num+100+i, res, 10, buff);
    }

    Contour ell(XY(100,100), 40, 10, 30); //30 degree rotated
    for (unsigned i = 0; i<10; i++) {
        const Contour block(0, 65+i*5, 0, 200);
        Contour res = ell - block;
        char buff[200];
        sprintf(buff, "Area: %g, Circumference: %g", res.GetArea(), res.GetCircumference());
        DrawIsinside(num+200+i, res, 10, buff);
    }
};



void contour_test_relations(unsigned num)
{
    Contour c1[10], c2[10], c3[10];
    c1[0] = Block(22,28, 22,28); //tiny
    c1[1] = Block(20,30, 20,30); //small
    c1[2] = Block(10,40, 10,40); //medium
    c1[3] = Block(0,50, 0,50);   //large
    c1[4] = c1[3] - c1[2];       //large with hole
    c1[5] = c1[4] + c1[1];       //small in large with hole 

    for (unsigned u=0; u<10; u++) {
        c2[u] = c1[u].CreateShifted(XY(60,0));
        c3[u] = c1[u].CreateShifted(XY(120,0));
    }
    
    DrawRelation(num+00, c1[3], c1[9], "B_EMPTY");
    DrawRelation(num+01, c1[9], c1[1], "A_EMPTY");
    DrawRelation(num+02, c1[9], c1[9], "BOTH_EMPTY");

    DrawRelation(num+03, c1[3], c1[2], "B_IN_A");
    DrawRelation(num+04, c1[0], c2[1], "APART");

    DrawRelation(num+05, c1[3], c1[2], "B_IN_A");
    DrawRelation(num+06, c1[0], c1[2], "A_IN_B");
    DrawRelation(num+07, c1[1], c1[1], "SAME");
    DrawRelation(num+ 8, c1[0], c2[1], "APART");
    DrawRelation(num+ 9, c1[3], c1[2].CreateShifted(XY(20,0)), "OVERLAP");

    DrawRelation(num+10, c1[4], c1[1], "B_IN_HOLE_A");
    DrawRelation(num+11, c1[1], c1[4], "A_IN_HOLE_B");

    DrawRelation(num+20, c1[1]+c2[1], c1[4], "HOLE_MIXED");
};

void contour_test_distance(unsigned num)
{
    Contour c1(30, 100, 30, 100);
    Contour c2(130, 150, 140, 200);
    DrawDistance(num++, c1, c2, "");

    Contour c3(XY(120, 200), 40, 60, 20);
    DrawDistance(num++, c1, c3);

    c3.Shift(XY(30,0));
    DrawDistance(num++, c1, c3);

    c3.Shift(XY(70,0));
    DrawDistance(num++, c1, c3);
    DrawDistance(num++, c1, c3 * Block(180, 400, 0, 500));
    DrawDistance(num++, c1, c3 * Block(200, 400, 0, 500));
    DrawDistance(num++, c1, c3 * Block(2300, 400, 0, 500));

    DrawDistance(num++, c3.CreateRotated(30), c3);
    DrawDistance(num++, c3 * Block(180, 400, 0, 500), (c3 * Block(180, 400, 0, 500)).Rotate(30));
    DrawDistance(num++, c3 * Block(200, 400, 0, 500), (c3 * Block(200, 400, 0, 500)).Rotate(30));

    Contour c4 = c1 - Block(40, 90, 40, 90);
    DrawDistance(num++, c4, Contour(XY(60,60), 40, 20));
    DrawDistance(num++, c4, Contour(XY(60,60), 30, 20));
    DrawDistance(num++, c4, Contour(XY(60,60), 30, 15));
    DrawDistance(num++, c4, Contour(XY(60,60), 20, 15));
    DrawDistance(num++, c4, Contour(XY(60,60), 15, 10));

    DrawDistance(num++, c1, Block(40, 90, 40, 90));
    DrawDistance(num++, c1, Contour(40, 90, 40, 90) + Contour(140,150,30,40));
    DrawDistance(num++, c1 - Block(40, 50, 40, 50), Block(60, 70, 60, 70));
    DrawDistance(num++, c1 - Block(40, 50, 40, 50) - Block (65, 75, 65, 75), Block(60, 70, 60, 70));
    DrawDistance(num++, c4 + Block(50, 80, 50, 80), Block(60, 70, 60, 70));
}

void contour_test_cut(unsigned num)
{
    generate_forms();
    const XY start[] = {XY(0,0), XY(20,30), XY(100, 200)};
    const XY end[] = {XY(30,40), XY(210,20), XY(10, 30)};
    DrawCut(num++, generated_forms::boxhole.GetBoundingBox(), sizeof(start)/sizeof(XY), start, end);
    DrawCut(num++, generated_forms::tri, sizeof(start)/sizeof(XY), start, end);
    DrawCut(num++, generated_forms::raster, sizeof(start)/sizeof(XY), start, end);
    DrawCut(num++, generated_forms::boxhole, sizeof(start)/sizeof(XY), start, end);
    DrawCut(num++, generated_forms::cooomplex3, sizeof(start)/sizeof(XY), start, end);
}


void contour_test_expand2D(unsigned num)
{
    generate_forms();
    DrawExpand2D(num++, generated_forms::boxhole, XY(10,10));
    DrawExpand2D(num++, generated_forms::boxhole, XY(10,20));
    DrawExpand2D(num++, generated_forms::boxhole, XY(10,30));
    DrawExpand2D(num++, generated_forms::boxhole, XY(30,10));
    DrawExpand2D(num++, generated_forms::boxhole, XY(0,100));

    DrawExpand2D(num++, Contour(XY(50,50),40), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40), -XY(5,10));

    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 00), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 10), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 20), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 30), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 40), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 50), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 60), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 70), XY(30,10));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 80), XY(30,10));

    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 00), -XY(10,5));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 10), -XY(10,5));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 20), -XY(10,5));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 30), -XY(10,5));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 40), -XY(10,5));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 50), -XY(10,5));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 60), -XY(10,5));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 70), -XY(10,5));
    DrawExpand2D(num++, Contour(XY(50,50),40, 20, 80), -XY(10,5));

    DrawExpand2D(num++, generated_forms::tri, XY(30,10));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(10), XY(30,10));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(20), XY(30,10));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(30), XY(30,10));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(40), XY(30,10));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(50), XY(30,10));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(60), XY(30,10));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(70), XY(30,10));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(80), XY(30,10));

    DrawExpand2D(num++, generated_forms::tri, -XY(10,5));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(10), -XY(10,5));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(20), -XY(10,5));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(30), -XY(10,5));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(40), -XY(10,5));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(50), -XY(10,5));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(60), -XY(10,5));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(70), -XY(10,5));
    DrawExpand2D(num++, generated_forms::tri.CreateRotated(80), -XY(10,5));

    DrawExpand2D(num++, generated_forms::circle, XY(30,10));
    DrawExpand2D(num++, generated_forms::concave, XY(30,10));
    DrawExpand2D(num++, generated_forms::cooomplex2[0], XY(30,10));
    DrawExpand2D(num++, generated_forms::cooomplex2[1], XY(30,10));
    DrawExpand2D(num++, generated_forms::cooomplex2, XY(30,10));
}

void contour_test_tangent(unsigned num)
{
    generate_forms();
    DrawTangent(num++, generated_forms::boxhole, generated_forms::boxhole.CreateShifted(XY(50,20)));
    DrawTangent(num++, generated_forms::boxhole, generated_forms::boxhole.CreateShifted(XY(50,-20)));
    DrawTangent(num++, generated_forms::boxhole, generated_forms::tri.CreateShifted(XY(150,20)));
    DrawTangent(num++, generated_forms::boxhole, Contour(XY(150,20), 10, 20, 30));
    DrawTangent(num++, Contour(XY(10,50), 30, 10, -10), Contour(XY(150,20), 10, 20, 30));
    DrawTangent(num++, generated_forms::boxhole, Contour(generated_forms::cooomplex2[0]).CreateShifted(XY(50,20)));
    DrawTangent(num++, generated_forms::boxhole, Contour(generated_forms::cooomplex2[1]).CreateShifted(XY(50,20)));
    DrawTangent(num++, generated_forms::boxhole, generated_forms::cooomplex2.CreateShifted(XY(50,20)));
    DrawTangent(num++, generated_forms::cooomplex2, Contour(XY(150,20), 10, 20, 30));
    DrawTangent(num++, generated_forms::cooomplex2.CreateRotated(10), generated_forms::cooomplex2.CreateShifted(XY(100,20)));
}


void DrawBezier(CairoContext &c, Edge &A)
{
    cairo_move_to(c.cr, A.GetStart().x, A.GetStart().y);
    A.PathTo(c.cr);
    cairo_set_line_width(c.cr, 1);
    cairo_set_source_rgb(c.cr, 0, 0, 0);
    cairo_stroke(c.cr);
}

void DrawDot(CairoContext &c, XY p)
{
    cairo_arc(c.cr, p.x, p.y, 5, 0, 2*M_PI);
    cairo_fill(c.cr);
}

void DrawIntersection(CairoContext &c, Edge &A, std::vector<Edge> &OK, 
    double off=0)
{
    DrawBezier(c, A);
    for (unsigned uu = 0; uu<OK.size(); uu++) {
        Edge edge(OK[uu].CreateShifted(XY(0, off*(uu+1))));
        DrawBezier(c, edge);
        double m[10], o[10];
        XY r[10];
        unsigned num = A.Crossing(OK[uu], r, m, o);
        for (unsigned u = 0; u<num; u++)
            DrawDot(c, r[u]+XY(0, off*(uu+1)));
    }
}

void contour_test_bezier(unsigned num)
{
    Edge A(XY(10, 100), XY(110, 100));
    std::vector<Edge> OK = {
        Edge(XY(20, 100), XY(120, 100)),
        Edge(XY(0, 100), XY(120, 100)),
        Edge(XY(20, 100), XY(90, 100)),
        Edge(XY(0, 100), XY(10, 100)),
        Edge(XY(0, 100), XY(5, 100)),
        Edge(XY(110, 100), XY(120, 100)),
        Edge(XY(115, 100), XY(120, 100)),
        Edge(XY(50, 50), XY(75, 150))
    };
    {
        CairoContext ctx(num, Block(-10, 200, -10, 200), "bezier 1");
        DrawIntersection(ctx, A, OK, 10);
    }

    {
        CairoContext c(num+1, Block(-10, 200, -10, 200), "bezier 2");
        Edge B(XY(10, 100), XY(110, 100), XY(10, 50), XY(110, 50)), C, D, E, F;
        DrawBezier(c, B);
        B.Split(C, D);
        C.Split(0.2, E, F);
        DrawBezier(c, C.CreateShifted(XY(2, 2)));
        DrawBezier(c, D.CreateShifted(XY(4, 4)));
        DrawBezier(c, E.CreateShifted(XY(6, 6)));
        DrawBezier(c, F.CreateShifted(XY(8, 8)));
    }


    Edge B(XY(10, 100), XY(110, 100), XY(10, 50), XY(110, 50));
    std::vector<Edge> OK2 = {
        Edge(XY(50, 50), XY(75, 150)),
        Edge(XY(10, 50), XY(75, 150)),
        Edge(XY(80, 50), XY(45, 150), XY(100, 90), XY(70, 150)),
        Edge(XY(100, 50), XY(45, 50), XY(120, 140), XY(20, 150))
    };
    {
        CairoContext ctx(num+2, Block(-10, 200, -10, 200), "bezier 3");
        DrawIntersection(ctx, B, OK2);
    }

    auto b = B.Angle(false, 0);
    auto c = B.Angle(true, 0.5);
    auto d = B.Angle(false, 0.5);
    auto e = B.Angle(true, 1);


    {
        CairoContext c(num+3, Block(-10, 200, -10, 200), "bezier 4");
        Edge B(XY(10, 100), XY(110, 100), XY(10, 50), XY(110, 50)), C, D, E, F;
        DrawBezier(c, B);
        B.Split(C, D);
        C.Split(E, F);
        Block BB = B.CreateBoundingBox();
        cairo_rectangle(c.cr, BB.x.from, BB.y.from, BB.x.Spans(), BB.y.Spans());
        cairo_stroke(c.cr);
    }
}

/** A set of drawing operations drawing interesting test cases.
 * @ingroup contour_internal
 * Not very sophisticated, I admit.
 */
void contour_test(void)
{
    generate_forms();
    using namespace generated_forms;

    Contour cc;
    cooomplex2[0].outline.Expand(EXPAND_ROUND, -16, cc, CONTOUR_INFINITY);

    DrawExpand(153, EXPAND_MITER, CONTOUR_INFINITY, cooomplex2[0], 2, "rounded complex3 with miter");

    contour_test_basic();
    contour_test_expand();
    contour_test_assign(111);
    contour_test_lohere();
    contour_test_area(400);
    contour_test_relations(7000);
    contour_test_distance(7100);
    contour_test_cut(7300);
    contour_test_expand2D(7400);
    contour_test_tangent(7500);
    contour_test_bezier(8000);
}

} //namespace