
// MainFrm.cpp : CMainFrame 클래스의 구현
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
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
:pResultVDoc(NULL), pRealVDoc(NULL)
, m_fIsResultTemplate(false)
, m_fIsRealTemplate(false)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
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
		TRACE0("메뉴 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 메뉴 모음을 활성화해도 포커스가 이동하지 않게 합니다.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	// ==================== 툴바 부분 삭제 ====================
	// ==================== 툴바 부분 삭제 ====================

// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
// 	{
// 		TRACE0("도구 모음을 만들지 못했습니다.\n");
// 		return -1;      // 만들지 못했습니다.
// 	}

// 	CString strToolBarName;
// 	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
// 	ASSERT(bNameValid);
// 	m_wndToolBar.SetWindowText(strToolBarName);

// 	CString strCustomize;
// 	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
// 	ASSERT(bNameValid);
// 	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 사용자 정의 도구 모음 작업을 허용합니다.
/*	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);*/

	// ==================== 툴바 부분 삭제 ====================
	// ==================== 툴바 부분 삭제 ====================


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 내가 한것  메뉴 변경
	m_wndMenuBar.SetShowAllCommands();
	// TODO: 도구 모음 및 메뉴 모음을 도킹할 수 없게 하려면 이 다섯 줄을 삭제하십시오.
// 	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
// 	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
/*	DockPane(&m_wndToolBar);*/


	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 향상된 창 관리 대화 상자를 활성화합니다.
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

// 	// 도구 모음 및 도킹 창 메뉴 바꾸기를 활성화합니다.
	// ==================== 툴바 부분 삭제 ====================
	// ==================== 툴바 부분 삭제 ====================
// 	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);
	// ==================== 툴바 부분 삭제 ====================
	// ==================== 툴바 부분 삭제 ====================

	// 빠른(<Alt> 키를 누른 채 끌기) 도구 모음 사용자 지정을 활성화합니다.
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 사용자 정의 도구 모음 이미지를 로드합니다.
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 메뉴 개인 설정을 활성화합니다(가장 최근에 사용한 명령).
	// TODO: 사용자의 기본 명령을 정의하여 각 풀다운 메뉴에 하나 이상의 기본 명령을 포함시킵니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	return TRUE;
}

// CMainFrame 진단

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


// CMainFrame 메시지 처리기

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust;
	pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
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
	// 기본 클래스가 실제 작업을 수행합니다.

	if (!CMDIFrameWndEx::LoadFrame(nIDResource
								 , dwDefaultStyle
								 , pParentWnd
								 , pContext))
	{
		return FALSE;
	}


	// 모든 사용자 도구 모음에 사용자 지정 단추를 활성화합니다.
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
	int TypeOfView=HIWORD(wp);  // 이게 0이면 종료
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

	// 메세지 전송
	// WPARAM LOWORD : 선택되는 신호 타입을 나타낸다.
	//				 : 1 -> 단일 신호
	//				 : 2 -> 전달 함수
	//				 : 3 -> 신호 비교
	// WPARAM HIWORD : 선택된 신호를 나타낸다.
	//				 : 100번대 Zero Index
	//				 : 200번대
	//				 : 300번대
	// LPARAM LOWORD : CheckBox에서 선택된 것이 Bit연산되어있다.
	// LPARAM HIWORD : 0 -> 창 닫기
	//				 : 1 -> 창 생성하기
	//				 : 2 -> 리스트 선택 무시하고 신호와 관련된 모든창 생성
	//				 : 3 -> 체크 버튼 변화에 따른 창업데이트 할때
	//				 : 4 -> 대화상자에서 ALL Close버튼을 눌렀을때.
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
 * 종료 하라는 명령처리
 * ========================*/
	// 종료이면 나간다. 
	// 즉 창이 있었다면 앞선 루푸에서 닫히고 종료가 된다.
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
 * 윈도우 생성한다.
 * ========================*/
	CreatResultViewWindow(TYPE ,nSelectedSignal
						 ,nCheckedViewMode,bUpdate);


/* ==========================
 * 모든 창을 정렬하는 부분이다.
 * ========================*/
/*	MDITile(MDITILE_HORIZONTAL); // MDIFrameWnd::MDITile(int nType);*/


/* ==========================
 * 명령어를 처리하는 부분이다.
 * ========================*/

	// 현 도큐먼트의 핸들 획득.
	// 타이틀 변경
    //	pCTimeDoc->ModifyFrameTitle(int(wp));

	// TypeOfSignla = 1 은 화이트 노이즈를 나타낸다.
