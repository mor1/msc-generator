/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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
/** @file SrvrItem.cpp The interface for CMscGenSrvrItem class, which is providing
 * rendering and serialization of chart objects to OLE. Essentially this provides
 * the OLE server functions.
 * @ingroup Msc_generator_files */

#include "stdafx.h"
//#include <afxpriv.h>    // for CSharedFile
#include "Msc-generator.h"

#include "MscGenDoc.h"
#include "MainFrm.h"
#include "SrvrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMscGenSrvrItem implementation

IMPLEMENT_DYNAMIC(CMscGenSrvrItem, CDocObjectServerItem)

/** This server item object is created at two places, in 
 * CMscGenDoc::OnGetEmbeddedItem() and CMscGenDoc::OnGetLinkedItem.
 * In the first case we set forcePage to zero, since all of the chart is
 * contained in an embedded objects. In the second case we also use a page
 * since a linked item may point to one page only.*/
CMscGenSrvrItem::CMscGenSrvrItem(CMscGenDoc* pContainerDoc, unsigned forcePage)
	: CDocObjectServerItem(pContainerDoc, TRUE), m_forcePage(forcePage)
{
	//Add text format
	FORMATETC formatEtc;
	formatEtc.ptd = NULL;
	formatEtc.dwAspect = DVASPECT_CONTENT;
	formatEtc.lindex = -1;
	formatEtc.cfFormat = CF_TEXT;
	formatEtc.tymed = TYMED_HGLOBAL;
	m_dataSource.DelayRenderData(0, &formatEtc);

    char buff[100];
    sprintf(buff, "%u", forcePage);
    SetItemName(buff);
}

/** Depending on linking/embedding, returns the page to draw/extent 
 * That is, for a linked item we always return the forced page.
 * For embedded objects we take the current page of the shown chart.*/
unsigned CMscGenSrvrItem::GetPage() const
{
    if (IsLinkedItem()) return m_forcePage;
    CMscGenDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    return pDoc ? pDoc->m_ChartShown.GetPage() : 0;
}


/** CMscGenSrvrItem::Serialize will be called by the framework if
 * the item is copied to the clipboard.  This can happen automatically
 * through the OLE callback OnGetClipboardData.  A good default for
 * the embedded item is simply to delegate to the document's Serialize
 * function.  If you support links, then you will want to serialize
 * just a portion of the document.*/
void CMscGenSrvrItem::Serialize(CArchive& ar)
{
    CMscGenDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    unsigned fp = GetPage();
    pDoc->SerializePage(ar, fp);  //overwrite page stored in pDoc with fp, but keep m_forcedpage on read
}

/** Return the size of our chart.
 * Most applications, like this one, only handle drawing the content
 * aspect of the item.  If you wish to support other aspects, such
 * as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
 * implementation of OnGetExtent should be modified to handle the
 * additional aspect(s).
 * I did not provide thumbnails - that would be too much, really.*/
BOOL CMscGenSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{

	if (dwDrawAspect != DVASPECT_CONTENT)
		return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CMscGenSrvrItem::OnGetExtent is called to get the extent in
	//  HIMETRIC units of the entire item.  
    CMscGenDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc) return FALSE;
    rSize = pDoc->m_ChartShown.GetSize(pDoc->m_ChartShown.GetPage());

	CClientDC dc(NULL);
	// use a mapping mode based on logical units
	//    (we can't use MM_LOENGLISH because MM_LOENGLISH uses
	//    physical inches)
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetViewportExt(dc.GetDeviceCaps(LOGPIXELSX), dc.GetDeviceCaps(LOGPIXELSY));
	dc.SetWindowExt(100, -100);
	dc.LPtoHIMETRIC(&rSize);

	return TRUE;
}

/** Paint the relevant page (or all the document). 
 * This is the place where we draw, how a chart looks like in a container
 * document. (sigh) Unfortunately we get a WMF DC here and this is
 * 'by design' according to OLE documentation. So we cannot use
 * coordinates larger than 32K, shaded colors, rotated text, 
 * dashed lines and so on. Hence the huge workaround-pile in 
 * class Canvas.*/
BOOL CMscGenSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	if (!pDC)
		return FALSE;

	// Remove this if you use rSize
	UNREFERENCED_PARAMETER(rSize);    
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT_VALID(pApp);
    CMscGenDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc || !pApp) return FALSE;
    size_t size = pDoc->m_ChartShown.DrawToDC(Canvas::WMF,
        pDC->m_hDC, XY(1,1), pDoc->m_ChartShown.GetPage(), pApp->m_bPageBreaks, pApp->m_uFallbackResolution);
    size += pDoc->serialize_doc_overhead + pDoc->m_ChartShown.GetText().GetLength();
	return TRUE;
}


/** Called by EditCopy and Drag, when we drop our chart to some other app.
 * Here we copy the native format then call GetClipboardData to do the rest.*/
COleDataSource* CMscGenSrvrItem::OnGetClipboardData(BOOL bIncludeLink, LPPOINT lpOffset, LPSIZE lpSize)
{
	ASSERT_VALID(this);

	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->CompileEditingChart(false, true, false);
    COleDataSource* pDataSource = new COleDataSource;
    if (pDoc->m_ChartShown.GetErrorNum(false))
        if (IDYES == AfxMessageBox("The chart had errors, do you want to copy it nevertheless?", MB_ICONQUESTION | MB_OKCANCEL))
            return pDataSource;

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
        pDataSource = new COleDataSource;
		THROW_LAST();
	}
	END_CATCH_ALL

	ASSERT_VALID(pDataSource);
	return pDataSource;
}

