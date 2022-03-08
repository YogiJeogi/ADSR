// TimeVDig.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "ResultVDig.h"
#include "WhiteDig.h"


// 사용자 정의 메세지
#define WM_HD_RTV  WM_USER+2


// 정적 맴버 초기화
BOOL CResultVDig::m_bIsDesiredSignalTimeView=FALSE;
BOOL CResultVDig::m_bIsWhiteNoiseTimeView=FALSE;

// CResultVDig dialog

IMPLEMENT_DYNAMIC(CResultVDig, CDialog)

CResultVDig::CResultVDig(CWnd* pParent /*=NULL*/)
	: CDialog(CResultVDig::IDD, pParent)
{
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CResultVDig::IDD;
}

CResultVDig::CResultVDig( int Type,CWnd* pParent /*= NULL*/ )
	: CDialog(CResultVDig::IDD, pParent)

{
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CResultVDig::IDD;
	TYPE=Type;
}

CResultVDig::~CResultVDig()
{
}

void CResultVDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RV_SS, m_CtrlStaticSS);
	DDX_Control(pDX, IDC_STATIC_RV_VM, m_CtrlStaticVM);
	DDX_Control(pDX, IDC_LIST_Channel, m_CtrlListCh);
	DDX_Control(pDX, IDC_BUTTON_WIN_HOR, m_CtrlButtonHor);
	DDX_Control(pDX, IDC_BUTTON_WIN_CAS, m_CtrlButtonCas);
	DDX_Control(pDX, IDC_BUTTON_WIN_ALLCLOSE, m_CtrlButtonAllClose);
	DDX_Control(pDX, IDC_CHECK_TIME, m_CtrlCheckTime);
	DDX_Control(pDX, IDC_CHECK_PSD, m_CtrlCheckPSD);
	DDX_Control(pDX, IDC_CHECK_XY, m_CtrlCheckXY);
	DDX_Control(pDX, IDC_CHECK_STATIS, m_CtrlCheckStatistics);
	DDX_Control(pDX, IDOK, m_CtrlButtonView);
	DDX_Control(pDX, IDC_LIST_Channel2, m_CtrlListCh2);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_CtrlButtonClose);
	DDX_Control(pDX, IDC_STATIC_RV_H, m_CtrlStaticH);
	DDX_Control(pDX, IDC_STATIC_RV_SSCOM, m_CtrlStaticSSComp);
}


BEGIN_MESSAGE_MAP(CResultVDig, CDialog)
/*	ON_BN_CLICKED(IDC_BUTTON_WN_TimeV, &CResultVDig::OnBnClickedButtonWnTimev)*/
	ON_BN_CLICKED(IDOK, &CResultVDig::OnBnClickedView)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CResultVDig::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_RADIO_SS_WhiteNoise, &CResultVDig::OnBnClickedRadio0)
	ON_BN_CLICKED(IDC_RADIO12, &CResultVDig::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO13, &CResultVDig::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO14, &CResultVDig::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO15, &CResultVDig::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON_WIN_HOR, &CResultVDig::OnBnClickedButtonWinHor)
	ON_BN_CLICKED(IDC_BUTTON_WIN_CAS, &CResultVDig::OnBnClickedButtonWinCas)
	ON_BN_CLICKED(IDC_BUTTON_WIN_ALLCLOSE, &CResultVDig::OnBnClickedButtonWinAllclose)
END_MESSAGE_MAP()


// CResultVDig message handlers

void CResultVDig::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

BOOL CResultVDig::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CResultVDig::OnBnClickedView()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// 디버깅
	
	short nSelectedSignal;
	int nCheckedRadio=GetCheckedRadioButton(IDC_RADIO_SS_WhiteNoise,IDC_RADIO_SS_WhiteNoise+5)
											-IDC_RADIO_SS_WhiteNoise;
