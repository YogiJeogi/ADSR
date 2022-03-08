
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "ADSR.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "RealVDoc.h"
#include "ResultDoc.h"
#include "../ADSRAlgorithms/SI_class.h"
#include "ResultView.h"
#include <afx.h>
#include "RealFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_MESSAGE(WM_HD_TF,&CMainFrame::OnRealVCalled)
	ON_MESSAGE(WM_HD_RTV,&CMainFrame::OnResultViewCalled)
	ON_MESSAGE(WM_HD_UP,&CMainFrame::OnUpDateDoc)	
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
:pResultVDoc(NULL), pRealVDoc(NULL)
, m_fIsResultTemplate(false)
, m_fIsRealTemplate(false)
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	m_bIsFirstResp=true;
	m_taskRunning=false;

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

//	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("�޴� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// �޴� ������ Ȱ��ȭ�ص� ��Ŀ���� �̵����� �ʰ� �մϴ�.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	// ==================== ���� �κ� ���� ====================
	// ==================== ���� �κ� ���� ====================

// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
// 	{
// 		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
// 		return -1;      // ������ ���߽��ϴ�.
// 	}

// 	CString strToolBarName;
// 	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
// 	ASSERT(bNameValid);
// 	m_wndToolBar.SetWindowText(strToolBarName);

// 	CString strCustomize;
// 	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
// 	ASSERT(bNameValid);
// 	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// ����� ���� ���� ���� �۾��� ����մϴ�.
/*	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);*/

	// ==================== ���� �κ� ���� ====================
	// ==================== ���� �κ� ���� ====================


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� �Ѱ�  �޴� ����
	m_wndMenuBar.SetShowAllCommands();
	// TODO: ���� ���� �� �޴� ������ ��ŷ�� �� ���� �Ϸ��� �� �ټ� ���� �����Ͻʽÿ�.
// 	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
// 	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
/*	DockPane(&m_wndToolBar);*/


	// Visual Studio 2005 ��Ÿ�� ��ŷ â ������ Ȱ��ȭ�մϴ�.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 ��Ÿ�� ��ŷ â �ڵ� ���� ������ Ȱ��ȭ�մϴ�.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���� â ���� ��ȭ ���ڸ� Ȱ��ȭ�մϴ�.
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

// 	// ���� ���� �� ��ŷ â �޴� �ٲٱ⸦ Ȱ��ȭ�մϴ�.
	// ==================== ���� �κ� ���� ====================
	// ==================== ���� �κ� ���� ====================
