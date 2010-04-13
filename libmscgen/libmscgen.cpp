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
#include "msc.h"
#include "libmscgen.h"


void *Msc_ParseText(const char *buff, unsigned len, const char *filename, void *m, bool pedantic)
{
    Msc *msc;
    if (m==NULL)
        msc = new Msc;
    else
        msc = static_cast<Msc*>(m);
    msc->pedantic = pedantic;
    if (buff && len>0)
        msc->ParseText(buff, len, filename);

    return msc;
}

void Msc_GetDesigns(const void *m, char *buff, unsigned len)
{
	if (m==NULL || buff==NULL || len<2) return;
    const Msc *msc = static_cast<const Msc*>(m);
	string designs = msc->GetDesigns();
	len--;
	if (len > designs.length()) len = designs.length();
	memcpy(buff, designs.c_str(), len);
	buff[len]=0;
}

bool Msc_ForceDesign(void* m, const char* design)
{
	if (m==NULL) return false;
    Msc *msc = static_cast<Msc*>(m);
	if (!msc->SetDesign(design, true)) return false;
	msc->ignore_designs = true;
	return true;
}

unsigned Msc_PostParse(void *m)
{
    Msc *msc = static_cast<Msc*>(m);
    //Allocate (non-sized) output object and assign it to the chart
    //From this point on, the chart sees xy dimensions
    msc->SetOutput(MscDrawer::WMF);
    //Sort Entities, add numbering, fill in auto-calculated values,
    //and throw warnings for badly constructed diagrams.
    msc->PostParseProcess();
    msc->CalculateWidthHeight();
    //A final step of prcessing, checking for additional drawing warnings
    msc->PostHeightProcess();
    msc->CloseOutput();
    return true;
}

void Msc_GetErrors(const void *msc, bool oWarnings, char* buff, unsigned len)
{
    if (msc==NULL) return;
    if (buff != NULL && len > 0) {
        string s = static_cast<const Msc*>(msc)->Error.Print(oWarnings);
        if (len > s.length()+1)
            len = s.length()+1;
        memcpy(buff, s.c_str(), len-1);
        buff[len-1] = 0;
    }
    return;
}

unsigned Msc_GetErrorLine(void *m, unsigned number)
{
    Msc *msc = static_cast<Msc*>(m);
	return msc->Error.GetFileLineOfErrorNo(number).line;
}

void Msc_SetCopyrightText(void *msc, const char* text)
{
    if (msc==NULL) return;
	if (text==NULL || text[0]==0) static_cast<Msc*>(msc)->copyrightText.erase();
	else static_cast<Msc*>(msc)->copyrightText = text;
}

//Return the number of pages
unsigned Msc_GetPages(const void *msc)
{
    if (msc==NULL) return 0;
    return static_cast<const Msc*>(msc)->yPageStart.size();
}

//Returns the unscaled size
double Msc_GetXSize(const void *msc, unsigned page)
{
    if (msc==NULL) return 0;
    //All pages of equal width
    return static_cast<const Msc*>(msc)->totalWidth;
}

//Returns the unscaled size, page numbering starts at 1 here (contrary to Msc::)
double Msc_GetYSize(const void *m, unsigned page)
{
    if (m==NULL) return 0;
	const Msc *msc = static_cast<const Msc*>(m);
	XY offset, size;
	msc->GetPagePosition(int(page)-1, offset, size);
    return size.y*msc->scale + msc->copyrightTextHeight;
}

void Msc_Draw(void *m, void* hdc, Msc_DrawType type, double zoom, unsigned page)
{
    if (m==NULL) return;
	Msc *msc = static_cast<Msc*>(m);
	MscDrawer::OutputType ot;
	switch (type) {
	case ::DRAW_EMF: ot = MscDrawer::EMF; break;
	case ::DRAW_WMF: ot = MscDrawer::WMF; break;
	case ::DRAW_DIRECT: ot = MscDrawer::WIN; break;
        }
    if (!msc->SetOutputWin32(ot, HDC(hdc), zoom, int(page)-1))
        return;
    msc->Draw();
	msc->UnClip(); //Unclip the banner text exclusion clipped in SetOutputWin32()
	msc->DrawCopyrightText(int(page)-1);
    msc->CloseOutput();
}

void Msc_Draw_to_File(void *msc, const char* fileName)
{
    if (msc==NULL) return;
    string fn(fileName);
    size_t pos = fn.find_last_of('.');
    if (pos==string::npos) {
        pos = fn.length();
        fn += ".png";
    }
    string ext = fn.substr(pos+1);
    MscDrawer::OutputType ot;
    if (ext == "png") ot = MscDrawer::PNG;
    else if (ext == "png") ot = MscDrawer::PNG;
    else if (ext == "emf") ot = MscDrawer::EMF;
    else if (ext == "svg") ot = MscDrawer::SVG;
    else if (ext == "pdf") ot = MscDrawer::PDF;
    else if (ext == "eps") ot = MscDrawer::EPS;
    else {
        ot = MscDrawer::EPS;
        fn += ".png";
    }
	//Ignore useTextPaths
    static_cast<Msc*>(msc)->DrawToOutput(ot, fn);
}

void Msc_Destroy(void *msc)
{
    if (msc==NULL) return;
    delete static_cast<Msc*>(msc);
}

