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

const unsigned placing_depth = 1;

class NotePlacement
{
    const Msc * const             chart;
    
    const contour::Bitmap        &original_map_imp;
    const contour::Bitmap        &original_map_all;
    contour::Bitmap               map_all[placing_depth];
    contour::Bitmap               map_imp[placing_depth];
    contour::Bitmap               obj[placing_depth];
public:
    NotePlacement(const Msc *msc);
    bool PlaceNote(const CommandNote *);
};


#endif //NOTES_H