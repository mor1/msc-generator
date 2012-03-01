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

#include "contour_bitmap.h"

namespace contour {


bool Bitmap::Allocate(unsigned _x, unsigned _y)
{
    if (_x==0 || _y==0) {
        data = NULL;
        return false;
    }
    empty = true;
    x = _x;
    y = _y;
    stride = GetStride(_x);
    data = new unsigned char[stride*_y];
    
    if (!data) goto error1;
    surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_A8, _x, _y, stride);
    if (!surface) goto error2;
    cairo_status_t status = cairo_surface_status(surface);
    if (status != CAIRO_STATUS_SUCCESS) goto error3;
    context = cairo_create(surface);
    status = cairo_status(context);
    if (status != CAIRO_STATUS_SUCCESS) goto error4;
    return true;

error4:
    cairo_destroy(context);
error3:
    cairo_surface_destroy(surface);
error2:
    delete data;
error1:
    data = NULL;
    surface = NULL;
    context = NULL;
    x = y = stride = 0;
    return false;
}

void Bitmap::Deallocate()
{
    if (data) {
        cairo_destroy(context);
        cairo_surface_destroy(surface);
        delete data;
    }
    data = NULL;
    surface = NULL;
    context = NULL;
    x = y = stride = 0;
}

Bitmap & Bitmap::operator = (const Bitmap &b)
{
    Deallocate();
    if (!b.data) return *this;
    Allocate(b.x, b.y);
    memcpy(data, b.data, y*stride);
    return *this;
}

Bitmap &Bitmap::operator = (Bitmap &&b)
{
    Deallocate();
    if (!b.data) return *this;
    x = b.x;
    y = b.y;
    stride = b.stride;
    data = b.data;
    empty = b.empty;
    surface = b.surface;
    context = b.context;
    b.data = NULL;
    b.context = NULL;
    b.surface = NULL;
    return *this;
}

void Bitmap::Normalize() 
{
    if (!data) return;
    const unsigned char *end = data + y*stride;
    for (unsigned char *p = data; p<end; p++)
        if (*p!=0 && *p!=255) *p=128;
}

//This function returns how many pixels of overlap there was.
//A definite overlap is when at least one of the bitmaps is 255
//A partial overlap is when none of them is neither 255 nor 0
//Here we expect normalized bitmaps.
//_x and _y are coordinates of "this"
void Bitmap::Overlap(Bitmap &&b, unsigned long long &definite, unsigned long long &partial, unsigned _x, unsigned _y) const
{
    if (!b.data || !data) return;
    //Here we expect a normalized bitmap
    //We do an operator CAIRO_OPERATOR_IN, which creates a multiplication
    //of alpha values (we only have alpha in CAIRO_FORMAT_A8)
    cairo_set_source_surface(b.context, surface, -double(_x), -double(_y));
    cairo_set_operator(b.context, CAIRO_OPERATOR_IN);
    cairo_paint(b.context);
    register unsigned long long de = 0, pa = 0;
    for (register int m = b.y-1; m>=0; m--) {
        const register unsigned char *end = b.data + m*b.stride;
        for (register const unsigned char *p = end + b.x-1; p>=end; p--)
            if (*p==0) continue;
            else if (*p>=128) de++;
            else pa++;
    }
    definite = de;
    partial = pa;
}

void Bitmap::Frame()
{
    for (unsigned char *p = data, *end = p+x; p<end; p++)
        *p=255;
    for (unsigned char *p = data+(y-1)*stride, *end = p+x; p<end; p++)
        *p=255;
    for (unsigned char *p = data, *end = p+y*stride; p<end; p+=stride)
        *p=255;
    for (unsigned char *p = data+x-1, *end = p+y*stride; p<end; p+=stride)
        *p=255;
}

