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
/** @file area.cpp The implementation of Area.
 * @ingroup libmscgen_files */
#include <cassert>
#include "area.h"
#include "commands.h"

/////////////////////////////////////////  Contour implementation

Area Area::CreateExpand(double gap, contour::EExpandType et4pos, contour::EExpandType et4neg,
                        double miter_limit_positive, double miter_limit_negative) const
{
    Area result;
    if (gap == 0) return (result = *this);  //always return result->compiler optimizes
    result.arc = arc;
    result.mainline = mainline;
    result.mainline.Expand(gap);
    Contour::Expand(et4pos, et4neg, gap, result, miter_limit_positive, miter_limit_negative);
    return result;
}

void Area::swap(Area &a)
{
    std::swap(arc, a.arc);
    std::swap(mainline, a.mainline);
    Contour::swap(a);
}

double Area::OffsetBelow(const Area &below, double &touchpoint, double offset, bool bMainline) const
{
    if (offset < below.GetBoundingBox().y.from - GetBoundingBox().y.till) return offset;
    if (bMainline && !mainline.IsEmpty() && mainline.GetBoundingBox().x.Overlaps(below.mainline.GetBoundingBox().x))
        offset = mainline.OffsetBelow(below.mainline, touchpoint, offset);
    if (!GetBoundingBox().x.Overlaps(below.GetBoundingBox().x)) return offset;
    return Contour::OffsetBelow(below, touchpoint, offset);
}


AreaList AreaList::CreateExpand(double gap, contour::EExpandType et4pos, contour::EExpandType et4neg,
                                double miter_limit_positive, double miter_limit_negative) const
{
    if (!gap) return *this;
    AreaList al;
    for (auto i=cover.begin(); i!=cover.end(); i++)
        al += i->CreateExpand(gap, et4pos, et4neg, miter_limit_positive, miter_limit_negative);
    return al;
}

double AreaList::OffsetBelow(const Area &below, double &touchpoint, double offset, bool bMainline) const
{
    if (offset < below.GetBoundingBox().y.from - boundingBox.y.till) return offset;
    if (!boundingBox.x.Overlaps(below.GetBoundingBox().x)) return offset;
    if (bMainline && !mainline.IsEmpty() && mainline.GetBoundingBox().x.Overlaps(below.mainline.GetBoundingBox().x))
        offset = mainline.OffsetBelow(below.mainline, touchpoint, offset);
    for (auto i = cover.begin(); i!=cover.end(); i++)
        offset = i->OffsetBelow(below, touchpoint, offset, bMainline);
    return offset;
}

double AreaList::OffsetBelow(const AreaList &below, double &touchpoint, double offset, bool bMainline) const
{
    if (offset < below.boundingBox.y.from - boundingBox.y.till) return offset;
    if (bMainline && !mainline.IsEmpty() && mainline.GetBoundingBox().x.Overlaps(below.mainline.GetBoundingBox().x))
        offset = mainline.OffsetBelow(below.mainline, touchpoint, offset);
    if (!boundingBox.x.Overlaps(below.boundingBox.x)) return offset;
    for (auto i = cover.begin(); i!=cover.end(); i++)
        for (auto j = below.cover.begin(); j!=below.cover.end(); j++)
            offset = i->OffsetBelow(*j, touchpoint, offset, bMainline);
    return offset;
}
