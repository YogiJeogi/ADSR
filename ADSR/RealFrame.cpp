// RealFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "RealFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame


IMPLEMENT_DYNCREATE(CRealFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CRealFrame, CMDIChildWndEx)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildFrame ����/�Ҹ�

CRealFrame::CRealFrame()

{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CRealFrame::~CRealFrame()
{

}

BOOL CRealFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~(LONG)FWS_ADDTOTITLE;

	// Ÿ��Ʋ ����

	return TRUE;
}

// CChildFrame ����

#ifdef _DEBUG
void CRealFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CRealFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame �޽��� ó����

void CRealFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnClose();
}


int CRealFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CRealFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWndEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	// ����� Ŭ���̾�Ʈ ��ǥ ����

}