Bitmap Bitmap::CreateDownscale(unsigned magnify) const
{
    if (!data || magnify<=1) return *this; //a copy
    Bitmap b((x+magnify-1)/magnify, (y+magnify-1)/magnify);
    if (!b.data) return b;
    for (unsigned n=0, n_new=0; n<y; n+=magnify, n_new++) 
        for (unsigned m=0, m_new=0; m<x; m+=magnify, m_new++) {
            const unsigned n2_end = std::min(n+magnify, y);            
            bool all255 = true;
            bool all000 = true;
            for (unsigned n2 = n; n2<n2_end; n2++) {
                const unsigned char *const p_end = GetPos(std::min(m+magnify, x), n2);
                for (const unsigned char *p = GetPos(m, n2); p<p_end; p++) {
                    if (*p!=0) all000 = false;
                    if (*p!=255) all255 = false;
                    if (!all000 && !all255) goto end;
                }
            }
            //by default the new bitmap is all zero
            //here we cannot have all000 and all255 both false
            //so if all255 is false, all000 is true and we do not need to do anyting
            if (all255) *b.GetPos(m_new, n_new) = 255;
            else continue;
end:
            *b.GetPos(m_new, n_new) = 128;
        }
    return b;
}

Bitmap Bitmap::CreateDownscaleBy2() const
{
    if (!data) return *this; //a copy
    Bitmap b((x+1)/2, (y+1)/2);
    if (!b.data) return b;
    for (unsigned n=0, n_new=0; n<y-1; n+=2, n_new++) {
        const unsigned char *const end = GetPos(x-1, n);
        const unsigned char *s1 = GetPos(0, n), *s2 = GetPos(0, n+1);
        unsigned char *p = b.GetPos(0, n_new);
        for(; s1<end; s1+=2, s2+=2, p++)
            if (255 == (s1[0] & s1[1] & s2[0] & s2[1])) *p = 255;
            else if (0 != (s1[0] | s1[1] | s2[0] | s2[1])) *p = 128;
        if (x%1) { //odd width
            if (255 == (s1[0] & s2[0])) *p = 255;
            else if (0!=(s1[0] | s2[0])) *p = 128;
        }
    }
    if (y%1) { //odd height
        const unsigned char *const end = GetPos(x-1, y-1);
        const unsigned char *s1 = GetPos(0, y-1);
        unsigned char *p = b.GetPos(0, (y+1)/2);
        for(; s1<=end; s1+=2, p++)
            if (255 == (s1[0] & s1[1])) *p = 255;
            else if (0 != (s1[0] | s1[1])) *p = 128;
        if (x%1)  //odd width
            *p = *s1;
    }
    return b;
}


//_x and _y are coordinates of "cr"
void Bitmap::DrawOnto(cairo_t *cr, unsigned magnify, double _x, double _y)
{
    cairo_save(cr);
    cairo_translate(cr, _x, _y);
    cairo_scale(cr, magnify, magnify);
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_pattern_t *pattern = cairo_pattern_create_rgba(0, 1, 0, 0.5);
    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE);
    cairo_mask(cr, pattern);
    cairo_pattern_destroy(pattern);
    cairo_restore(cr);
}

//checks the small box of offset of X,Y to X+1,Y+1, where is best non-overlap
//returns the number of partial hits, 0 if we are not overlapping
//and <0 if definite overlaps
long long Bitmap::Position(const Bitmap &b, unsigned &X, unsigned &Y) const
{
    unsigned long long partial, definite, best_part = (unsigned long long)(long long)(-1);
    unsigned best_x, best_y;
    for (unsigned u = 0; u<4; u++) {
        Overlap(b, definite, partial, X + ((u&1) ? 1 : 0), Y + ((u&2) ? 1 : 0));
        if (definite) continue;
        if (!partial) {
            X += ((u&1) ? 1 : 0);
            Y += ((u&2) ? 1 : 0);
            return 0;
        }
        if (best_part > partial) {
            best_part = partial;
            best_x = X + ((u&1) ? 1 : 0);
            best_y = Y + ((u&2) ? 1 : 0);
        }
    }
    if (best_part == (unsigned long long)(long long)(-1)) return -1;
    X = best_x;
    Y = best_y;
    return best_part;
}
       

} //namespace