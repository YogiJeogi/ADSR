
// ChildFrm.cpp : CChildFrame Ŭ������ ����
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

// CChildFrame ����/�Ҹ�

CChildFrame::CChildFrame()

{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.

	m_iMyNumber=m_staticNumbering++;

}

CChildFrame::~CChildFrame()
{
	m_staticNumbering--;
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.style &= ~(LONG)FWS_ADDTOTITLE;

	// Ÿ��Ʋ ����
	CString t_strTitle;
	t_strTitle.Format(_T(" : ����ä�� #%d"),m_iMyNumber);
	t_strTitle = m_strTitle + t_strTitle;
	// Convert to a char*
	const size_t newsize = 100;
	char t_chTitle[newsize];
	strcpy_s(t_chTitle, t_strTitle);

	cs.lpszName = t_chTitle;

	return TRUE;
}

// CChildFrame ����

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

// CChildFrame �޽��� ó����

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
