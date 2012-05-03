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

#include "notes.h"


double NotePlacement::TryNotePointer(const PtrList<const Contour> &contour_map, 
    const XY &origin, const XY &pointto) const
{
    const XY center = contour_object.GetBoundingBox().Centroid();
    const double pos_crossing = region_belts[0].Cut(pointto, center).till;
    const XY hit = pointto + pos_crossing*(center-pointto);
    const Contour contour_pointer = note.CoverPointer(canvas, hit).Shift(origin);
    if (contour_pointer.IsEmpty()) return 0;
    double area = 0;
    for (auto i = contour_map.begin(); i!=contour_map.end(); i++)
        area += (**i * contour_pointer).GetArea();
    return area;
}

double NotePlacement::PlaceNotePointer(const PtrList<const Contour> &contour_map, 
    const XY &origin, const XY &from, const XY &till, XY &xy) const
{
    xy = (from+till)/2;
    XY step = (from-till)/4;
    double best_score = TryNotePointer(contour_map, origin, xy);
    while (best_score>0 && step.length()>2) {
        const double sc1 = TryNotePointer(contour_map, origin, xy+step);
        const double sc2 = TryNotePointer(contour_map, origin, xy-step);
        step /= 2;
        if (sc1>best_score && sc2>best_score) continue;
        if (sc1<sc2)
            xy += step;
        else
            xy -= step;
    }
    return best_score;
}


const XY max_distance(200, 50);



//Try placing the note body around "origin" which is a non-overlapping position
//Then try also placing the arrow.
Score NotePlacement::TryPlacingNoteAndArrowHere(const PtrList<const Contour> &contour_map, 
                                                XY &origin, XY &pointto) const
{
    //Now find the distance to all contours in "contour_map"
    const unsigned D = 2*4; //shall be a multiple of 4
    contour::DistanceType<D> s;
    contour::Distance(contour_map, contour_object.CreateShifted(origin), s);
    _ASSERT(!s.WasIn());
//    _ASSERT(!s.IsZero());
    Score score;
    if (s.WasIn()||s.IsZero()) return score; //0,0 here
    //Add a limit at the edge of the chart
    Contour limit(0, chart.total.x, 0, chart.total.y);
    s.Merge(limit.Distance<D>(contour_object.CreateShifted(origin)));
    //Find out what is the disctance in each direction (even if Distance() returned no value)
    //distances start from -> and counterclockwise
    double distances[D], dummy;
    for (unsigned u = 0; u<D; u++) {
        distances[u] = fabs(s.Distance(u));  //limit added negative values
        if (fabs(distances[u]) != MaxVal(dummy)) continue;
        for (unsigned v = 0; v<D; v++) 
            if (fabs(s.Distance(v)) < fabs(distances[u])) {
                const double projected = contour::DistanceType<D>::ranges.ProjektVectorToMidline(u, s.OnMe(v), s.OnOther(v));
                if (projected>0) distances[u] = projected;
            }
    }
    //See where can we move the object so that it is more equidistant from its
    //surrounding
    XY shift;
    shift.x = (distances[0]   - distances[2*(D/4)])/2;
    shift.y = (distances[D/4] - distances[3*(D/4)])/2;
    if (shift.x > max_distance.x) 
        shift.x = std::max(max_distance.x - distances[2*(D/4)], 0.);
    else if (shift.x < -max_distance.x) 
        shift.x = std::min(distances[0] - max_distance.x, 0.);
    if (shift.y > max_distance.y) 
        shift.y = std::max(max_distance.y - distances[3*(D/4)], 0.);
    else if (shift.y < -max_distance.y) 
        shift.y = std::min(distances[D/4] - max_distance.y, 0.);
    origin += shift;
    //Now find the new distance for scoring purposes
    //s.Reset();
    contour::DistanceType<D> ss;
    contour::Distance(contour_map, contour_object.CreateShifted(origin), ss);
    const double md = std::min(max_distance.x, max_distance.y);
    const double dd = ss.Distance();
//    _ASSERT(dd>0);
    if (dd == 0 || fabs(dd) > md)
        score.SetBody(100);
    else 
        score.SetBody(md*100/dd);
    const XY from = note.GetTarget()->GetDefNodeTarget();
    const XY till = from;
    score.SetArrow(PlaceNotePointer(contour_map, origin, from, till, pointto));
    return score;
}

//start search from "origin" use "map" bitmaps and the "contour_map" contour list to 
//search a position to place this->obj + this->contour_object within "region"
//All coordinates are in the space of this->chart->total
Score NotePlacement::PlaceNoteInRegion(const PtrList<const Contour> &contour_map, const Bitmap map[], 
                                        Contour &&region, XY &origin, XY &pointto) const
{
    Score score;
    //Use bitmaps and binary search to place the object 
    SearchStatus status(origin, placing_depth-1, std::move(region));
    unsigned long long candidate_part = (unsigned long long)(long long)(-1);
    do {
        //try placing objet
        unsigned long long partial, definite;
        map[placing_depth-1].Overlap(obj[placing_depth-1], definite, partial, 
            unsigned(status.xy.x / (1<<(placing_depth-1))), 
            unsigned(status.xy.y / (1<<(placing_depth-1))));
        if (!definite) {
            if (partial==0) {
tryhere:
                XY ori=status.xy, pto;
                const Score sc = TryPlacingNoteAndArrowHere(contour_map, ori, pto);
                if (sc > score) {
                    origin = ori;
                    pointto = pto;
                    score = sc;
                    if (score.GoodEnough()) return score;
                }
                continue;
            }
            unsigned long long best_part = (unsigned long long)(long long)(-1);
            XY best_xy = status.xy;
            for (int d = placing_depth-2; d>=0; d--) {
                const double unit = 1>>d;
                const XY base = best_xy/unit;
                //try to refine 
                best_part = (unsigned long long)(long long)(-1);
                for (unsigned u = 0; u<4; u++) {
                    const XY delta((u&1) ? unit : 0, (u&2) ? unit : 0);
                    map[d].Overlap(obj[d], definite, partial, 
                                   unsigned(base.x+delta.x), unsigned(base.y+delta.y));
                    if (definite) continue;
                    if (best_part > partial) {
                        best_part = partial;
                        best_xy = (base+delta)*unit;
                        if (best_part==0) break;
                    }
                }
                if (best_part = (unsigned long long)(long long)(-1)) 
                    break; //definite overlap, move to another point on the coarser map
                if (best_part==0)  //no overlap - try placing 
                    goto tryhere;
                //We have partial overlap, search on a finer resolution
                best_part *= 4; //calculate number on a finer resolution
            }
            if (candidate_part < best_part) //we ended up with partial match on the finest scale do nothing
                candidate_part = best_part;
        }
        //if there is no finer resolution or we had definite overlap, select next
    } while(status.Step());
    return score;
};