// 	if (TypeOfSignal == 1 )
// 	{
// 		pResultVDoc->GetTimedata(theApp.m_SIWhiteNoiseTimeData);
// 		pResultVDoc->UpdateAllViews(NULL);
// 	}
// WPARAM에 따라서 데이타 출력 
	return 0;
}

void CMainFrame::CloseALLResultViewWindows()
{
/* ==========================
 * 대화상자에서 ALL close버튼을 눌렀을때의 처리 함수
 * ========================*/
	// 차일드 윈도우가 기존에 있다면 TimeView 윈도우인지 판별
	// 있다면 종료 시킨다.
	if( pResultVDoc ) 
	{
		pResultVDoc->OnCloseDocument();
	}
	pResultVDoc=NULL;

	// 다시 도큐먼트를 생성한다.
	if ( m_fIsResultTemplate == true )
	{
	pDocument = pResultVTemplate->CreateNewDocument();
	pResultVDoc=(CResultVDoc*)pDocument;
	}

}
void CMainFrame::CloseResultViewWindows()
{

/* ==========================
 * 기존 창을 닫는 부분이다.
 * ========================*/
	// 차일드 윈도우가 기존에 있다면 TimeView 윈도우인지 판별
	// 있다면 종료 시킨다.
	if( pResultVDoc ) 
	{
		pResultVDoc->OnCloseDocument();
	}
	pResultVDoc=NULL;

	// 다시 도큐먼트를 생성한다.
	if ( m_fIsResultTemplate == true )
	{
	pDocument = pResultVTemplate->CreateNewDocument();
	pResultVDoc=(CResultVDoc*)pDocument;
	}

	// RealView윈도우를 다시 나타낸다.
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
 *  RealView 윈도우가 있을경우 숨긴다.
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
 *  새로운 윈도우를 생성한다
 * ========================*/

// 1. 템플릿 찾는다.
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

// 2. 도큐먼트 생성
	POSITION Pos = pResultVTemplate->GetFirstDocPosition();
	if (Pos == NULL )
	{
		pDocument = pResultVTemplate->CreateNewDocument();
		pResultVDoc=(CResultVDoc*)pDocument;
	}

	//// 탬플릿 도큐먼트 생성 끝 ////////////////////////////

	// 생성된 윈도우의 뷰 핸들과 도큐먼트 핸들 획득
	pActiveChild = MDIGetActive();

// 3. 도큐먼트에 신호 타입, 모드 등 임시 저장 (뷰에서 사용할거임)

	// 도큐먼트에 정보 임시 저장.
	pResultVDoc->nTYPE=TYPE;
	pResultVDoc->nSelectedSignal=nSelectedSignal;
	pResultVDoc->nCheckedViewMode=nCheckedViewMode;

	// 타입이 응답 신호 or TS 측정일때.
	pResultVDoc->m_PreViewCtrlID1=0;
	pResultVDoc->m_PreViewCtrlID2=0;

// 4. 창 생성 신호와 타입에 따라 각각 생성된다.
	unsigned int MaxiIndex=g_sVUserData.RTV.m_nNSelectedCh1;
	unsigned int MaxJindex=g_sVUserData.RTV.m_nNSelectedCh2;
	
	if ( TYPE==1 || TYPE==3)
	{
		ResultViewType1DataLoading();
		ResultViewType3DataLoading();
		for( unsigned int iIndex=0; iIndex < MaxiIndex ; iIndex++ )
		{	
			// 프레임과 뷰를 생성 및 결합한다. 다만 OnUpdate는 아직.
			pFrame = pResultVTemplate->CreateNewFrame(pResultVDoc,pActiveChild);
				if (pFrame == NULL)
				{
					TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
					return ;     // command failed
				}

				// 뷰 인덱스 지정부분이다.
				pResultVDoc->m_PreViewCtrlID1=g_sVUserData.RTV.m_anSelectedCh1[iIndex];
				// 도큐먼트 프레임과 뷰를 도큐먼트에 붙인다. OnUpdate콜이 있다.
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
						// 프레임과 뷰를 생성 및 결합한다. 다만 OnUpdate는 아직.
						pFrame = pResultVTemplate->CreateNewFrame(pResultVDoc,pActiveChild);
						if (pFrame == NULL)
						{
							TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
							return ;     // command failed
						}

						// 뷰 인덱스 지정부분이다.
						pResultVDoc->m_PreViewCtrlID1=g_sVUserData.RTV.m_anSelectedCh1[iIndex];
						pResultVDoc->m_PreViewCtrlID2=g_sVUserData.RTV.m_anSelectedCh2[jIndex];
						// 도큐먼트 프레임과 뷰를 도큐먼트에 붙인다. OnUpdate콜이 있다.
						pResultVTemplate->InitialUpdateFrame(pFrame,pResultVDoc,TRUE /* Visible */);
					}
		}
		else
		{
			// 미완
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
	// 다시 도큐먼트를 생성한다.
	pDocument = pRealVTemplate->CreateNewDocument();
	pRealVDoc=(CRealVDoc*)pDocument;
	}
}