//  CString tt;
// 	tt.Format("%d",nCheckedRadio);
// 	AfxMessageBox(tt);
	int nCheckedViewMode=0;

	// 시그널 판별
	if		(TYPE==1) // Signal
	{
		nSelectedSignal=static_cast<short>(nCheckedRadio+100);
	}
	else if (TYPE==2) // H Model
	{
		nSelectedSignal=static_cast<short>(nCheckedRadio+200);
	}
	else if (TYPE==3) // Signal Comparison
	{
		nSelectedSignal=static_cast<short>(nCheckedRadio+300);
	}

	// 체크버튼 판별
	if(m_CtrlCheckTime.GetCheck()==BST_CHECKED) 
												nCheckedViewMode=nCheckedViewMode|1;
	if(m_CtrlCheckPSD.GetCheck()==BST_CHECKED)	
												nCheckedViewMode=nCheckedViewMode|2;
	if(m_CtrlCheckXY.GetCheck()==BST_CHECKED)	
												nCheckedViewMode=nCheckedViewMode|4;
	if(m_CtrlCheckStatistics.GetCheck()==BST_CHECKED) 
												nCheckedViewMode=nCheckedViewMode|8;
	
	// 리스트 판별
	POSITION pos2 = m_CtrlListCh2.GetFirstSelectedItemPosition();
	POSITION pos1 = m_CtrlListCh.GetFirstSelectedItemPosition();
	if (pos1 == NULL)
	{
		AfxMessageBox("선택된 신호가 없습니다."); return;
	}
	if ( TYPE==2 && pos2==NULL )
	{
		AfxMessageBox("선택된 신호가 없습니다."); return;
	}
	else
	{
		unsigned int nCount=m_CtrlListCh.GetSelectedCount();
		g_sVUserData.RTV.m_nNSelectedCh1=nCount;
		g_sVUserData.RTV.m_anSelectedCh1.SetSize(nCount);
		for (unsigned int i=0; i < m_CtrlListCh.GetSelectedCount() ; i++ )
		{
			g_sVUserData.RTV.m_anSelectedCh1[i]=m_CtrlListCh.GetNextSelectedItem(pos1);
		}
		nCount=m_CtrlListCh2.GetSelectedCount();
		g_sVUserData.RTV.m_nNSelectedCh2=nCount;
		g_sVUserData.RTV.m_anSelectedCh2.SetSize(nCount);
		g_sVUserData.RTV.m_anSelectedCh2.SetSize(m_CtrlListCh2.GetSelectedCount());	
		for (unsigned int i=0; i < m_CtrlListCh2.GetSelectedCount() ; i++ )
		{
			g_sVUserData.RTV.m_anSelectedCh2[i]=m_CtrlListCh2.GetNextSelectedItem(pos2);
		}
	}
	// 		CString tt;
	// 		tt.Format("%d",g_sVUserData.RTV.m_anSelectedCh1[0]);
	// 		AfxMessageBox(tt);
	


	// 메세지 전송
	// WPARAM LOWORD : 선택되는 신호 타입을 나타낸다.
	//				 : 1 -> 단일 신호
	//				 : 2 -> 전달 함수
	//				 : 3 -> 신호 비교
	// WPARAM HIWORD : 선택된 신호를 나타낸다.
	//				 : 100번대 
	//				 : 200번대
	//				 : 300번대
	// LPARAM LOWORD : CheckBox에서 선택된 것이 Bit연산되어있다.
	// LPARAM HIWORD : 0 -> 창 닫기
	//				 : 1 -> 창 생성하기
	//				 : 2 -> 리스트 선택 무시하고 신호와 관련된 모든창 생성
	//				 : 3 -> 체크 버튼 변화에 따른 창업데이트 할때
	AfxGetMainWnd()->SendMessage(WM_HD_RTV
								,MAKEWPARAM(TYPE,nSelectedSignal)
								,MAKELPARAM(nCheckedViewMode,1));

	//OnOK();
}


void CResultVDig::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
//	CDialog::OnCancel();
}