//returns score
// zero: not possible to place
// 100: best fit
Score NotePlacement::PlaceNoteInRegions(const PtrList<const Contour> &cont_map, Bitmap map[], 
                                         const Contour &contour_target, const TrackableElement &target, 
                                         int dir_x, int dir_y, int dist, XY &origin, XY &pointto) const
{
    unsigned distances_search_order[region_distances] = {1, 0, 2};
    // if "dist" proscribes a specific distance, (e.g., near or far) start with that
    if (dist>=0) {
        unsigned u = 0;
        while (u<region_distances) {
            if (distances_search_order[u] == dist) break;
            u++;
        }
        _ASSERT(u<region_distances);
        if (u<region_distances) 
            std::swap(distances_search_order[0], distances_search_order[u]);
    }
    Score score;
    for (unsigned u = 0; u<region_distances; u++) {
        XY temp_origin, temp_pointto;
        Contour region = GetRegion(distances_search_order[u], dir_x, dir_y, temp_origin);
        if (region.IsEmpty()) continue;
        const Score new_score = PlaceNoteInRegion(cont_map, map, std::move(region), temp_origin, temp_pointto);
        if (new_score > score) {
            origin = temp_origin;
            pointto = temp_pointto;
            score = new_score;
            if (score.GoodEnough()) return score;
        }
        if (score.GoodEnough()) return score;
    }
    if (!score.Bad()) return score;
    if (dir_x == 0 && dir_y == 0) return score;
    //we did not succeed, relax the requirements of the user
    return PlaceNoteInRegions(cont_map, map, contour_target, target, 0, 0, dist, origin, pointto);
}

bool NotePlacement::PlaceNote(XY &origin, XY &pointto)
{
    const TrackableElement &target = *note.GetTarget();
    const Contour &contour_target = target.GetAreaToDraw();
    //Create bitmaps
    map_all[0] = original_map_all;
    map_all[0].Fill(contour_target); //Add the target to the area to avoid
    for (unsigned u = 1; u<placing_depth; u++)
        map_all[u] = map_all[u-1].CreateDownscaleBy2();

    const XY margin(1,1); //should x==y
    contour_object = note.CoverBody(canvas).Shift(margin);
    Block bb = contour_object.GetBoundingBox();
    _ASSERT(bb.UpperLeft()==margin);
    bb.Expand(margin.x);
    obj[0] = Bitmap(unsigned(bb.x.Spans()), unsigned(bb.y.Spans()));
    obj[0].Fill(contour_object);
    for (unsigned u = 1; u<placing_depth; u++)
        obj[u] = obj[u-1].CreateDownscaleBy2();

    //create regions in which to check
    const static double region_distance_sizes[region_distances+1] = {0, 50, 100, 200};
    Contour cont_target_if_empty = Block(target.GetDefNodeTarget(), target.GetDefNodeTarget()); 
    const Contour &contour_target_to_use = contour_target.IsEmpty() ? cont_target_if_empty : contour_target;
    Contour prev = contour_target_to_use.CreateExpand(region_distance_sizes[region_distances], EXPAND_MITER, EXPAND_MITER);
    for (int i = region_distances-1; i>=0; i--) {
        Contour next = contour_target_to_use.CreateExpand(region_distance_sizes[i], EXPAND_MITER, EXPAND_MITER);
        region_belts[i] = prev - next;
        prev.swap(next);
    }

    
    //See if we can place it onto the map containing all visual elements
    Score score = PlaceNoteInRegions(contour_map_all, map_all, contour_target, target,
        note.float_dir_x, note.float_dir_y, note.float_dist, origin, pointto);
    if (!score.GoodEnough()) {
        map_imp[0] = original_map_imp;
        map_imp[0].Fill(contour_target); //Add the target to the area to avoid
        for (unsigned u = 1; u<placing_depth; u++)
            map_imp[u] = map_imp[u-1].CreateDownscaleBy2();
        XY res_imp;
        const Score score_imp = PlaceNoteInRegions(contour_map_imp, map_imp, contour_target, target,
            note.float_dir_x, note.float_dir_y, note.float_dist, origin, pointto);
       // _ASSERT(score_imp>=score);
        if (score << score_imp) {
            origin = res_imp;
            score = score_imp;
        }
    }
    //ok, unshift result
    origin+=margin;
    return !score.Bad();
}

NotePlacement::NotePlacement(const Msc &msc, MscCanvas &c, const CommandNote &n,
                             const Bitmap &map_imp, const Bitmap &map_all) : 
chart(msc), canvas(c), note(n),
contour_map_imp(msc.NoteMapImp), contour_map_all(msc.NoteMapAll), 
original_map_imp(map_imp), original_map_all(map_all)
{
}