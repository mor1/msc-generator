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
#include "contour_bitmap.h"

using contour::Bitmap;

struct SearchStatus
{
    int dx;
    int dy;
    unsigned step, pos;
    const unsigned limit;
    XY xy;
    const Contour limit_contour;
    SearchStatus(unsigned _x, unsigned _y, unsigned l, const Contour &lc) : 
     dx(1), dy(0), step(1), pos(0), xy(_x,_y), limit(l),
     limit_contour(lc) {}
    SearchStatus(unsigned _x, unsigned _y, unsigned l, Contour &&lc) : 
     dx(1), dy(0), step(1), pos(0), xy(_x,_y), limit(l),
     limit_contour(std::move(lc)) {}
    bool Step();
};

bool SearchStatus::Step()
{
    do {
        if (++pos>step) {
            //we need to turn a corner
            if      (dx== 1) {dx = 0; dy = 1;}
            else if (dy== 1) {dx =-1; dy = 0; step++;}
            else if (dx==-1) {dx = 0; dy =-1;}
            else             {dx = 1; dy = 0; step++;}
            pos = 0;
            if (step>=limit) return false;
        }
        xy.x += dx; 
        xy.y += dy; 
    } while (!limit_contour.IsWithin(xy));
    return true;
}


bool PlaceNoteInRegionBitmap(unsigned sx, unsigned dx, unsigned sy, unsigned dy,
                             const Bitmap map[], const Bitmap obj[], unsigned placing_depth, 
                             unsigned &x, unsigned &y)
{
    //convert coordinates to coarsest thing
    sx >>= placing_depth-1;
    dx >>= placing_depth-1;
    sy >>= placing_depth-1;
    dy >>= placing_depth-1;
    SearchStatus status((sx+dx)/2, (sy+dy)/2, std::max(dx-sx, dy-sy), sx, dx, sy, dy);
    do {
        x = status.x, y = status.y;
        for (int d = placing_depth-1; d>=0; d++, x*=2, y*=2) {
            long long p = map[d].Position(obj[d], x, y);
            if (p<0) break;    //definite overlap, move to another point on the coarser map
            if (p>0) continue; //We have partial overlap, search on a finer resolution
            //no overlap - return with results
            x<<=d;
            y<<=d;
            return true;
        }
        //if there is no finer resolution or we had definite overlap, select next
    } while(status.Step());
    return false;
}

const unsigned placing_depth=1;
const XY max_distance(200, 50);

double PlaceNoteInRegion(const PtrList<const Contour> &cont_map, const Contour &cont_obj, 
                         const Block &b,const Bitmap map[], const Bitmap obj[], 
                         unsigned placing_depth, const XY &total, XY &shift)
{
    unsigned x, y;
    unsigned sx = (unsigned)b.x.from;
    unsigned dx = (unsigned)b.x.till;
    unsigned sy = (unsigned)b.y.from;
    unsigned dy = (unsigned)b.y.till;
    if (!PlaceNoteInRegionBitmap(sx, dx, sy, dy, map, obj, placing_depth, x, y))
        return 0;
    const unsigned D = 2*4; //shall be a multiple of 4
    contour::DistanceType<D> s;
    contour::Distance(cont_map, cont_obj.CreateShifted(XY(x,y)), s);
    _ASSERT(!s.WasIn());
    if (s.WasIn()) return 0;
    Contour limit(0, total.x, 0, total.y);
    s.Merge(limit.Distance<D>(cont_obj.CreateShifted(XY(x,y))));
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
    //See where can we move. distances start from -> and counterclockwise
    shift.x = (distances[0]   - distances[2*(D/4)])/2;
    shift.y = (distances[3*(D/4)] - distances[D/4])/2;
    if (shift.x > max_distance.x) 
        shift.x = std::max(max_distance.x - distances[2*(D/4)], 0.);
    else if (shift.x < -max_distance.x) 
        shift.x = std::min(distances[0] - max_distance.x, 0.);
    if (shift.y > max_distance.y) 
        shift.y = std::max(max_distance.y - distances[D/4], 0.);
    else if (shift.y < -max_distance.y) 
        shift.y = std::min(distances[3*(D/4)] - max_distance.y, 0.);
    shift.x += x;
    shift.y += y;
    contour::Distance(cont_map, cont_obj.CreateShifted(XY(x,y)), s);
    double score;
    const double md = std::min(max_distance.x, max_distance.y);
    if (fabs(s.Distance()) > md)
        score = 100;
    else 
        score = md*100/s.Distance();
    return score;
};

