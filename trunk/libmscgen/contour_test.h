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

#include "contour.h"

namespace contour {

extern unsigned ContourTestDebug;
extern unsigned ContourTestDebugMinor;

void Draw(unsigned i, const Contour area1, const Contour area2, const Contour area3, const char *text=NULL);
void Draw(unsigned i, const Contour area1, const Contour area2, const char *text=NULL);
void Draw(unsigned i, const Contour area1, const char *text=NULL);

void contour_test(void);

} //namespace
