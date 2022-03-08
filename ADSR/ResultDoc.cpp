// TimeDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "ResultDoc.h"


// CResultVDoc

IMPLEMENT_DYNCREATE(CResultVDoc, CDocument)

CResultVDoc::CResultVDoc()
: m_PreViewCtrlID1(0)
{
	m_bWhiteDataReady=false;
}

BOOL CResultVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CResultVDoc::~CResultVDoc()
{
}


BEGIN_MESSAGE_MAP(CResultVDoc, CDocument)
END_MESSAGE_MAP()


// CResultVDoc diagnostics

#ifdef _DEBUG
void CResultVDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CResultVDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CResultVDoc serialization

void CResultVDoc::Serialize(CArchive& ar)
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

void CResultVDoc::GetTimedata( MatDoub &t_TimeMatrix )
{
	int nn=t_TimeMatrix.nrows();
	int mm=t_TimeMatrix.ncols();

	m_TimeMatrix.Resize(nn,mm);

	for ( int i=0 ; i < nn ; i++)
		for (int j=0; j < mm ; j++)
			m_TimeMatrix(i,j)=t_TimeMatrix[i][j];

	m_bWhiteDataReady=true;


}

#endif


// CResultVDoc commands
