
// ChildFrm.cpp : CChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "ADSR.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

int CChildFrame::m_staticNumbering=0;

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CLOSE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame()

{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.

	m_iMyNumber=m_staticNumbering++;

}

CChildFrame::~CChildFrame()
{
	m_staticNumbering--;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~(LONG)FWS_ADDTOTITLE;

	// 타이틀 생성
	CString t_strTitle;
	t_strTitle.Format(_T(" : 응답채널 #%d"),m_iMyNumber);
	t_strTitle = m_strTitle + t_strTitle;
	// Convert to a char*
	const size_t newsize = 100;
	char t_chTitle[newsize];
	strcpy_s(t_chTitle, t_strTitle);

	cs.lpszName = t_chTitle;

	return TRUE;
}

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기

void CChildFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CMDIChildWndEx::OnClose();
}


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CMenu *pSysMenu = GetSystemMenu(FALSE);
	ASSERT(pSysMenu != NULL);
	VERIFY(pSysMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND));

	return 0;
}

CString CChildFrame::m_strTitle="Title";
