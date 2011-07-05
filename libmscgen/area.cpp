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
#include <cassert>
#include "area.h"

/////////////////////////////////////////  Contours implementation

Area Area::CreateExpand(double gap, EExpandType et) const
{
    Area result;
    if (gap == 0) return (result = *this);  //always return result->compiler optimizes
    result.arc = arc;
    result.mainline = mainline;
    result.mainline.from -= gap;
    result.mainline.from += gap;
    Contours::Expand(et, gap, result);
    return result;
}

void Area::Line2(cairo_t *cr) const
{
    //surface boundaries with existing pen - solid line hopefully
    Contours::Path(cr, true, true);
    cairo_stroke(cr);
    //hole boundaries with dashed pen
    double dash[] = {2, 2};
    cairo_save(cr);
    cairo_set_dash(cr, dash, 2, 0);
    Contours::Path(cr, true, false);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void Area::swap(Area &a)
{
    std::swap(arc, a.arc);
    std::swap(mainline, a.mainline);
    Contours::swap(a);
}

Area &Area::operator += (const Area &b)
{
    Contours::operator+=(b);
    mainline += b.mainline;
    if (arc==NULL) arc = b.arc;
    return *this;
}

Area &Area::operator *= (const Area &b)
{
    Contours::operator*=(b);
    mainline *= b.mainline;
    if (arc==NULL) arc = b.arc;
    return *this;
}

Area &Area::operator -= (const Area &b)
{
    Contours::operator-=(b);
    //if any of our from or till falls into g's mainline, act.
    if (b.mainline.IsWithin(mainline.from)) mainline.from=b.mainline.till;
    if (b.mainline.IsWithin(mainline.till)) mainline.till=b.mainline.from;
    //we may end up being invalid (=empty) (=till<from);
    if (arc==NULL) arc = b.arc;
    return *this;
}

Area &Area::operator ^= (const Area &b)
{
    Contours::operator^=(b);
    //No change to mainlines
    if (arc==NULL) arc = b.arc;
    return *this;
}

AreaList AreaList::CreateExpand(double gap, EExpandType et) const
{
    if (!gap) return *this;
    AreaList al;
    for (auto i=cover.begin(); i!=cover.end(); i++)
        al += i->CreateExpand(gap, et);
    return al;
}