// 	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);
	// ==================== ���� �κ� ���� ====================
	// ==================== ���� �κ� ���� ====================

	// ����(<Alt> Ű�� ���� ä ����) ���� ���� ����� ������ Ȱ��ȭ�մϴ�.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// ����� ���� ���� ���� �̹����� �ε��մϴ�.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// �޴� ���� ������ Ȱ��ȭ�մϴ�(���� �ֱٿ� ����� ���).
	// TODO: ������� �⺻ ����� �����Ͽ� �� Ǯ�ٿ� �޴��� �ϳ� �̻��� �⺻ ����� ���Խ�ŵ�ϴ�.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust;
	pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* �޴��� �˻��մϴ�. */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();

}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource
						 , DWORD dwDefaultStyle
						 , CWnd* pParentWnd
						 , CCreateContext* pContext) 
{
	// �⺻ Ŭ������ ���� �۾��� �����մϴ�.

	if (!CMDIFrameWndEx::LoadFrame(nIDResource
								 , dwDefaultStyle
								 , pParentWnd
								 , pContext))
	{
		return FALSE;
	}


	// ��� ����� ���� ������ ����� ���� ���߸� Ȱ��ȭ�մϴ�.
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

LRESULT CMainFrame::OnRealVCalled( WPARAM wp, LPARAM lp )
{

	// WPARAM : 0 FRF;
	//        : 1 Ordinary Coherence
	//        : 2 Multiple Coherence
	// LPARAM : Number of Windows
	// LPARAM : 0 Close Window
	// 
/*
	pMainFrame->SendMessage( WM_HD_TF
							,MAKEWPARAM(m_nTypeGraph,0)
							,MAKELPARAM(nECount,nRCount) );
*/
	int TypeOfGraph=LOWORD(wp);
	int TypeOfView=HIWORD(wp);  // �̰� 0�̸� ����
	int nECount=LOWORD(lp);  
	int nRCount=HIWORD(lp);

	if ( TypeOfView == 0 ) {CloseRealViewWindows(); return 0;}

	CreatRealViewWindows(nECount, nRCount, TypeOfGraph);

	MDITile(MDITILE_HORIZONTAL);
	pRealVDoc->UpdateAllViews(NULL);
	return 0;

}


LRESULT CMainFrame::OnResultViewCalled( WPARAM wp, LPARAM lp )
{

	// �޼��� ����
	// WPARAM LOWORD : ���õǴ� ��ȣ Ÿ���� ��Ÿ����.
	//				 : 1 -> ���� ��ȣ
	//				 : 2 -> ���� �Լ�
	//				 : 3 -> ��ȣ ��
	// WPARAM HIWORD : ���õ� ��ȣ�� ��Ÿ����.
	//				 : 100���� Zero Index
	//				 : 200����
	//				 : 300����
	// LPARAM LOWORD : CheckBox���� ���õ� ���� Bit����Ǿ��ִ�.
	// LPARAM HIWORD : 0 -> â �ݱ�
	//				 : 1 -> â �����ϱ�
	//				 : 2 -> ����Ʈ ���� �����ϰ� ��ȣ�� ���õ� ���â ����
	//				 : 3 -> üũ ��ư ��ȭ�� ���� â������Ʈ �Ҷ�
	//				 : 4 -> ��ȭ���ڿ��� ALL Close��ư�� ��������.
/*
AfxGetMainWnd()->SendMessage(WM_HD_RTV
						,MAKEWPARAM(TYPE,nSelectedSignal)
						,MAKELPARAM(nCheckedViewMode,true));
*/

	int TYPE=LOWORD(wp);
	int nSelectedSignal=HIWORD(wp);
	int nCheckedViewMode=LOWORD(lp);
	int bUpdate=HIWORD(lp);
	

/* ==========================
 * ���� �϶�� ���ó��
 * ========================*/
	// �����̸� ������. 
	// �� â�� �־��ٸ� �ռ� ��Ǫ���� ������ ���ᰡ �ȴ�.
	if ( bUpdate==0 ) 
	{   
		CloseResultViewWindows();
		MDITile(MDITILE_HORIZONTAL); 
		return 0;
	}
	if ( bUpdate==4 ) 
	{   
		CloseALLResultViewWindows();
		/*MDITile(MDITILE_HORIZONTAL); */
		return 0;
	}

/* ==========================
 * ������ �����Ѵ�.
 * ========================*/
	CreatResultViewWindow(TYPE ,nSelectedSignal
						 ,nCheckedViewMode,bUpdate);


/* ==========================
 * ��� â�� �����ϴ� �κ��̴�.
 * ========================*/
/*	MDITile(MDITILE_HORIZONTAL); // MDIFrameWnd::MDITile(int nType);*/


/* ==========================
 * ��ɾ ó���ϴ� �κ��̴�.
 * ========================*/

	// �� ��ť��Ʈ�� �ڵ� ȹ��.
	// Ÿ��Ʋ ����
    //	pCTimeDoc->ModifyFrameTitle(int(wp));

	// TypeOfSignla = 1 �� ȭ��Ʈ ����� ��Ÿ����.
// 	if (TypeOfSignal == 1 )
// 	{
// 		pResultVDoc->GetTimedata(theApp.m_SIWhiteNoiseTimeData);
// 		pResultVDoc->UpdateAllViews(NULL);
// 	}
// WPARAM�� ���� ����Ÿ ��� 
	return 0;
}

void CMainFrame::CloseALLResultViewWindows()
{
/* ==========================
 * ��ȭ���ڿ��� ALL close��ư�� ���������� ó�� �Լ�
 * ========================*/
	// ���ϵ� �����찡 ������ �ִٸ� TimeView ���������� �Ǻ�
	// �ִٸ� ���� ��Ų��.
	if( pResultVDoc ) 
	{
		pResultVDoc->OnCloseDocument();
	}
	pResultVDoc=NULL;

	// �ٽ� ��ť��Ʈ�� �����Ѵ�.
	if ( m_fIsResultTemplate == true )
	{
	pDocument = pResultVTemplate->CreateNewDocument();
	pResultVDoc=(CResultVDoc*)pDocument;
	}

}
void CMainFrame::CloseResultViewWindows()
{

/* ==========================
 * ���� â�� �ݴ� �κ��̴�.
 * ========================*/
	// ���ϵ� �����찡 ������ �ִٸ� TimeView ���������� �Ǻ�
	// �ִٸ� ���� ��Ų��.
	if( pResultVDoc ) 
	{
		pResultVDoc->OnCloseDocument();
	}
	pResultVDoc=NULL;

	// �ٽ� ��ť��Ʈ�� �����Ѵ�.
	if ( m_fIsResultTemplate == true )
	{
	pDocument = pResultVTemplate->CreateNewDocument();
	pResultVDoc=(CResultVDoc*)pDocument;
	}

	// RealView�����츦 �ٽ� ��Ÿ����.
	if ( m_fIsRealTemplate == true )
	{
		POSITION Pos=pRealVTemplate->GetFirstDocPosition();
		if ( Pos != NULL )
		{
			CDocument* pDoc=pRealVTemplate->GetNextDoc(Pos);
			POSITION PosV=pDoc->GetFirstViewPosition();
			while(PosV)
			{
				pDoc->GetNextView(PosV)->GetParentFrame()->ShowWindow(SW_SHOW);
			}
		}
	}
}

void CMainFrame::CreatResultViewWindow(int TYPE, int nSelectedSignal, int nCheckedViewMode ,int UpdateWnd)
{

/* ==================================
 *  RealView �����찡 ������� �����.
 * =================================*/
	if ( m_fIsRealTemplate == true )
	{
		POSITION Pos=pRealVTemplate->GetFirstDocPosition();
		if ( Pos != NULL )
		{
			CDocument* pDoc=pRealVTemplate->GetNextDoc(Pos);
			POSITION PosV=pDoc->GetFirstViewPosition();
			while(PosV)
			{
				pDoc->GetNextView(PosV)->GetParentFrame()->ShowWindow(SW_HIDE);
			}
		}
	}
/* ==========================
 *  ���ο� �����츦 �����Ѵ�
 * ========================*/

// 1. ���ø� ã�´�.
	if ( m_fIsResultTemplate == false )
	{
		POSITION ResultVTemplatePos = AfxGetApp()->GetFirstDocTemplatePosition();
		while(ResultVTemplatePos != NULL)
		{
			CString str;
			pResultVTemplate = AfxGetApp()->GetNextDocTemplate(ResultVTemplatePos);
			pResultVTemplate->GetDocString(str, CDocTemplate::docName);
					
			if(str == "ResultView")
				break;
		}
		m_fIsResultTemplate=true;
	}

// 2. ��ť��Ʈ ����
	POSITION Pos = pResultVTemplate->GetFirstDocPosition();
	if (Pos == NULL )
	{
		pDocument = pResultVTemplate->CreateNewDocument();
		pResultVDoc=(CResultVDoc*)pDocument;
	}

	//// ���ø� ��ť��Ʈ ���� �� ////////////////////////////

	// ������ �������� �� �ڵ�� ��ť��Ʈ �ڵ� ȹ��
	pActiveChild = MDIGetActive();

// 3. ��ť��Ʈ�� ��ȣ Ÿ��, ��� �� �ӽ� ���� (�信�� ����Ұ���)

	// ��ť��Ʈ�� ���� �ӽ� ����.
	pResultVDoc->nTYPE=TYPE;
	pResultVDoc->nSelectedSignal=nSelectedSignal;
	pResultVDoc->nCheckedViewMode=nCheckedViewMode;

	// Ÿ���� ���� ��ȣ or TS �����϶�.
	pResultVDoc->m_PreViewCtrlID1=0;
	pResultVDoc->m_PreViewCtrlID2=0;

// 4. â ���� ��ȣ�� Ÿ�Կ� ���� ���� �����ȴ�.
	unsigned int MaxiIndex=g_sVUserData.RTV.m_nNSelectedCh1;
	unsigned int MaxJindex=g_sVUserData.RTV.m_nNSelectedCh2;
	
	if ( TYPE==1 || TYPE==3)
	{
		ResultViewType1DataLoading();
		ResultViewType3DataLoading();
		for( unsigned int iIndex=0; iIndex < MaxiIndex ; iIndex++ )
		{	
			// �����Ӱ� �並 ���� �� �����Ѵ�. �ٸ� OnUpdate�� ����.
			pFrame = pResultVTemplate->CreateNewFrame(pResultVDoc,pActiveChild);
				if (pFrame == NULL)
				{
					TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
					return ;     // command failed
				}

				// �� �ε��� �����κ��̴�.
				pResultVDoc->m_PreViewCtrlID1=g_sVUserData.RTV.m_anSelectedCh1[iIndex];
				// ��ť��Ʈ �����Ӱ� �並 ��ť��Ʈ�� ���δ�. OnUpdate���� �ִ�.
				pResultVTemplate->InitialUpdateFrame(pFrame,pResultVDoc,TRUE /* Visible */);
		}
	}
	else 
	{
		ResultViewType2DataLoading();
		if ( nSelectedSignal == 200 /*System Model H*/ )
		{
			for( unsigned int iIndex=0; iIndex < MaxiIndex ; iIndex++ )
				for( unsigned int jIndex=0; jIndex < MaxJindex ; jIndex++ )	
					{	
						// �����Ӱ� �並 ���� �� �����Ѵ�. �ٸ� OnUpdate�� ����.
						pFrame = pResultVTemplate->CreateNewFrame(pResultVDoc,pActiveChild);
						if (pFrame == NULL)
						{
							TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
							return ;     // command failed
						}

						// �� �ε��� �����κ��̴�.
						pResultVDoc->m_PreViewCtrlID1=g_sVUserData.RTV.m_anSelectedCh1[iIndex];
						pResultVDoc->m_PreViewCtrlID2=g_sVUserData.RTV.m_anSelectedCh2[jIndex];
						// ��ť��Ʈ �����Ӱ� �並 ��ť��Ʈ�� ���δ�. OnUpdate���� �ִ�.
						pResultVTemplate->InitialUpdateFrame(pFrame,pResultVDoc,TRUE /* Visible */);
					}
		}
		else
		{
			// �̿�
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
void CMainFrame::CloseRealViewWindows(void)
{
	if( pRealVDoc ) 
	{
		pRealVDoc->OnCloseDocument();
	}
	pRealVDoc=NULL;

	if ( m_fIsRealTemplate == true )
	{
	// �ٽ� ��ť��Ʈ�� �����Ѵ�.
	pDocument = pRealVTemplate->CreateNewDocument();
	pRealVDoc=(CRealVDoc*)pDocument;
	}
}

void CMainFrame::CreatRealViewWindows(int nE, int nR, int TypeOfGraph)
{
	
	// ���ø��� ó�� ��� ���̴�.
	// �̹� ����Ǿ��ٸ� ��ŵ�ȴ�. m_bIsRealTemplate
	if ( m_fIsRealTemplate==false)
	{
//		AfxMessageBox("������ â�� �������� �ʾƼ� ���ø��� ���ϴ� �����̴�.");
		POSITION RealVTemplatePos = AfxGetApp()->GetFirstDocTemplatePosition();
		while(RealVTemplatePos != NULL)
		{
			CString str;
			pRealVTemplate = AfxGetApp()->GetNextDocTemplate(RealVTemplatePos);
			pRealVTemplate->GetDocString(str, CDocTemplate::docName);
			/*AfxMessageBox(str);*/
			if(str == "RealView")
				break;
		}
		m_fIsRealTemplate=true;
	}
	//// ���ø� ��ť��Ʈ ���� ���� ////////////////////////////
	POSITION Pos = pRealVTemplate->GetFirstDocPosition();
	if (Pos == NULL )
	{
		pDocument = pRealVTemplate->CreateNewDocument();
		pRealVDoc=(CRealVDoc*)pDocument;
 	}
	//// ���ø� ��ť��Ʈ ���� �� ////////////////////////////

	// ������ �������� �� �ڵ�� ��ť��Ʈ �ڵ� ȹ��
	pActiveChild = MDIGetActive();
 
    pRealVDoc->m_TypeOfGraph=TypeOfGraph;
  	pRealVDoc->m_nECount=nE;
  	pRealVDoc->m_nRCount=nR;

	// Ÿ���� ���� ��ȣ or TS �����϶�.
	if ( TypeOfGraph == 0  )
	{		
		pRealVDoc->m_PreViewRCtrlID=0;
		for( int iIndex=0; iIndex < nR ; iIndex++ )
		{	
			// �����Ӱ� �並 ���� �� �����Ѵ�. �ٸ� OnUpdate�� ����.
			pFrame = pRealVTemplate->CreateNewFrame(pRealVDoc,pActiveChild /*pActiveChild*//*pActiveChild*/);
			if (pFrame == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
				return ;     // command failed
			}
			
			// �� �ε��� �����κ��̴�.
			pRealVDoc->m_PreViewRCtrlID=iIndex;
			// ��ť��Ʈ �����Ӱ� �並 ��ť��Ʈ�� ���δ�. OnUpdate���� �ִ�.
			pRealVTemplate->InitialUpdateFrame(pFrame,pRealVDoc,TRUE /* Visible */);
		}
	}

	// Ÿ���� SI TF ���� �϶�
	if ( TypeOfGraph == 1 )
	{
		pRealVDoc->m_PreViewRCtrlID=0;
		for( int EIndex=0; EIndex < nE ; EIndex++ )
		{
			for( int RIndex=0; RIndex < nR ; RIndex++ )
			{	
				// �����Ӱ� �並 ���� �� �����Ѵ�. �ٸ� OnUpdate�� ����.
				pFrame = pRealVTemplate->CreateNewFrame(pRealVDoc,pActiveChild /*pActiveChild*//*pActiveChild*/);
				if (pFrame == NULL)
				{
					TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
					return ;     // command failed
				}

				// �� �ε��� �����κ��̴�.
				pRealVDoc->m_PreViewRCtrlID=RIndex;
				pRealVDoc->m_PreViewECtrlID=EIndex;
				// ��ť��Ʈ �����Ӱ� �並 ��ť��Ʈ�� ���δ�. OnUpdate���� �ִ�.
				pRealVTemplate->InitialUpdateFrame(pFrame,pRealVDoc,TRUE /* Visible */);
			}
		}

	}

	// Ÿ���� TS �϶�
	if ( TypeOfGraph == 2 )
	{
		pRealVDoc->m_PreViewRCtrlID=0;
		for( int iIndex=0; iIndex < nR ; iIndex++ )
		{	
			// �����Ӱ� �並 ���� �� �����Ѵ�. �ٸ� OnUpdate�� ����.
			pFrame = pRealVTemplate->CreateNewFrame(pRealVDoc,pActiveChild /*pActiveChild*//*pActiveChild*/);
			if (pFrame == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
				return ;     // command failed
			}

			// �� �ε��� �����κ��̴�.
			pRealVDoc->m_PreViewRCtrlID=iIndex;
			// ��ť��Ʈ �����Ӱ� �並 ��ť��Ʈ�� ���δ�. OnUpdate���� �ִ�.
			pRealVTemplate->InitialUpdateFrame(pFrame,pRealVDoc,TRUE /* Visible */);
		}
	}
}

LRESULT CMainFrame::OnUpDateDoc( WPARAM wp, LPARAM lp )
{
//	theApp.System.SI.TakeResponseSignals(m_SIreaddata
//									   ,m_bIsFirstResp);
//									   
	int TypeOfSYSTEM=LOWORD(wp);


	if (pRealVDoc != NULL )
	{
		g_CriticalSection.Lock();
		// SI, TS ���� �ð� ����Ÿ.
		pRealVDoc->m_RespData=m_readdata;
		g_CriticalSection.Unlock();
		
		if (TypeOfSYSTEM==0) // SI
		{
			//System.SI.CalculatePSD();
			System.SI.CalculateH();

			if ( System.SI.m_bIsHReady==true)
			{
				System.SI.GetHAmplitudeModel(pRealVDoc->m_HAmplitude
											,pRealVDoc->m_HAngle);
				pRealVDoc->m_bType1dataReady=true;
			}
		}
		else if (TypeOfSYSTEM==1) // TS
		{
			System.TS.RealResponseProcessing();
			if ( System.TS.m_fIsRespPSDReady==true)
			{
				pRealVDoc->m_matTSTargetPSD=System.TS.GetPSD3DDesiredSignals();
				pRealVDoc->m_matTSRespPSD=System.TS.GetRealTimePSDRespSignals();
				pRealVDoc->m_bType2dataReady=true;
			}
		}
		pRealVDoc->m_bType0dataReady=true;
		pRealVDoc->UpdateAllViews(NULL);

	}
	return 0;
}

void CMainFrame::ResultViewType1DataLoading(void)
{
	if ( pResultVDoc!=NULL)
	{
		if(System.SI.m_bIsWhiteNoiseReady==true)
			pResultVDoc->m_matSIWhiteNoise=System.SI.GetWhiteNoise();
		if(System.SI.m_bIsRespDataFull==true)
			pResultVDoc->m_matSIResponse=System.SI.GetResponse();
		if(System.TS.m_fIsDriveSignalsReady==true)
			pResultVDoc->m_matTSDrive=System.TS.GetDriveSignals();

		if(System.TS.m_fIsTSInitialized==true)
		{
			pResultVDoc->m_matTSDesiredSignals=System.TS.GetDesiredSignals();
			pResultVDoc->m_matTSTargetPSD=System.TS.GetPSD3DDesiredSignals();
		}
		if(System.TS.m_fIsRespDataFull==true)
		{	
			pResultVDoc->m_matTSResponseSignals=System.TS.GetResponseSignals();
			pResultVDoc->m_matTSRespPSD=System.TS.GetPSD3DResponseSignals();
		}
	}
}

void CMainFrame::ResultViewType2DataLoading(void)
{
	if ( pResultVDoc!=NULL)
	{
		if(System.SI.m_bIsHReady==true)
		System.SI.GetHAmplitudeModel
				(pResultVDoc->m_HAmplitude,pResultVDoc->m_HAngle);
	}
}

void CMainFrame::ResultViewType3DataLoading(void)
{
	if ( pResultVDoc!=NULL)
	{
		if(System.TS.m_fIsTSInitialized==true)
		{
			pResultVDoc->m_matTSDesiredSignals=System.TS.GetDesiredSignals();
			pResultVDoc->m_matTSTargetPSD=System.TS.GetPSD3DDesiredSignals();
		}
		if(System.TS.m_fIsRespDataFull==true)
		{	
			pResultVDoc->m_matTSResponseSignals=System.TS.GetResponseSignals();
			pResultVDoc->m_matTSRespPSD=System.TS.GetPSD3DResponseSignals();
		}
	}
}
