/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010 Zoltan Turanyi
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
#include "geometry_xarea.h"

namespace geometry {

/////////////////////////////////////////  PolygonList implementation

void PolygonList::assign(std::vector<Edge> &&v, bool winding)
{
	clear();
    Polygon tmp(std::move(v));
    tmp.CalculateBoundingBox();
    switch (tmp.Untangle(*this, winding ? Polygon::WINDING_RULE : Polygon::EVENODD_RULE)) {
    case Polygon::SAME:       //just insert tmp
        append(std::move(tmp));
    case Polygon::A_IS_EMPTY: //empty result okay
    case Polygon::OVERLAP:    //result is already in *this
		break;
    }
}

void PolygonList::assign(const std::vector<Edge> &v, bool winding)
{
	clear();
    Polygon tmp(v);
    tmp.CalculateBoundingBox();
    switch (tmp.Untangle(*this, winding ? Polygon::WINDING_RULE : Polygon::EVENODD_RULE)) {
    case Polygon::SAME:       //just insert tmp
        append(std::move(tmp));
    case Polygon::A_IS_EMPTY: //empty result okay
    case Polygon::OVERLAP:    //result is already in *this
		break;
    }
}

PolygonList &PolygonList::operator += (const PolygonWithHoles &p)
{
    if (!boundingBox.Overlaps(p.GetBoundingBox())) {
        append(p);
        return *this;
    }
    PolygonWithHoles current_blob = p;
    for (auto i=begin(); i!=end(); /*none*/) {
        PolygonList res;
        const Polygon::poly_result_t ret = i->Add(current_blob, res);
        switch (ret) {
        default:
            _ASSERT(0);
        case Polygon::OVERLAP:                      //real union with an existing surface
        case Polygon::B_INSIDE_A:              //blob is inside i
        case Polygon::A_INSIDE_B:              //i is fully covered by blob, delete it
        case Polygon::SAME:                    //outer hull is same, holes may be modified
            current_blob.swap(*res.begin());   //update blob with a positive union
            //fallthrough: now delete i
        case Polygon::A_IS_EMPTY:
            erase(i++);
            break;
        case Polygon::APART:   //go to next in queue, no holes to add, keep blob as was
            i++;
            break;
        case Polygon::B_IS_EMPTY:  //empty blob means empty b
        case Polygon::BOTH_EMPTY:  //oh my
            return *this;
        }
    }
    append(current_blob); //this extends bounding box appropriately
    return *this;
}

PolygonList &PolygonList::operator *= (const PolygonWithHoles &p)
{
    if (!boundingBox.Overlaps(p.boundingBox)) {
        clear();
        return *this;
    }
    PolygonList result;
    for (auto i=begin(); i!=end(); i++) {
        PolygonList res;
        i->Mul(p, res);
        result.append(std::move(res));
    }
    swap(result);
    return *this;
}

PolygonList &PolygonList::operator -= (const PolygonWithHoles &p)
{
    if (!boundingBox.Overlaps(p.boundingBox))
        return *this;
    PolygonList result;
    for (auto i=begin(); i!=end(); i++) {
        PolygonList res;
        i->Sub(p, res);
        result.append(std::move(res));
    }
    swap(result);
    return *this;
}


void PolygonList::Expand(double gap, PolygonList &result) const
{
	for (auto i=begin(); i!=end(); i++) {
		PolygonList res;
		i->Expand(gap, res);
		result += res;
	}
}


Polygon::poly_result_t PolygonWithHoles::Add(const PolygonWithHoles &p, PolygonList &res) const
{
    const Polygon::poly_result_t ret = Combine(p, res, UNION);
    switch (ret) {
    default:
        _ASSERT(0);
    case APART:
        res.append(*this);
    case A_IS_EMPTY:
        res.append(p);
    case BOTH_EMPTY:
        break;
    case B_IS_EMPTY:
        res.append(*this);
        break;
    case A_INSIDE_B:
        res.append(p);
        res.rbegin()->holes -= *this;
        break;
    case B_INSIDE_A:
    case SAME:
        res.append(*this);
        res.rbegin()->holes -= p;
        break;
	case OVERLAP:
		if (holes.size())
			res.rbegin()->holes += (holes - p);
        if (p.holes.size())
			res.rbegin()->holes += (p.holes - *this);
		if (holes.size() && p.holes.size())
			res.rbegin()->holes += (holes * p.holes);
        break;
    }
    return ret;
}

Polygon::poly_result_t PolygonWithHoles::Mul(const PolygonWithHoles &p, PolygonList &res) const
{
    const Polygon::poly_result_t ret = Combine(p, res, INTERSECT);
    switch (ret) {
    default:
        _ASSERT(0);
    case A_IS_EMPTY:
    case BOTH_EMPTY:
    case B_IS_EMPTY:
    case APART:
        break; //empty
    case A_INSIDE_B:
        res.append((Polygon)(*this)); //append only surface, no holes
		if (holes.size() || p.holes.size())
			res -= (holes + p.holes);
        break;
    case B_INSIDE_A:
        res.append((Polygon)p); //append only surface, no holes
		//fallthrough
    case OVERLAP:
		if (holes.size() || p.holes.size())
	        res -= (holes + p.holes);
        break;
    case SAME:
        res.append(*this);
        res.rbegin()->holes += p.holes;
        break;
    }
    return ret;
}

Polygon::poly_result_t PolygonWithHoles::Sub(const PolygonWithHoles &p, PolygonList &res) const
{
    const Polygon::poly_result_t ret = Combine(p, res, SUBSTRACT);
    switch (ret) {
    default:
        _ASSERT(0);
    case A_IS_EMPTY:
    case BOTH_EMPTY:
        break; //empty
    case B_IS_EMPTY:
    case APART:
        res.append(*this);
        break; //nothing to substract
    case A_INSIDE_B:
        res = p.holes;
        res *= *this;
        break;
    case B_INSIDE_A:
        res.append(*this);
        res.rbegin()->holes += p;
        break;
    case SAME:
        res = p.holes;
        res -= holes;
        break;
    case OVERLAP:
        res.append(p.holes * Polygon(*this)); //add parts of our surface in p's holes
        res -= holes;
        break;
    }
    return ret;
}

void PolygonWithHoles::Expand(double gap, PolygonList &res) const
{
	Polygon::Expand(gap, res);
	PolygonList tmp;
	holes.Expand(-gap, tmp);
	res -= tmp;
}


Area Area::Expand(double gap) const
{
	Area result;
	if (gap == 0) return (result = *this);  //always return result->compiler optimizes
	result.arc = arc;
	result.draw = draw;
	result.find = find;
	result.mainline = mainline;
	result.mainline.from -= gap;
	result.mainline.from += gap;
	PolygonList::Expand(gap, result);
	return result;
}

void Area::Line2(cairo_t *cr) const
{
    //surface boundaries with existing pen - solid line hopefully
    PolygonList::Path(cr, true, true);
    cairo_stroke(cr);
    //hole boundaries with dashed pen
    double dash[] = {2, 2};
    cairo_save(cr);
    cairo_set_dash(cr, dash, 2, 0);
    PolygonList::Path(cr, true, false);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void Area::swap(Area &a)
{
    std::swap(arc, a.arc);
    std::swap(draw, a.draw);
    std::swap(find, a.find);
    std::swap(mainline, a.mainline);
    PolygonList::swap(a);
}

Area &Area::operator += (const Area &b)
{
    PolygonList::operator+=(b);
    mainline += b.mainline;
    if (b.find) find = true;
    if (int(b.draw)>int(draw)) draw = b.draw;
    if (arc==NULL) arc = b.arc;
    return *this;
}

Area &Area::operator *= (const Area &b)
{
    PolygonList::operator*=(b);
    mainline *= b.mainline;
    if (b.find) find = true;
    if (int(b.draw)>int(draw)) draw = b.draw;
    if (arc==NULL) arc = b.arc;
    return *this;
}

Area &Area::operator -= (const Area &b)
{
    PolygonList::operator-=(b);
    //if any of our from or till falls into g's mainline, act.
    if (b.mainline.IsWithin(mainline.from)) mainline.from=b.mainline.till;
    if (b.mainline.IsWithin(mainline.till)) mainline.till=b.mainline.from;
    //we may end up being invalid (=empty) (=till<from);
    if (b.find) find = true;
    if (int(b.draw)>int(draw)) draw = b.draw;
    if (arc==NULL) arc = b.arc;
    return *this;
}

}; //namespace
