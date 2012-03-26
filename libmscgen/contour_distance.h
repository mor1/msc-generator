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

#if !defined(CONTOUR_DISTANCE_H)
#define CONTOUR_DISTANCE_H

#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "contour_basics.h"

namespace contour {

//an fmod, that always return a value in [0..b] even if a<0
template<typename real> real fmod_negative_safe(real a, real b) {_ASSERT(b>0); return a>=0 ? fmod(a, b) : b - fmod(-a, b);}

//fake angle functions to avoid cos and sin.
//"angle is between [0..4) each unit representing a quarter of a circle
double angle(XY base, XY A, XY B);  //expanded in contour_edge.cpp
inline double angle2degree(double angle) {
    return (angle>=2) ? 360 - acos(angle-3)*(180./M_PI) : acos(1-angle)*(180./M_PI);
}
inline double angle2radian(double angle) {
    return (angle>=2) ? 2*M_PI - acos(angle-3) : acos(1-angle);
}
inline double degree2angle(double degree) {
    const double rad = fmod_negative_safe(degree, 360.)/(180./M_PI);
    return (rad>=M_PI) ? 3 + cos(2*M_PI-rad) : 1 - cos(rad);
}
inline double radian2angle(double rad) {
    rad = fmod_negative_safe(rad, 2*M_PI);
    return (rad>=M_PI) ? 3 + cos(2*M_PI-rad) : 1 - cos(rad);
}

struct DistanceType
{
private:
    friend class SimpleContour;
    friend class ContourWithHoles;
    bool was_inside;
    bool was_outside;
    double distance;
    XY point_on_me;
    XY point_on_other;
public:
    DistanceType() {Reset();}
    void Reset() {was_inside = was_outside; distance = MaxVal(distance);}
    void SwapPoints() {std::swap(point_on_me, point_on_other);}
    void Merge(double d, const XY &point_on_me, const XY &point_on_other); 
    void Merge(const DistanceType &o); 
    bool ConsiderBB(double bbdist) const {return fabs(distance) > fabs(bbdist);}
    const XY &OnMe() const {return point_on_me;}
    const XY &OnOther() const {return point_on_other;}
    double Distance() const {return distance;}
    bool IsValid() const {return distance != MaxVal(distance);}
    bool IsZero() const {return distance==0;}
    void ClearInOut() {was_inside = was_outside = false;}
    void MergeInOut(double dist) {(dist<0 ? was_inside : was_outside) = true;} //Just see if we get mixed in_out. Do not change "distance"
    bool WasIn() const {return was_inside;}
    bool WasOut() const {return was_outside;}
    void MakeAllInside() {was_outside = false; was_inside = true; distance = -fabs(distance);}
    void MakeAllOutside() {was_outside = true; was_inside = false; distance = fabs(distance);}
    void SwapInOut() {std::swap(was_inside, was_outside); distance *= -1;}
};

inline void DistanceType::Merge(double d, const XY &p_on_me, const XY &p_on_other)
{
    if (IsZero() || fabs(d) == MaxVal(d)) return;
    if (d>0) was_outside = true;
    else if (d<0) was_inside = true;
    if (fabs(distance) <= fabs(d)) return;
    distance = d;
    point_on_me = p_on_me;
    point_on_other = p_on_other;
}

inline void DistanceType::Merge(const DistanceType &o)
{
    if (o.was_inside) was_inside = true;
    if (o.was_outside) was_outside = true;
    Merge(o.distance, o.point_on_me, o.point_on_other);
}

} //namespace

#endif //CONTOUR_DISTANCE_H