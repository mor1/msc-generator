/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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

/*Parts of this file is based on WildMagic 5.4, which uses the Boost licence,
  which requires me to repeat it here. See http://www.geometrictools.com

	Boost Software License - Version 1.0 - August 17th, 2003

	Permission is hereby granted, free of charge, to any person or organization
	obtaining a copy of the software and accompanying documentation covered by
	this license (the "Software") to use, reproduce, display, distribute,
	execute, and transmit the Software, and to prepare derivative works of the
	Software, and to permit third-parties to whom the Software is furnished to
	do so, all subject to the following:

	The copyright notices in the Software and this entire statement, including
	the above license grant, this restriction and the following disclaimer,
	must be included in all copies of the Software, in whole or in part, and
	all derivative works of the Software, unless such copies or derivative
	works are solely in the form of machine-executable object code generated by
	a source language processor.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
	SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
	FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/

/** @file contour_distance.cpp Various auxiliary functions
 * non-inline functions from contour_basics.h.
 * @ingroup contour_files
 */

#include <cassert>
#include <vector>
#include <algorithm>
#include "contour_edge.h"

namespace contour {

/** Calculates how the line `A`->`B` crosses the rectangle.
*
* If no crossing, an invalid range is returned.
* If there is a crossing, "pos" values are returned, as follows.
* `0` corresponds to `A`, `1` corresponds to `B`, values in between
* correspond to the section `A`->`B` and outside likewise.
*/
Range Block::Cut(const XY &A, const XY &B) const
{
    Range ret;
    bool have_one = false;
    if (IsInvalid() || A.test_equal(B)) goto invalid;
    if (test_equal(A.x, B.x)) {
        if (!x.IsWithinBool(A.x)) goto invalid;
        ret.from = (y.from-A.y)/(B.y-A.y);
        ret.till = (y.till-A.y)/(B.y-A.y);
        goto valid;
    }
    if (test_equal(A.y, B.y)) {
        if (!y.IsWithinBool(A.y)) goto invalid;
        ret.from = (x.from-A.x)/(B.x-A.x);
        ret.till = (x.till-A.x)/(B.x-A.x);
        goto valid;
    }
    if (y.IsWithinBool((B.y-A.y)/(B.x-A.x)*(x.from-A.x)+A.y)) {
        ret.from = (x.from-A.x)/(B.x-A.x);
        have_one = true;
    }
    if (y.IsWithinBool((B.y-A.y)/(B.x-A.x)*(x.till-A.x)+A.y)) {
        (have_one ? ret.till : ret.from) = (x.till-A.x)/(B.x-A.x);
        if (have_one) goto valid;
        have_one = true;
    }
    if (x.IsWithinBool((B.x-A.x)/(B.y-A.y)*(y.from-A.y)+A.x)) {
        (have_one ? ret.till : ret.from) = (y.from-A.y)/(B.y-A.y);
        if (have_one) goto valid;
        have_one = true;
    }
    if (x.IsWithinBool((B.x-A.x)/(B.y-A.y)*(y.till-A.y)+A.x)) {
        (have_one ? ret.till : ret.from) = (y.till-A.y)/(B.y-A.y);
        if (have_one) goto valid;
        _ASSERT(0);
    }
invalid:
    ret.MakeInvalid();
    return ret;
valid:
    if (ret.from > ret.till)
        std::swap(ret.from, ret.till);
    return ret;
}

/** Returns the relation of the three points `a`, `b`, `c`.
* @ingroup contour_internal
*/
ETriangleDirType triangle_dir(XY a, XY b, XY c)
{
    if (a == b) return b==c ? ALL_EQUAL : A_EQUAL_B;
    if (a == c) return A_EQUAL_C;
    if (b == c) return B_EQUAL_C;
    //Decide if we divide by x or y coordinates
    if (fabs(a.x - b.x) < fabs(a.y - b.y)) {
        const double m = (a.x - b.x)/(a.y - b.y);
        double cx = m*(c.y-a.y) + a.x; //(cx, c.y) is a point on the a-b line
        if (test_equal(cx, c.x)) return IN_LINE;
        return ((c.x < cx) ^ (a.y < b.y)) ? COUNTERCLOCKWISE : CLOCKWISE;
    } else {
        const double m = (a.y - b.y)/(a.x - b.x);
        double cy = m*(c.x-a.x) + a.y; //(c.x, cy) is a point on the a-b line
        if (test_equal(cy, c.y)) return IN_LINE;
        return ((c.y < cy) ^ (a.x < b.x)) ? CLOCKWISE : COUNTERCLOCKWISE;
    }
}


/** Returns the *fake angle* of the `base`->`A` and the `base`->`B` segments.
* @ingroup contour_internal
*
* In order to save computation we do not use the angles in radian
* merely its sine, since we only do comparison with these values, no summation
* or other arithmetics. We call this *fake angle*.
* Values between [0..1] thus correspond to radians [0..PI/2], values
* between [1..2] to radians [PI/2..PI], etc.
*
* We return an angle that follows clockwise and can be larger than 180 degrees.
* So if `B` is just a bit counterclockwise from `A`, we get a value close to 360
* degree (that is the fake value 4).
* @param base The base of the angle.
* @param A one end
* @param B the other end.
* @returns The fake angle from 'A' to 'B'. -1 on error (degenerate cases)
*/
double angle(const XY &base, const XY &A, const XY &B)
{
    bool clockwise;
    switch (triangle_dir(base, A, B)) {
    case IN_LINE:
    case CLOCKWISE:
        clockwise = true;
        break;
    case COUNTERCLOCKWISE:
        clockwise = false;
        break;
    case B_EQUAL_C:
        return 0;  //zero degrees
    default:
        return -1; //error one of them equals to another
    };
    double cos = (A-base).DotProduct(B-base) / base.Distance(A) / base.Distance(B);
    cos = std::min(cos, 1.);
    cos = std::max(cos, -1.);
    if (clockwise)
        return 1-cos; //gives [0..2]
    else
        return cos+3; //gives (2..4)
}

/** Returns the (clockwise) *fake angle* of the `base`->`A` compared to a horizontal line.
* @ingroup contour_internal
*
* In order to save computation we do not use the angles in radian
* merely its sine, since we only do comparison with these values, no summation
* or other arithmetics. We call this *fake angle*.
* Values between [0..1] thus correspond to radians [0..PI/2], values
* between [1..2] to radians [PI/2..PI], etc.
*
* We return an angle that follows clockwise and can be larger than 180 degrees.
* @param base The base of the angle.
* @param A The point of which we calculate the angle for.
* @returns The fake angle of 'base'->'A' from a horizontal line going through 'base'. 
*          -1 on error ('base'=='A')
*/
double angle_to_horizontal(const XY &base, const XY &A)
{
    if (base.test_equal(A))
        return -1;
    double cos = XY(100,0).DotProduct(A-base) / 100 / base.Distance(A);
    //normalize to avoid errors from imprecision above
    cos = std::min(cos, 1.);
    cos = std::max(cos, -1.);
    if (A.y>=base.y)
        return 1-cos; //gives [0..2]
    else
        return cos+3; //gives (2..4)
}

/** Calculates `pos` on a straight segment.
*
* @param [in] M Start of the segment.
* @param [in] N End of the segment.
* @param [in] p Point to calculate pos for. Assumed to be on the line of `M`->`N`.
* @returns pos value. [0..1] if p is inside MN, other value if not.
*/
double point2pos_straight(const XY &M, const XY&N, const XY &p)
{
    if (M.test_equal(N)) {
        _ASSERT(M.test_equal(p));
        return 0;
    }
    if (fabs(M.x-N.x) > fabs(M.y-N.y)) {
        double t = (p.x-M.x)/(N.x-M.x);
        if (test_equal(p.y, M.y + (N.y-M.y)*t)) return t;
    } else {
        double t = (p.y-M.y)/(N.y-M.y);
        if (test_equal(p.x, M.x + (N.x-M.x)*t)) return t;
    }
    _ASSERT(0);
    return -1;
}

}  //namespace contour


