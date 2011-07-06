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

/////////////////////////////////////////  Contour implementation

Area Area::CreateExpand(double gap, EExpandType et) const
{
    Area result;
    if (gap == 0) return (result = *this);  //always return result->compiler optimizes
    result.arc = arc;
    result.mainline = mainline;
    result.mainline.from -= gap;
    result.mainline.from += gap;
    Contour::Expand(et, gap, result);
    return result;
}

void Area::swap(Area &a)
{
    std::swap(arc, a.arc);
    std::swap(mainline, a.mainline);
    Contour::swap(a);
}


AreaList AreaList::CreateExpand(double gap, EExpandType et) const
{
    if (!gap) return *this;
    AreaList al;
    for (auto i=cover.begin(); i!=cover.end(); i++)
        al += i->CreateExpand(gap, et);
    return al;
}


