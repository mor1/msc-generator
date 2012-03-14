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


//The full circle is divided into D segments (0th of which has the 0 direction in its middle)
//Here we calculate the dividing points in "angle" units
template <unsigned D> 
struct DistanceAngleRange
{
    double angles[D];
    XY midvector[D];
    double operator [](unsigned d) const {return angles[d];}
    DistanceAngleRange();
    unsigned Index(const XY &me, const XY &other) const;
    double ProjektVectorToMidline(unsigned d, const XY &me, const XY &other) const;
};

template<unsigned D>
DistanceAngleRange<D>::DistanceAngleRange()
{
    for (unsigned u=0; u<D; u++) {
        angles[u] = radian2angle((0.5+u)*(2.*M_PI/D));
        midvector[u].x = cos(u*2.*M_PI/D);
        midvector[u].y = sin(u*2.*M_PI/D);
    }
}

template<unsigned D>
unsigned DistanceAngleRange<D>::Index(const XY &me, const XY &other) const
{
    const double a = angle(me, me+XY(100,0), other);
    unsigned index=0;
    while (index<=D && a>angles[index]) index++;
    if (index==D) index = 0;
    return index;
}

template<unsigned D>
inline double DistanceAngleRange<D>::ProjektVectorToMidline(unsigned d, const XY &me, const XY &other) const
{
    return midvector[d].DotProduct(other-me);
}


template<> 
inline DistanceAngleRange<1>::DistanceAngleRange() {}
template<> 
inline unsigned DistanceAngleRange<1>::Index(const XY &, const XY &) const {return 0;}


template <unsigned D> 
struct DistanceType
{
private:
    friend class SimpleContour;
    friend class ContourWithHoles;
    unsigned smallest; //if zero, this equals 0
    unsigned largest;
    bool was_inside;
    bool was_outside;
    bool valid;
    bool zero;
    struct {
        double distance;
        XY point_on_me;
        XY point_on_other;
    } dirs[D];
    void RecalcLargest() {largest = 0; if (!zero) for (unsigned u = 1; u<D; u++) if (fabs(dirs[largest].distance) < fabs(dirs[u].distance)) largest = u;}
public:
    static const DistanceAngleRange<D> ranges;
    DistanceType() {Reset();}
    void Reset();
    void SwapPoints() {for (unsigned u=0; u<D; u++) std::swap(dirs[u].point_on_me, dirs[u].point_on_other);}
    void Merge(double d, const XY &point_on_me, const XY &point_on_other); 
    void Merge(const DistanceType<D> &o); 
    bool ConsiderBB(double bbdist) const {return fabs(dirs[largest].distance) > fabs(bbdist);}
    unsigned SmallestIndex() const {return smallest;}
    const XY &OnMe(int index=-1) const {return dirs[index<0 ? smallest : index].point_on_me;}
    const XY &OnOther(int index=-1) const {return dirs[index<0 ? smallest : index].point_on_other;}
    double Distance(int index=-1) const  {return dirs[index<0 ? smallest : index].distance;}
    bool IsValid() const {return valid;}
    bool IsZero() const {return zero;}
    void ClearInOut() {was_inside = was_outside = false;}
    void MergeInOut(double dist) {(dist<0 ? was_inside : was_outside) = true;} //Just see if we get mixed in_out. Do not change "distance"
    bool WasIn() const {return was_inside;}
    bool WasOut() const {return was_outside;}
    void MakeAllInside() {was_outside = false; was_inside = true; for (unsigned u = 1; u<D; u++) dirs[u].distance = -fabs(dirs[u].distance);}
    void MakeAllOutside() {was_outside = true; was_inside = false; for (unsigned u = 1; u<D; u++) dirs[u].distance = fabs(dirs[u].distance);}
    void SwapInOut() {std::swap(was_inside, was_outside); for (unsigned u = 1; u<D; u++) dirs[u].distance *= -1;}
};

template<unsigned D> const DistanceAngleRange<D> DistanceType<D>::ranges;

template<> 
inline void DistanceType<1>::RecalcLargest() {}

template<> 
inline bool DistanceType<1>::ConsiderBB(double bbdist) const 
        {return fabs(dirs[0].distance) > fabs(bbdist);}

template<> 
inline const XY &DistanceType<1>::OnMe(int index) const 
        {return dirs[0].point_on_me;}

template<> 
inline const XY &DistanceType<1>::OnOther(int index) const 
        {return dirs[0].point_on_other;}

template<> 
inline double DistanceType<1>::Distance(int index) const 
        {return dirs[0].distance;}

template<> 
inline unsigned DistanceType<1>::SmallestIndex() const 
        {return 0;}


template<unsigned D> 
void DistanceType<D>::Reset()
{
    smallest= 0;
    largest = 0; 
    was_inside = false; 
    was_outside = false; 
    valid = false; 
    zero = false; 
    for (unsigned u=0; u<D; u++) 
        dirs[u].distance = MaxVal(dirs[u].distance);
}

template<unsigned D> 
inline void DistanceType<D>::Merge(double d, const XY &point_on_me, const XY &point_on_other)
{
    _ASSERT(test_equal(fabs(d), point_on_me.Distance(point_on_other)));
    if (zero || fabs(d) == MaxVal(d)) return;
    valid = true;
    if (d>0) was_outside = true;
    else if (d==0) zero = true;
    else was_inside = true;
    const unsigned u = d==0 ? 0 : ranges.Index(point_on_me, point_on_other);
    if (fabs(dirs[u].distance) <= fabs(d)) return;
    dirs[u].distance = d;
    dirs[u].point_on_me = point_on_me;
    dirs[u].point_on_other = point_on_other;
    if (D==1) return;
    if (fabs(dirs[smallest].distance) > fabs(d))
        smallest = u;
    if (largest == u) RecalcLargest();
}


template<unsigned D> 
inline void DistanceType<D>::Merge(const DistanceType<D> &o)
{
    if (!o.IsValid() || zero) return;
    if (!IsValid()) {
        *this = o; 
        return;
    }
    //smallest and largest indexes are valid
    was_inside |= o.was_inside;
    was_outside|= o.was_outside;
    if (o.zero) zero = true;
    bool largest_changed = false;
    for (unsigned u=0; u<D; u++) 
        if (fabs(dirs[u].distance) > fabs(o.dirs[u].distance)) {
            dirs[u] = o.dirs[u];
            largest_changed |= (u==largest);
        }
    if (fabs(dirs[smallest].distance) > fabs(fabs(dirs[o.smallest].distance)))
        smallest = o.smallest;
    if (largest_changed) RecalcLargest();
}

template<> 
inline void DistanceType<1>::Merge(const DistanceType<1> &o)
{
    if (!o.IsValid() || zero) return;
    if (!IsValid()) {
        *this = o; 
        return;
    }
    //smallest and largest indexes are valid
    was_inside |= o.was_inside;
    was_outside|= o.was_outside;
    if (o.zero) zero = true;
    if (fabs(dirs[0].distance) > fabs(o.dirs[0].distance))
            dirs[0] = o.dirs[0];
}

} //namespace

#endif //CONTOUR_DISTANCE_H