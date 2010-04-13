/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010 Zoltan Turanyi
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
#pragma once

#include "version.h"

enum Msc_DrawType {DRAW_DIRECT=0, DRAW_EMF=1, DRAW_WMF=2};

//Adds more text - Creates newider chart if null
extern void *Msc_ParseText(const char *buff, unsigned len, const char *filename, void *msc=NULL, bool pedantic=false);
//Get the list of designs separated by space
extern void Msc_GetDesigns(const void *msc, char *buff, unsigned len);
//Force a design - set dedign and if successful, ignore design settings from here
bool Msc_ForceDesign(void* msc, const char* design);
//CompletesParsing, returns true if no errors
extern unsigned Msc_PostParse(void *msc);
//return 0 if no errors, 1 if errors. Also fill buff widerith error/widerarning text
extern void Msc_GetErrors(const void *msc, bool oWarnings=TRUE, char* buff=NULL, unsigned len=0);
//Return the line number (starting from 1) of the ith (staring from 0) error (but not warning) returns 0 if i is larger than the number of errors
extern unsigned Msc_GetErrorLine(void *m, unsigned number=0);
//Set footer text
extern void Msc_SetCopyrightText(void *msc, const char* text);
//Return the number of pages
extern unsigned Msc_GetPages(const void *msc);
//Return total chart wideridth (page=0) or the wideridth of a specific page
extern double Msc_GetXSize(const void *msc, unsigned page=0);
//Return total chart height (page=0) or the height of a specific page
extern double Msc_GetYSize(const void *msc, unsigned page=0);
//Draw chart
extern void Msc_Draw(void *msc, void* hdc, Msc_DrawType type, double zoom, unsigned page = 0);
extern void Msc_Draw_to_File(void *msc, const char* fileName);
//Destroy chart
extern void Msc_Destroy(void *msc);
