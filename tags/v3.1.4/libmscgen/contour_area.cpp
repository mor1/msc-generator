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
#include "contour_area.h"

/////////////////////////////////////////  ContourList implementation

void ContourList::assign(std::vector<Edge> &&v, bool winding)
{
	clear();
    Contour tmp(std::move(v));
    tmp.CalculateBoundingBox();
    switch (tmp.Untangle(*this, winding ? Contour::WINDING_RULE : Contour::EVENODD_RULE)) {
    case Contour::SAME:       //just insert tmp
        append(std::move(tmp));
    case Contour::A_IS_EMPTY: //empty result okay
    case Contour::OVERLAP:    //result is already in *this
		break;
    }
}

void ContourList::assign(const std::vector<Edge> &v, bool winding)
{
	clear();
    Contour tmp(v);
    tmp.CalculateBoundingBox();
    switch (tmp.Untangle(*this, winding ? Contour::WINDING_RULE : Contour::EVENODD_RULE)) {
    case Contour::SAME:       //just insert tmp
        append(std::move(tmp));
    case Contour::A_IS_EMPTY: //empty result okay
    case Contour::OVERLAP:    //result is already in *this
		break;
    }
}

void ContourList::SwapXY()
{
    for (auto i=begin(); i!=end(); i++)
        i->SwapXY();
    boundingBox.SwapXY();
}

ContourList &ContourList::operator += (const ContourWithHoles &p)
{
    if (!boundingBox.Overlaps(p.GetBoundingBox())) {
        append(p);
        return *this;
    }
    ContourWithHoles current_blob = p;
    for (auto i=begin(); i!=end(); /*none*/) {
        ContourList res;
        const Contour::result_t ret = i->Add(current_blob, res);
        switch (ret) {
        default:
            _ASSERT(0);
        case Contour::OVERLAP:                 //real union with an existing surface
        case Contour::B_INSIDE_A:              //blob is inside i
        case Contour::A_INSIDE_B:              //i is fully covered by blob, delete it
        case Contour::SAME:                    //outer hull is same, holes may be modified
            current_blob.swap(*res.begin());   //update blob with a positive union
            //fallthrough: now delete i
        case Contour::A_IS_EMPTY:
            erase(i++);
            break;
        case Contour::APART:   //go to next in queue, no holes to add, keep blob as was
            i++;
            break;
        case Contour::B_IS_EMPTY:  //empty blob means empty b
        case Contour::BOTH_EMPTY:  //oh my
            return *this;
        }
    }
    append(std::move(current_blob)); //this extends bounding box appropriately
    return *this;
}

ContourList &ContourList::operator *= (const ContourWithHoles &p)
{
    if (!boundingBox.Overlaps(p.boundingBox)) {
        clear();
        return *this;
    }
    ContourList result;
    for (auto i=begin(); i!=end(); i++) {
        ContourList res;
        i->Mul(p, res);
        result.append(std::move(res));
    }
    swap(result);
    return *this;
}

ContourList &ContourList::operator -= (const ContourWithHoles &p)
{
    if (!boundingBox.Overlaps(p.boundingBox))
        return *this;
    ContourList result;
    for (auto i=begin(); i!=end(); i++) {
        ContourList res;
        i->Sub(p, res);
        result.append(std::move(res));
    }
    swap(result);
    return *this;
}

ContourList &ContourList::operator ^= (const ContourWithHoles &p)
{
    if (!boundingBox.Overlaps(p.boundingBox)) {
        append(p);
        return *this;
    }
    ContourList result;
    for (auto i=begin(); i!=end(); i++) {
        ContourList res;
        i->DoXor(p, res);
        result.append(std::move(res));
    }
    swap(result);
    return *this;
}


void ContourList::Expand(double gap, ContourList &result) const
{
	for (auto i=begin(); i!=end(); i++) {
		ContourList res;
		i->Expand(gap, res);
		result += res;
	}
}

