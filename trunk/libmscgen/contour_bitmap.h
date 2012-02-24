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
#include "contour.h"

namespace contour {


//These bitmaps contain an A8 (8-bits per pixel) bitmaps
//pixels thar are 255 are covered fully, 0 are not covered, anything in-between
//are covered partly. Latter is normalized to 128 always
class Bitmap {
protected:
    unsigned x, y, stride;
    unsigned char *data;
    cairo_surface_t *surface;
    cairo_t *context;
    bool empty;
    bool Allocate(unsigned _x, unsigned _y);
    void Deallocate(); 
    void Normalize();
    unsigned char *GetPos(unsigned _x, unsigned _y) {return data + _y*stride + _x;}
public:
    Bitmap() : data(NULL) {}
    Bitmap(unsigned _x, unsigned _y) {Allocate(_x, _y); if (data) memset(data, 0, y*stride);}
    Bitmap(const Bitmap&b) : data(NULL) {operator = (b);}
    Bitmap(Bitmap &&b) : data(NULL) {operator = (std::move(b));};
    Bitmap &operator = (const Bitmap &);
    Bitmap &operator = (Bitmap &&);
    ~Bitmap() {Deallocate();}
    static unsigned GetStride(unsigned width) {return cairo_format_stride_for_width(CAIRO_FORMAT_A8, width);}
    const unsigned char *GetData() const {return data;}
    const unsigned GetX() const {return x;}
    const unsigned GetY() const {return y;}
    const unsigned char *GetPos(unsigned _x, unsigned _y) const {return data + _y*stride + _x;}

    template<typename C> void Fill(const C&c) {c.Fill(context);} //C must have ::Fill(cairo_t *)
    template<typename C> void Fill(const C&, const XY &shift, unsigned magnify=1); //C must have ::Fill(cairo_t *)
    template<typename CL> void FillList(const CL &cl, const XY &shift=XY(0,0), unsigned magnify=1);
    Bitmap CreateDownscale(unsigned magnify) const;
    Bitmap CreateDownscaleBy2() const;
    void DrawOnto(cairo_t *cr, unsigned magnify=1, double _x=0, double _y=0);

    void Overlap(const Bitmap &b, unsigned long long &definite, unsigned long long &partial, 
                 unsigned _x=0, unsigned _y=0) const {Overlap(Bitmap(b), definite, partial, _x, _y);}
    void Overlap(Bitmap &&, unsigned long long &definite, unsigned long long &partial, 
                 unsigned _x=0, unsigned _y=0) const;
    long long Position(const Bitmap &b, unsigned &X, unsigned &Y) const;  //-1 if NOK, ==0 if OK, >0 if partial overlaps
};

template<typename C> void Bitmap::Fill(const C&c, const XY &shift, unsigned magnify)  //C must have ::Fill(cairo_t *)
{
    if (!data || c.IsEmpty()) return;
    cairo_save(context);
    cairo_scale(context, 1./magnify, 1./magnify);
    cairo_translate(context, shift.x, shift.y);
    Fill(C);
    cairo_restore(context);
    //if (empty)
    //    empty = false;
    //else
    //    Normalize();
}

template<typename CL> void Bitmap::FillList(const CL &cl, const XY &shift, unsigned magnify) 
{
    if (!data || cl.size()==0) return;
    cairo_save(context);
    cairo_scale(context, 1./magnify, 1./magnify);
    cairo_translate(context, shift.x, shift.y);
    unsigned count = 1;
    for (auto i=cl.begin(); i!=cl.end(); i++, count++) {
        i->Fill(context);
        //if (count == 8) {
        //    Normalize();
        //    count = 0;
        //}
    }
    cairo_restore(context);
    //if (count!=1) 
    //    Normalize();
    //empty = false;
}


} //namespace