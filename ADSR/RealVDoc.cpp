// ModelDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "RealVDoc.h"


// CRealVDoc

IMPLEMENT_DYNCREATE(CRealVDoc, CDocument)

CRealVDoc::CRealVDoc()
	:	m_TypeOfGraph(99)
	,	m_nECount(0)
	,	m_nRCount(0)
	,   m_PreViewRCtrlID(0)
	,   m_PreViewECtrlID(0)
	,	m_bType0dataReady(false)
	,   m_bType1dataReady(false)
	,   m_bType2dataReady(false)
{
}

BOOL CRealVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CRealVDoc::~CRealVDoc()
{
}


BEGIN_MESSAGE_MAP(CRealVDoc, CDocument)
END_MESSAGE_MAP()


// CRealVDoc diagnostics

#ifdef _DEBUG
void CRealVDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CRealVDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CRealVDoc serialization

void CRealVDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#endif


