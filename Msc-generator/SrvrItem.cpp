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
// SrvrItem.cpp : implementation of the CMscGenSrvrItem class
//

#include "stdafx.h"
//#include <afxpriv.h>    // for CSharedFile
#include "Msc-generator.h"

#include "MscGenDoc.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMscGenSrvrItem implementation

IMPLEMENT_DYNAMIC(CMscGenSrvrItem, CDocObjectServerItem)

CMscGenSrvrItem::CMscGenSrvrItem(CMscGenDoc* pContainerDoc)
	: CDocObjectServerItem(pContainerDoc, TRUE)
{
	//Add text format
	FORMATETC formatEtc;
	formatEtc.ptd = NULL;
	formatEtc.dwAspect = DVASPECT_CONTENT;
	formatEtc.lindex = -1;
	formatEtc.cfFormat = CF_TEXT;
	formatEtc.tymed = TYMED_HGLOBAL;
	m_dataSource.DelayRenderData(0, &formatEtc);
}

CMscGenSrvrItem::~CMscGenSrvrItem()
{
	// add cleanup code here
}

void CMscGenSrvrItem::Serialize(CArchive& ar)
{
	// CMscGenSrvrItem::Serialize will be called by the framework if
	//  the item is copied to the clipboard.  This can happen automatically
	//  through the OLE callback OnGetClipboardData.  A good default for
	//  the embedded item is simply to delegate to the document's Serialize
	//  function.  If you support links, then you will want to serialize
	//  just a portion of the document.

	if (!IsLinkedItem())
	{
		CMscGenDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
			pDoc->Serialize(ar);
	}
}

BOOL CMscGenSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// Most applications, like this one, only handle drawing the content
	//  aspect of the item.  If you wish to support other aspects, such
	//  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
	//  implementation of OnGetExtent should be modified to handle the
	//  additional aspect(s).

	if (dwDrawAspect != DVASPECT_CONTENT)
		return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CMscGenSrvrItem::OnGetExtent is called to get the extent in
	//  HIMETRIC units of the entire item.  The default implementation
	//  here simply returns a hard-coded number of units.

	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	rSize = pDoc->m_itrCurrent->GetSize(pDoc->m_page);

	CClientDC dc(NULL);
	// use a mapping mode based on logical units
	//    (we can't use MM_LOENGLISH because MM_LOENGLISH uses
	//    physical inches)
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetViewportExt(dc.GetDeviceCaps(LOGPIXELSX),
	dc.GetDeviceCaps(LOGPIXELSY));
	dc.SetWindowExt(100, -100);
	dc.LPtoHIMETRIC(&rSize);

	return TRUE;
}

BOOL CMscGenSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	if (!pDC)
		return FALSE;

	// Remove this if you use rSize
	UNREFERENCED_PARAMETER(rSize);

	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT_VALID(pApp);
	pDoc->m_itrCurrent->Draw(pDC->m_hDC, DRAW_WMF, 100, pDoc->m_page, pApp->m_bPB_Embedded);
	return TRUE;
}


// CMscGenSrvrItem diagnostics

#ifdef _DEBUG
void CMscGenSrvrItem::AssertValid() const
{
	CDocObjectServerItem::AssertValid();
}

void CMscGenSrvrItem::Dump(CDumpContext& dc) const
{
	CDocObjectServerItem::Dump(dc);
}
#endif

//Called by EditCopy and Drag
//Here we copy the native format then call GetClipboardData to do the rest.
COleDataSource* CMscGenSrvrItem::OnGetClipboardData(BOOL bIncludeLink, LPPOINT lpOffset, LPSIZE lpSize)
{
	ASSERT_VALID(this);

	COleDataSource* pDataSource = new COleDataSource;
	TRY
	{
		ASSERT(CMscGenDoc::m_cfPrivate != NULL);

		// Create a shared file and associate a CArchive with it
		CSharedFile file;
		CArchive ar(&file, CArchive::store);

		// Serialize selected objects to the archive
		Serialize(ar);
		ar.Close();

		// put on local format instead of or in addation to
		pDataSource->CacheGlobalData(CMscGenDoc::m_cfPrivate, file.Detach());
		// Call GetClipboardData. It will add CF_EMBEDDEDOBJECT, CF_OBJECTDESCRIPTOR and call AddOtherClipboardData.
		GetClipboardData(pDataSource, bIncludeLink, lpOffset, lpSize);
	}
	CATCH_ALL(e)
	{
		delete pDataSource;
		THROW_LAST();
	}
	END_CATCH_ALL

	ASSERT_VALID(pDataSource);
	return pDataSource;
}

//Called on EditCopy by AddOtherClipboardFormat for the formats registered in the Constructor
//The native 
BOOL CMscGenSrvrItem::OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));

	if (lpFormatEtc->cfFormat == CF_TEXT) {
		if (lpFormatEtc->lindex != -1)
			return FALSE; // does not support extended layout
		else
			return GetTextData(lpFormatEtc, lpStgMedium);
	} 
	return COleServerItem::OnRenderData(lpFormatEtc, lpStgMedium);
}

BOOL CMscGenSrvrItem::GetTextData(LPFORMATETC lpFormatEtc , LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));
	ASSERT(lpStgMedium->tymed == TYMED_NULL);   // GetDataHere not valid
	ASSERT(lpStgMedium->pUnkForRelease == NULL);

	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// medium must be TYMED_HGLOBAL and we must fill in 
	if (!(lpFormatEtc->tymed & TYMED_HGLOBAL) || lpStgMedium->hGlobal != NULL)
		return FALSE;

	CString text(pDoc->m_itrCurrent->GetText());
	lpStgMedium->tymed= TYMED_HGLOBAL;
	lpStgMedium->hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.GetLength()+1);
	if (lpStgMedium->hGlobal == NULL) 
		return FALSE;
	memcpy(GlobalLock(lpStgMedium->hGlobal), text, text.GetLength()+1);
	GlobalUnlock(lpStgMedium->hGlobal);
	return TRUE;
}