BOOL CResultVDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int nPriResp=System.SS.FRONTEND.m_nResponse_Channels;
	int nRespC=nPriResp+System.SS.FRONTEND.m_nSlave_Channels;
	int nExc=System.SS.FRONTEND.m_nInput_Channels;
	char N[50];
	CWnd * pRadiobutton;

	/*============================
	*	좌표 
	/*============================*/
	// 좌표 구하기
	CRect rect,MWndRect;
	CRect CtrlStaticSSRect,CtrlStaticHRect,CtrlListRect,CtrlList2Rect;
	CRect CtrlStaticSSCompRect,CtrlStaticVMRect;
	int x,dx1,dx2,dx;
	int y,dy;
	int cx;
	int cy;
	// 대화상자 좌표
	GetClientRect(&rect);
	// 메인프레임 좌표
	AfxGetMainWnd()->GetClientRect(&MWndRect);
	// 차일드 윈도우 좌표
	m_CtrlStaticSS.GetClientRect(&CtrlStaticSSRect);
	m_CtrlStaticH.GetClientRect(&CtrlStaticHRect);
	m_CtrlListCh.GetClientRect(&CtrlListRect);
	m_CtrlListCh2.GetClientRect(&CtrlList2Rect);
	m_CtrlStaticSSComp.GetClientRect(&CtrlStaticSSCompRect);
	m_CtrlStaticVM.GetClientRect(&CtrlStaticVMRect);

	/*============================
	*	리스트
	/*============================*/
	// 리스트 대입
	m_CtrlListCh.InsertColumn(0,"No",0);
	m_CtrlListCh.InsertColumn(1,"Excitation",0);
	m_CtrlListCh2.InsertColumn(0,"No",0);
	m_CtrlListCh2.InsertColumn(1,"Response",0);
		// 스타일 정의
	m_CtrlListCh.ModifyStyle(LVS_TYPEMASK|LVS_SINGLESEL,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_CtrlListCh2.ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_CtrlListCh.SetExtendedStyle(m_CtrlListCh.GetExtendedStyle()|LVS_EX_FULLROWSELECT
							|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);
	m_CtrlListCh2.SetExtendedStyle(m_CtrlListCh2.GetExtendedStyle()|LVS_EX_FULLROWSELECT
							|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);	

	/*============================
	*	체크 타임 초기치 체크됨
	/*============================*/
	m_CtrlCheckTime.SetCheck(BST_CHECKED);

	// Signals
	if (TYPE==1)
	{
		// 리스트
		// 리스트 콜룸 이름 변경
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Exc. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// 채널 이름 대입
		for (int i=0; i<nExc ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrInputChannelName[i].data());
		}
		// 리스트 칼럼 조절.
		m_CtrlListCh.SetColumnWidth(0,CtrlStaticSSRect.Width()*25/100);
		m_CtrlListCh.SetColumnWidth(1,CtrlStaticSSRect.Width()*72/100);


		// 다이어로그 창 크기 및 위치 설정
		cx=CtrlStaticSSRect.right-CtrlStaticSSRect.left+CtrlStaticSSRect.left*2+35;
		cy=rect.Height()+35;
		SetWindowPos(&wndTop,MWndRect.Width()-(cx+50),150,cx,cy,0);

		//라디오 버튼 체크 0번
		CheckRadioButton(IDC_RADIO_SS_WhiteNoise,IDC_RADIO_SS_WhiteNoise+5,IDC_RADIO_SS_WhiteNoise);

		////////////////////////////////////////////////////////////////////////
		if ( ProgStates.bWNGen!=true) {pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise); pRadiobutton->EnableWindow(false); m_CtrlButtonView.EnableWindow(FALSE);}
		if ( ProgStates.bFirstDrive!=true){pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+1); pRadiobutton->EnableWindow(false);}
		if ( ProgStates.bSIComplete!=true){pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+2); pRadiobutton->EnableWindow(false);}
		if ( ProgStates.bTSComplete!=true){pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+3); pRadiobutton->EnableWindow(false);}
		if ( ProgStates.bTargetImport!=true){pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+4); pRadiobutton->EnableWindow(false);}
		////////////////////////////////////////////////////////////////////////
	}
	// H Model
	else if (TYPE==2)
	{
		// 리스트
		// 리스트 콜룸 이름 변경
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Exc. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		m_CtrlListCh2.GetColumn(1,&col);
		col.pszText="Response";
		m_CtrlListCh2.SetColumn(1,&col);
		// 채널 이름 대입
		for (int i=0; i<nExc ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrInputChannelName[i].data());
		}
		for (int i=0; i<nPriResp ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh2.InsertItem(i,N);
			m_CtrlListCh2.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
		// 리스트 칼럼 조절.
		m_CtrlListCh.SetColumnWidth(0,CtrlStaticHRect.Width()*18/100);
		m_CtrlListCh.SetColumnWidth(1,CtrlStaticHRect.Width()*80/100);
		m_CtrlListCh2.SetColumnWidth(0,CtrlStaticHRect.Width()*18/100);
		m_CtrlListCh2.SetColumnWidth(1,CtrlStaticHRect.Width()*80/100);

		m_CtrlStaticVM.ShowWindow(FALSE);
		m_CtrlCheckStatistics.ShowWindow(FALSE);
		m_CtrlCheckTime.ShowWindow(FALSE);
		m_CtrlCheckXY.ShowWindow(FALSE);
		m_CtrlCheckPSD.ShowWindow(FALSE);
		m_CtrlStaticSS.ShowWindow(FALSE);

		// 다이어로그 창 크기 및 위치 설정
		cx=CtrlStaticHRect.right+35;
		cy=rect.Height()+35;
		SetWindowPos(&wndTop,MWndRect.Width()-(cx+50),150,cx,cy,0);

		
		x=15;
		dx=15;
		y=15;
		dy=30;

		// 라디오 윈도우 설정.
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise);
		pRadiobutton->SetWindowText("System Model H");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+dy,CtrlStaticHRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+1);
		pRadiobutton->SetWindowText("Respective Coherence");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+2*dy,CtrlStaticHRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+2);
		pRadiobutton->SetWindowText("Partial Coherence");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+3*dy,CtrlStaticHRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+3);
		pRadiobutton->SetWindowText("Multiple Coherence");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+4*dy,CtrlStaticHRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+4);
		pRadiobutton->ShowWindow(FALSE);
		pRadiobutton->EnableWindow(FALSE);

		// 스태틱 윈도우 위치 설정.
		m_CtrlStaticH.SetWindowPos(&wndTop,x,y,0,0,SWP_NOSIZE);

		//라디오 버튼 체크 0번
		CheckRadioButton(IDC_RADIO_SS_WhiteNoise,IDC_RADIO_SS_WhiteNoise+5,IDC_RADIO_SS_WhiteNoise);

		// 리스트 윈도우 위치 설정.
		m_CtrlListCh.SetWindowPos(&wndTop,15,15+CtrlStaticHRect.Height()+15
								,CtrlStaticHRect.right,CtrlListRect.Height(),0);
		m_CtrlListCh2.SetWindowPos(&wndTop,15,15+CtrlStaticHRect.Height()+15+CtrlListRect.Height()+15
								,CtrlStaticHRect.right,CtrlList2Rect.Height(),0);
		if ( ProgStates.bSIComplete!=true)	m_CtrlButtonView.EnableWindow(FALSE);
	}
	// Signal Comparison

	//////////////////////////////////////////////
	///////////        3                     ///////
	else if (TYPE==3)
	{
		// 리스트
		// 컬룸 이름 수정
		m_CtrlCheckXY.EnableWindow(true);
		m_CtrlCheckPSD.EnableWindow(TRUE);

		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Resp. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// 채널 이름 대입
		for (int i=0; i<nRespC ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
		// 리스트 칼럼 조절.
		m_CtrlListCh.SetColumnWidth(0,CtrlStaticSSCompRect.Width()*18/100);
		m_CtrlListCh.SetColumnWidth(1,CtrlStaticSSCompRect.Width()*80/100);


		// 기존 설정참 필요없는 창 숨기기
		m_CtrlStaticSS.ShowWindow(FALSE);

		// 다이어로그 창 크기 및 위치 설정
		int cx=CtrlStaticSSCompRect.right+35;
		int cy=rect.Height()+35;
		SetWindowPos(&wndTop,MWndRect.Width()-(cx+50),150,cx,cy,0);

		// 스태틱 윈도우 위치 설정.
		x=15;
		dx=15;
		y=15;
		dy=30;
		m_CtrlStaticSSComp.SetWindowPos(&wndTop,x,y,0,0,SWP_NOSIZE);
		// 라디오 윈도우 위치 설정.
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise);
		pRadiobutton->SetWindowText("Unfiltered and Filtered Desired Signals");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+dy,CtrlStaticSSCompRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+1);
		pRadiobutton->SetWindowText("Unfiltered and Filtered Response Signals");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+2*dy,CtrlStaticSSCompRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+2);
		pRadiobutton->SetWindowText("Unfiltered Desired and Response Signals");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+3*dy,CtrlStaticSSCompRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+3);
		pRadiobutton->SetWindowText("Filtered Desired and Response Signals");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+4*dy,CtrlStaticSSCompRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+4);
		pRadiobutton->ShowWindow(FALSE);
		pRadiobutton->EnableWindow(FALSE);

		if (ProgStates.bTSComplete!=true)
		{
			for (int i=0; i<4; i++)
			{
				pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+i);
				pRadiobutton->EnableWindow(FALSE);
			}
		}

		//라디오 버튼 체크 0번
		CheckRadioButton(IDC_RADIO_SS_WhiteNoise,IDC_RADIO_SS_WhiteNoise+5,IDC_RADIO_SS_WhiteNoise);

		// 스태틱 윈도우 위치 설정.
		x=15;
		dx1=15;
		dx2=CtrlStaticSSCompRect.right/2+20;
		y=15+CtrlStaticSSCompRect.Height()+15;
		dy=15;
		cx=CtrlStaticSSCompRect.right;
		cy=CtrlStaticVMRect.Height()*2/3;
		m_CtrlStaticVM.SetWindowPos(&wndTop,x,y,cx,cy,0);
		// 체크 버튼 위치 설정.
		m_CtrlCheckTime.SetWindowPos(&wndTop,x+dx1,y+5+dy,0,0,SWP_NOSIZE);
		m_CtrlCheckPSD.SetWindowPos(&wndTop,x+dx1,y+5+2*dy,0,0,SWP_NOSIZE);
		m_CtrlCheckXY.SetWindowPos(&wndTop,x+dx2,y+5+dy,0,0,SWP_NOSIZE);
		m_CtrlCheckStatistics.SetWindowPos(&wndTop,15+dx2,y+5+2*dy,0,0,SWP_NOSIZE);

		// 컨트롤 리스트 위치 설정
		x=15;
		y=15+CtrlStaticSSCompRect.Height()+15+CtrlStaticVMRect.Height()*2/3+15;
		cx=CtrlStaticSSCompRect.right;
		cy=CtrlListRect.Height();
		m_CtrlListCh.SetWindowPos(&wndTop,x,y,cx,cy,0);

	    if ( ProgStates.bTSComplete!=true)	m_CtrlButtonView.EnableWindow(FALSE);
	}
	else
	{
		return false;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CResultVDig::OnBnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	theApp.Modalessdone();
	AfxGetMainWnd()->SendMessage(WM_HD_RTV,0,0);
	DestroyWindow();
}