void CMainFrame::CreatRealViewWindows(int nE, int nR, int TypeOfGraph)
{
	
	// 템플릿을 처음 얻는 곳이다.
	// 이미 실행되었다면 스킵된다. m_bIsRealTemplate
	if ( m_fIsRealTemplate==false)
	{
//		AfxMessageBox("기존에 창이 존재하지 않아서 템플릿을 구하는 영역이다.");
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
	//// 탬플릿 도큐먼트 생성 시작 ////////////////////////////
	POSITION Pos = pRealVTemplate->GetFirstDocPosition();
	if (Pos == NULL )
	{
		pDocument = pRealVTemplate->CreateNewDocument();
		pRealVDoc=(CRealVDoc*)pDocument;
 	}
	//// 탬플릿 도큐먼트 생성 끝 ////////////////////////////

	// 생성된 윈도우의 뷰 핸들과 도큐먼트 핸들 획득
	pActiveChild = MDIGetActive();
 
    pRealVDoc->m_TypeOfGraph=TypeOfGraph;
  	pRealVDoc->m_nECount=nE;
  	pRealVDoc->m_nRCount=nR;

	// 타입이 응답 신호 or TS 측정일때.
	if ( TypeOfGraph == 0  )
	{		
		pRealVDoc->m_PreViewRCtrlID=0;
		for( int iIndex=0; iIndex < nR ; iIndex++ )
		{	
			// 프레임과 뷰를 생성 및 결합한다. 다만 OnUpdate는 아직.
			pFrame = pRealVTemplate->CreateNewFrame(pRealVDoc,pActiveChild /*pActiveChild*//*pActiveChild*/);
			if (pFrame == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
				return ;     // command failed
			}
			
			// 뷰 인덱스 지정부분이다.
			pRealVDoc->m_PreViewRCtrlID=iIndex;
			// 도큐먼트 프레임과 뷰를 도큐먼트에 붙인다. OnUpdate콜이 있다.
			pRealVTemplate->InitialUpdateFrame(pFrame,pRealVDoc,TRUE /* Visible */);
		}
	}

	// 타입이 SI TF 측정 일때
	if ( TypeOfGraph == 1 )
	{
		pRealVDoc->m_PreViewRCtrlID=0;
		for( int EIndex=0; EIndex < nE ; EIndex++ )
		{
			for( int RIndex=0; RIndex < nR ; RIndex++ )
			{	
				// 프레임과 뷰를 생성 및 결합한다. 다만 OnUpdate는 아직.
				pFrame = pRealVTemplate->CreateNewFrame(pRealVDoc,pActiveChild /*pActiveChild*//*pActiveChild*/);
				if (pFrame == NULL)
				{
					TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
					return ;     // command failed
				}

				// 뷰 인덱스 지정부분이다.
				pRealVDoc->m_PreViewRCtrlID=RIndex;
				pRealVDoc->m_PreViewECtrlID=EIndex;
				// 도큐먼트 프레임과 뷰를 도큐먼트에 붙인다. OnUpdate콜이 있다.
				pRealVTemplate->InitialUpdateFrame(pFrame,pRealVDoc,TRUE /* Visible */);
			}
		}

	}

	// 타입이 TS 일때
	if ( TypeOfGraph == 2 )
	{
		pRealVDoc->m_PreViewRCtrlID=0;
		for( int iIndex=0; iIndex < nR ; iIndex++ )
		{	
			// 프레임과 뷰를 생성 및 결합한다. 다만 OnUpdate는 아직.
			pFrame = pRealVTemplate->CreateNewFrame(pRealVDoc,pActiveChild /*pActiveChild*//*pActiveChild*/);
			if (pFrame == NULL)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to create new frame.\n");
				return ;     // command failed
			}

			// 뷰 인덱스 지정부분이다.
			pRealVDoc->m_PreViewRCtrlID=iIndex;
			// 도큐먼트 프레임과 뷰를 도큐먼트에 붙인다. OnUpdate콜이 있다.
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
		// SI, TS 공통 시간 데이타.
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
