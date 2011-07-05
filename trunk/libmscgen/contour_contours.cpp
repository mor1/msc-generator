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
#include "contour_contours.h"

/////////////////////////////////////////  Contours implementation

void Contours::assign(const std::vector<XY> &v, bool winding)
{
    clear();
    if (v.size()<2) return;
    ContourWithHoles tmp;
    for (unsigned i=0; i<v.size(); i++)
        tmp.push_back(Edge(v[i], v[(i+1)%v.size()]));
    tmp.CalculateBoundingBox();
    switch (tmp.Untangle(*this, winding ? Contour::WINDING_NONZERO : Contour::WINDING_EVENODD)) {
    case Contour::SAME:       //just insert tmp
        append(std::move(tmp));
    case Contour::A_IS_EMPTY: //empty result okay
    case Contour::OVERLAP:    //result is already in *this
        break;
    default:
        _ASSERT(0);
    }
}

void Contours::SwapXY()
{
    for (auto i=content.begin(); i!=content.end(); i++)
        i->SwapXY();
    boundingBox.SwapXY();
}

Contours &Contours::operator += (const ContourWithHoles &p)
{
    if (!boundingBox.Overlaps(p.GetBoundingBox())) {
        append(p);
        return *this;
    }
    ContourWithHoles current_blob = p;
    for (auto i=content.begin(); i!=content.end(); /*none*/) {
        Contours res;
        const Contour::result_t ret = i->Add(current_blob, res);
        switch (ret) {
        default:
            _ASSERT(0);
        case Contour::OVERLAP:                 //real union with an existing surface
        case Contour::B_INSIDE_A:              //blob is inside i
        case Contour::A_INSIDE_B:              //i is fully covered by blob, delete it
        case Contour::SAME:                    //outer hull is same, holes may be modified
            if (res.IsEmpty()) {
                _ASSERT(0);
            } else {
                current_blob.swap(*res.content.begin());   //update blob with a positive union
            }
            //fallthrough: now delete i
        case Contour::A_IS_EMPTY:
            content.erase(i++);
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

Contours &Contours::operator *= (const ContourWithHoles &p)
{
    if (!boundingBox.Overlaps(p.boundingBox)) {
        clear();
        return *this;
    }
    Contours result;
    for (auto i=content.begin(); i!=content.end(); i++) {
        Contours res;
        i->Mul(p, res);
        result.append(std::move(res));
    }
    swap(result);
    return *this;
}

Contours &Contours::operator -= (const ContourWithHoles &p)
{
    if (!boundingBox.Overlaps(p.boundingBox))
        return *this;
    Contours result;
    for (auto i=content.begin(); i!=content.end(); i++) {
        Contours res;
        i->Sub(p, res);
        result.append(std::move(res));
    }
    swap(result);
    return *this;
}

Contours &Contours::operator ^= (const ContourWithHoles &p)
{
    if (!boundingBox.Overlaps(p.boundingBox)) {
        append(p);
        return *this;
    }

    ContourWithHoles current_blob = p;
    for (auto i=content.begin(); i!=content.end(); /*none*/) {
        Contours res;
        const Contour::result_t ret = i->Xor(current_blob, res);
        switch (ret) {
        default:
            _ASSERT(0);
        case Contour::OVERLAP:                 //real XOR with an existing surface
        case Contour::B_INSIDE_A:              //blob is inside i
        case Contour::A_INSIDE_B:              //i is fully covered by blob, delete it
        case Contour::SAME:                    //outer hull is same, holes may be modified
            if (res.IsEmpty()) {
                _ASSERT(0);
            } else {
                current_blob.swap(*res.content.begin());   //update blob with a positive union
            }
            //fallthrough: now delete i
        case Contour::A_IS_EMPTY:
            content.erase(i++);
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


    for (auto i=content.begin(); i!=content.end(); i++) {
        Contours res;
        i->Xor(p, res);
        result.append(std::move(res));
    }
    swap(result);
    return *this;
}


void Contours::Expand(EExpandType et, double gap, Contours &result) const
{
    for (auto i=content.begin(); i!=content.end(); i++) {
        Contours res;
        i->Expand(et, gap, res);
        result += res;
    }
}

Contour::result_t ContourWithHoles::Add(const ContourWithHoles &p, Contours &res) const
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
        res.content.rbegin()->holes -= *this;
        break;
    case B_INSIDE_A:
    case SAME:
        res.append(*this);
        res.content.rbegin()->holes -= p;
        break;
    case OVERLAP:
        if (holes.content.size())
            res.content.rbegin()->holes += (holes - p);
        if (p.holes.content.size())
            res.content.rbegin()->holes += (p.holes - *this);
        if (holes.content.size() && p.holes.content.size())
            res.content.rbegin()->holes += (holes * p.holes);
        break;
    }
    return ret;
}

Contour::result_t ContourWithHoles::Mul(const ContourWithHoles &p, Contours &res) const
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
        res.append(ContourWithHoles((Contour)(*this))); //append only surface, no holes
		if (holes.content.size() || p.holes.content.size())
			res -= (holes + p.holes);
        break;
    case B_INSIDE_A:
        res.append(ContourWithHoles((Contour)p)); //append only surface, no holes
        //fallthrough
    case OVERLAP:
        if (holes.content.size() || p.holes.content.size())
            res -= (holes + p.holes);
        break;
    case SAME:
        res.append(ContourWithHoles((Contour)(*this))); //append only surface, no holes
        res.content.rbegin()->holes += p.holes;
        break;
    }
    return ret;
}

Contour::result_t ContourWithHoles::Sub(const ContourWithHoles &p, Contours &res) const
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
        res.content.rbegin()->holes += p;
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

Contour::result_t ContourWithHoles::Xor(const ContourWithHoles &p, Contours &res) const
{
    const Contour::result_t ret = Contour::Xor(p, res);
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
        res.content.rbegin()->holes ^= *this;
        break;
    case B_INSIDE_A:
        res.append(*this);
        res.content.rbegin()->holes ^= p;
        break;
    case OVERLAP:
        res ^= holes; 
        res ^= p.holes;
        break;
    }
    return ret;
}

void ContourWithHoles::Expand(EExpandType et, double gap, Contours &res) const
{
    Contour::Expand(et, gap, res);
    Contours tmp;
    holes.Expand(et, -gap, tmp);
    res -= tmp;
}

