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

#if !defined(NOTES_H)
#define NOTES_H

#include "msc.h"




//This object is used to place a single floating note
class NotePlacement
{
    const Msc                    &chart;
    MscCanvas                    &canvas;
    const CommandNote            &note;
    const PtrList<const Contour> &contour_map_imp;
    const PtrList<const Contour> &contour_map_all;
    Contour                       contour_object;
    Contour                       region_belts[region_distances];

    double TryNotePointer(const PtrList<const Contour> &contour_map, 
        const XY &origin, const XY &pointto) const;
    double PlaceNotePointer(const PtrList<const Contour> &contour_map, const XY &origin, const XY &from, const XY &till, XY &xy) const;
    Score TryPlacingNoteAndArrowHere(const PtrList<const Contour> &contour_map, 
                                                XY &origin, XY &pointto) const;
    Score PlaceNoteInRegion(const PtrList<const Contour> &contour_map, const Bitmap map[], 
                             Contour &&region, XY &origin, XY &pointto) const;
    Contour GetRegion(unsigned distance, int dir_x, int dir_y, XY &start_xy) const;
    Score PlaceNoteInRegions(const PtrList<const Contour> &cont_map, Bitmap map[], 
                              const Contour &contour_target, const TrackableElement &target, 
                              int dir_x, int dir_y, int dist, XY &origin, XY &pointto) const;
public:
    NotePlacement(const Msc &msc, MscCanvas &canvas, const CommandNote &note, 
                  const Bitmap &map_imp, const Bitmap &map_all);
    bool PlaceNote(XY &origin, XY &pointto);
};


#endif //NOTES_H