void CResultVDig::OnBnClickedRadio0()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// 콜룸 채널 명 변경
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Exc. Channel";
		m_CtrlListCh.SetColumn(1,&col);

		// 콜룸 아이템 변경
		int nExc=System.SS.FRONTEND.m_nInput_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		for (int i=0; i<nExc ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrInputChannelName[i].data());
		}
	}
}

void CResultVDig::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// 콜룸 채널 명 변경
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Exc. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		int nExc=System.SS.FRONTEND.m_nInput_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		// 리스트
		// 채널 이름 대입
		for (int i=0; i<nExc ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrInputChannelName[i].data());
		}
	}

}

void CResultVDig::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// 콜룸 채널 명 변경
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Resp. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// 콜룸 아이템 변경
		int nResp=System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		for (int i=0; i < nResp ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
	}

}

void CResultVDig::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// 콜룸 채널 명 변경
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Resp. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// 콜룸 아이템 변경
		int nResp=System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		for (int i=0; i < nResp ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
	}
}

void CResultVDig::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// 콜룸 채널 명 변경
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Resp. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// 콜룸 아이템 변경
		int nResp=System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		for (int i=0; i < nResp ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
	}
}

void CResultVDig::OnBnClickedButtonWinHor()
{
	// TODO: Add your control notification handler code here
	CMDIFrameWndEx* pmain=(CMDIFrameWndEx*)AfxGetMainWnd();
	pmain->MDITile();
}

void CResultVDig::OnBnClickedButtonWinCas()
{
	// TODO: Add your control notification handler code here
	CMDIFrameWndEx* pmain=(CMDIFrameWndEx*)AfxGetMainWnd();
	pmain->MDICascade();
}

void CResultVDig::OnBnClickedButtonWinAllclose()
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_HD_RTV
								,MAKEWPARAM(0,0)
								,MAKELPARAM(0,4));
}
