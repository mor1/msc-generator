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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
// SrvrItem.h : interface of the CMscGenSrvrItem class
//

#pragma once

class CMscGenSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CMscGenSrvrItem)

// Constructors
public:
	CMscGenSrvrItem(CMscGenDoc* pContainerDoc);

// Attributes
	CMscGenDoc* GetDocument() const
		{ return reinterpret_cast<CMscGenDoc*>(CDocObjectServerItem::GetDocument()); }

// Overrides
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium);
	virtual COleDataSource* OnGetClipboardData(BOOL bIncludeLink, LPPOINT lpOffset, LPSIZE lpSize);
	BOOL GetTextData(LPFORMATETC lpFormatEtc , LPSTGMEDIUM lpStgMedium);
	BOOL GetBitmapData(LPFORMATETC lpFormatEtc , LPSTGMEDIUM lpStgMedium);

// Implementation
public:
	~CMscGenSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