//dir_x can be +1 (right), -1 (left) or 0 (indifferent). Same for dir_y
//distance can be 0, 1, 2 for near, normal and far
//"obj" is the BB of the element for which we make the comment
//We return the region to check in max 4 pieces and ret the number of pieces
unsigned GetRegion(const Block &obj, unsigned distance, int dir_x, int dir_y, Block b[])
{
    const double ideal_distance = 50;
    //A type to denote if "from" or "till" shall be used (false & true, resp)
    struct BoolBlock {
        bool l, r, u, d;
        BoolBlock(bool a, bool b, bool c, bool e) : l(a), r(b), u(c), d(e) {}
    };
    Block regions[] = {Block(+20,+100, -20,+20)};
    BoolBlock breg[]= {BoolBlock(true, true, false, true)};
    
    int sx = (int)((breg[u].l ? b.x.till : b.x.from) + regions[u].x.from);
    int dx = (int)((breg[u].r ? b.x.till : b.x.from) + regions[u].x.till);
    int sy = (int)((breg[u].u ? b.y.till : b.y.from) + regions[u].y.from);
    int dy = (int)((breg[u].d ? b.y.till : b.y.from) + regions[u].y.till);
    if (sx<0) sx = 0;
    if (sy<0) sy = 0;
}

const double score_excellent = 75;
const double score_good_enough = 50;
//returns score
// zero: not possible to place
// 100: best fit
double PlaceNote(const PtrList<const Contour> &cont_map, const Contour &cont_obj, 
                 Bitmap map[], Bitmap obj[], const Contour &cover, const TrackableElement &target, 
                 const XY &total, XY &res) 
{
    const static unsigned distances[] = {1, 0, 2};
    double score = 0;
    Block object_block;
    if (cover.IsEmpty()) 
        object_block = Block(target.GetDefNodeTarget(), target.GetDefNodeTarget());
    else 
        object_block = cover.GetBoundingBox();
    for (unsigned u = 0; u<sizeof(distances)/sizeof(unsigned); u++) {
        Block regions[4];
        const unsigned reg = GetRegion(object_block, u, dir_x, dir_y, regions);
        for (unsigned v=0; v<reg; v++) {
            XY shift;
            const double new_score = PlaceNoteInRegion(cont_map, cont_obj, regions[v], map, obj,
                                                 placing_depth, total, shift);
            if (new_score > score) {
                res = shift;
                score = new_score;
                if (score >= score_excellent) return score;
            }
        }
        if (score >= score_good_enough) return score;
    }
    return score;
}

void Msc::PlaceNotes(MscCanvas &canvas)
{
    _ASSERT(placing_depth>0);
    Bitmap original_map_imp(unsigned(ceil(total.x)), unsigned(ceil(total.y)));
    Bitmap original_map_all(unsigned(ceil(total.x)), unsigned(ceil(total.y)));
    for (auto i = NoteMapImp.begin(); i!=NoteMapImp.end(); i++)
        original_map_imp.Fill(**i);
    for (auto i = NoteMapAll.begin(); i!=NoteMapAll.end(); i++)
        original_map_all.Fill(**i);
    original_map_imp.Frame();
    original_map_all.Frame();
    for (auto note = Notes.begin(); note!=Notes.end(); note++) {
        const TrackableElement &target = *(*note)->GetTarget();
        const Contour &cover = target.GetAreaToDraw();
        //Create bitmaps
        Bitmap map_all[placing_depth], map_imp[placing_depth], obj[placing_depth];
        map_all[0] = original_map_all;
        map_all[0].Fill(cover); //Add the target to the area to avoid
        for (unsigned u = 1; u<placing_depth; u++)
            map_all[u] = map_all[u-1].CreateDownscaleBy2();

        Contour object = (*note)->Cover(canvas);
        Block bb = object.GetBoundingBox();
        XY shift = bb.UpperLeft();
        bb.Expand(1);
        object.Shift(-shift);
        obj[0] = Bitmap(unsigned(bb.x.Spans()), unsigned(bb.y.Spans()));
        obj[0].Fill(object);
        for (unsigned u = 1; u<placing_depth; u++)
            obj[u] = obj[u-1].CreateDownscaleBy2();

        //See if we can place it onto the map containing all visual elements
        XY res;
        double score = PlaceNote(NoteMapAll, object, map_all, obj, cover, target, total, res);
        if (score < score_good_enough) {
            map_imp[0] = original_map_imp;
            map_imp[0].Fill(cover); //Add the target to the area to avoid
            for (unsigned u = 1; u<placing_depth; u++)
                map_imp[u] = map_imp[u-1].CreateDownscaleBy2();
            XY res_imp;
            double score_imp = PlaceNote(NoteMapImp, object, map_imp, obj, cover, target, total, res_imp);
            _ASSERT(score_imp>=score);
            if (score_imp - score > 20 || score == 0) {
                res = res_imp;
                score = score_imp;
            }
        }
        if (score) {
            (*note)->Place(canvas, res.x, res.y);
        }
    }
}
