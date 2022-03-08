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

// CChildFrame 생성/소멸

CRealFrame::CRealFrame()

{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CRealFrame::~CRealFrame()
{

}

BOOL CRealFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~(LONG)FWS_ADDTOTITLE;

	// 타이틀 생성

	return TRUE;
}

// CChildFrame 진단

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

// CChildFrame 메시지 처리기

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

	// 변경된 클라이언트 좌표 구함

}