/** Called on EditCopy by AddOtherClipboardFormat for the formats registered in the 
 * Constructor. Basically We copy our data (the chart text) to the clipboard,
 * if text is expected of us. Else NIL.*/
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

/** Copy the chart text to 'lpStgMedium.*/
BOOL CMscGenSrvrItem::GetTextData(LPFORMATETC lpFormatEtc , LPSTGMEDIUM lpStgMedium)
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(lpFormatEtc, sizeof(FORMATETC), FALSE));
	ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));
	ASSERT(lpStgMedium->tymed == TYMED_NULL);   // GetDataHere not valid
	ASSERT(lpStgMedium->pUnkForRelease == NULL);

	// medium must be TYMED_HGLOBAL and we must fill in 
	if (!(lpFormatEtc->tymed & TYMED_HGLOBAL) || lpStgMedium->hGlobal != NULL)
		return FALSE;

	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    //We always copy the editing text (even if a full screen read only doc)
	CString text(pDoc->m_itrEditing->GetText());
	lpStgMedium->tymed= TYMED_HGLOBAL;
	lpStgMedium->hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.GetLength()+1);
	if (lpStgMedium->hGlobal == NULL) 
		return FALSE;
	memcpy(GlobalLock(lpStgMedium->hGlobal), text, text.GetLength()+1);
	GlobalUnlock(lpStgMedium->hGlobal);
	return TRUE;
}

/** Do an OLE verb. This is where we support full-screen view only as verb #2.
 * Since in-place editing sucks and does not work very well with ribbons anyway,
 * If an in-place editing verb arrives, we just do OPEN.
 * We fill in the CMscGenDoc::serialize_doc_overhead here, since here
 * we know the both length of the full archive and the length of the text.
 * The actual object size is the archive size, plus the rendered WMF.*/
void CMscGenSrvrItem::OnDoVerb(LONG iVerb)
{
	CMscGenApp *pApp = dynamic_cast<CMscGenApp *>(AfxGetApp());
	ASSERT_VALID(pApp);
    if (pApp==NULL) return; //WTF
	CMscGenDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    if (pDoc==NULL) return; //WTF
	CMainFrame *pMainFrame = dynamic_cast<CMainFrame *>(pDoc->GetFirstFrame());

    //Calculate how many bytes are added by the serialize system to the
    //chart text
    CMemFile mem;
    {
        CArchive ar(&mem, CArchive::store);
        Serialize(ar);
    }
    pDoc->serialize_doc_overhead = mem.GetLength();
    if (pDoc->m_ChartShown.GetText().GetLength() < pDoc->serialize_doc_overhead)
        pDoc->serialize_doc_overhead -= pDoc->m_ChartShown.GetText().GetLength();
    else 
        pDoc->serialize_doc_overhead = 0;

    if (iVerb == 2) {
		//If the embedded object has been activated with this verb (Full Screen View)
		//The CMainFrame is already loaded with CMainFrame::LoadFrame, but is not shown yet
		if (pMainFrame) {
			//Signal to MscGenDoc that chart is read only and "nothing" works, just viewing
			pApp->m_bFullScreenViewMode = true;
			//Switch to full screen
			pMainFrame->ShowFullScreen();
            pMainFrame->AddToFullScreenToolbar();
			if (pApp->IsInternalEditorRunning()) 
                pApp->m_pWndEditor->SetReadOnly(true);
			//Finally show the CMainFrame Window
			CDocObjectServerItem::OnDoVerb(1); //Open
			//Hide menu, seems to be needed, or else a native menu appears
			pMainFrame->SetMenu(NULL); 
			return;
		}
		//We could not do full screen, do nothing
        return;
	} else {
		pApp->m_bFullScreenViewMode = false;
		//Avoid in-place editing 
		switch (iVerb) {
		case OLEIVERB_PRIMARY: //primary verb is edit (in-place)
		case OLEIVERB_SHOW:    //this is the show (in-place edit command ==-1)
        case OLEIVERB_INPLACEACTIVATE:
        case OLEIVERB_OPEN:
            iVerb = 1; //The secondary verb, which is open
		}
        //Show embedded objects context category on the ribbon
        if (iVerb==1 && pMainFrame) {
            //size_t size = pDoc->serialize_doc_overhead + pDoc->m_ChartShown.GetText().GetLength();
            //HDC hdc = CreateMetaFile(NULL);
            //size += pDoc->m_ChartShown.DrawToMetafile(hdc, Canvas::WMF, pApp->m_bPageBreaks, true, GetPage());
            //DeleteMetaFile(CloseMetaFile(hdc));
            //pMainFrame->FillEmbeddedSizeNow(size);
            pMainFrame->m_wndRibbonBar.ShowContextCategories(ID_CONTEXT_EMBEDDEDOPTIONS);            
            pMainFrame->m_wndRibbonBar.RecalcLayout();
        }

		CDocObjectServerItem::OnDoVerb(iVerb);
	}

}