Contour::result_t ContourWithHoles::Add(const ContourWithHoles &p, ContourList &res) const
{
    const Contour::result_t ret = Union(p, res);
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

Contour::result_t ContourWithHoles::Mul(const ContourWithHoles &p, ContourList &res) const
{
    const Contour::result_t ret = Intersect(p, res);
    switch (ret) {
    default:
        _ASSERT(0);
    case A_IS_EMPTY:
    case BOTH_EMPTY:
    case B_IS_EMPTY:
    case APART:
        break; //empty
    case A_INSIDE_B:
        res.append((Contour)(*this)); //append only surface, no holes
		if (holes.size() || p.holes.size())
			res -= (holes + p.holes);
        break;
    case B_INSIDE_A:
        res.append((Contour)p); //append only surface, no holes
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

Contour::result_t ContourWithHoles::Sub(const ContourWithHoles &p, ContourList &res) const
{
    const Contour::result_t ret = Substract(p, res);
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
        res.append(p.holes * Contour(*this)); //no holes of ours: add parts of our surface in p's holes
        res -= holes;
        break;
    }
    return ret;
}

Contour::result_t ContourWithHoles::Xor(const ContourWithHoles &p, ContourList &res) const
{
    const Contour::result_t ret = DoXor(p, res);
    switch (ret) {
    default:
        _ASSERT(0);
    case APART:
        res.append(*this);
    case A_IS_EMPTY:
        res.append(p);
    case BOTH_EMPTY:
    case SAME:
        break; //empty
    case B_IS_EMPTY:
        res.append(*this);
        break;
    case A_INSIDE_B:
        res.append(p);
        res.rbegin()->holes ^= *this;
        break;
    case B_INSIDE_A:
        res.append(*this);
        res.rbegin()->holes ^= p;
        break;
    case OVERLAP:
        res -= holes+p.holes;
        break;
}
    return ret;
}

void ContourWithHoles::Expand(double gap, ContourList &res) const
{
	Contour::Expand(gap, res);
	ContourList tmp;
	holes.Expand(-gap, tmp);
	res -= tmp;
}

is_within_t Area::IsWithin(const XY & p) const
{
    for (auto i=begin(); i!=end(); i++) {
        is_within_t ret = i->IsWithin(p);
        if (ret != WI_OUTSIDE) return ret;
    }
    return WI_OUTSIDE;
}

Area Area::CreateExpand(double gap) const
{
	Area result;
	if (gap == 0) return (result = *this);  //always return result->compiler optimizes
	result.arc = arc;
	result.mainline = mainline;
	result.mainline.from -= gap;
	result.mainline.from += gap;
	ContourList::Expand(gap, result);
	return result;
}

void Area::Line2(cairo_t *cr) const
{
    //surface boundaries with existing pen - solid line hopefully
    ContourList::Path(cr, true, true);
    cairo_stroke(cr);
    //hole boundaries with dashed pen
    double dash[] = {2, 2};
    cairo_save(cr);
    cairo_set_dash(cr, dash, 2, 0);
    ContourList::Path(cr, true, false);
    cairo_stroke(cr);
    cairo_restore(cr);
}

void Area::swap(Area &a)
{
    std::swap(arc, a.arc);
    std::swap(mainline, a.mainline);
    ContourList::swap(a);
}

Area &Area::operator += (const Area &b)
{
    ContourList::operator+=(b);
    mainline += b.mainline;
    if (arc==NULL) arc = b.arc;
    return *this;
}

Area &Area::operator *= (const Area &b)
{
    ContourList::operator*=(b);
    mainline *= b.mainline;
    if (arc==NULL) arc = b.arc;
    return *this;
}

Area &Area::operator -= (const Area &b)
{
    ContourList::operator-=(b);
    //if any of our from or till falls into g's mainline, act.
    if (b.mainline.IsWithin(mainline.from)) mainline.from=b.mainline.till;
    if (b.mainline.IsWithin(mainline.till)) mainline.till=b.mainline.from;
    //we may end up being invalid (=empty) (=till<from);
    if (arc==NULL) arc = b.arc;
    return *this;
}

void Area::Fill2(cairo_t *cr, int r, int g, int b) const
{
    if (size()==0) return;
    int rr=r/size();
    int gg=g/size();
    int bb=b/size();
    for (auto i = begin(); i!=end(); i++) {
        cairo_set_source_rgb(cr, r/255., g/255., b/255.);
        i->Path(cr, false, true);
        cairo_fill(cr);
        r-=rr;
        g-=gg;
        b-=bb;
    }
}


AreaList AreaList::CreateExpand(double gap) const
{
    if (!gap) return *this;
    AreaList al;
    for (auto i=cover.begin(); i!=cover.end(); i++)
        al += i->CreateExpand(gap);
    return al;
}


