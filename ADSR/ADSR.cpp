
// ADSR.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "ADSR.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "SYSSetDig.h"
#include "ExcDig.h"
#include "RespDig.h"
#include "RealView.h"
#include "RealVDoc.h"
#include "WhiteDig.h"
#include "ResultView.h"
#include "ResultDoc.h"
#include "StartDAQ.h"
#include "RealVDlg.h"
#include "RealFrame.h"
#include "DSignalsImport.h"
#include "DrvGen.h"

#define  _SI TRUE
#define  _TS FALSE

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCriticalSection g_CriticalSection;
CSystem       System;
sRealResultViewUserData  g_sVUserData;
ProgressState ProgStates; 

// CADSRApp

BEGIN_MESSAGE_MAP(CADSRApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CADSRApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_MENU_SYS_SYSSET, &CADSRApp::OnMenuSysSysset)
	ON_COMMAND(ID_MENU_SYS_EXC, &CADSRApp::OnMenuSysExc)
	ON_COMMAND(ID_MENU_SYS_Resp, &CADSRApp::OnMenuSysResp)
	ON_COMMAND(ID_MENU_SI_WN, &CADSRApp::OnMenuSiWn)
	ON_COMMAND(ID_MENU_SI_SIV, &CADSRApp::OnMenuSiSiv)
	ON_UPDATE_COMMAND_UI(ID_MENU_SYS_EXC, &CADSRApp::OnUpdateMenuSysExc)
	ON_UPDATE_COMMAND_UI(ID_MENU_SYS_Resp, &CADSRApp::OnUpdateMenuSysResp)
	ON_COMMAND(ID_MENU_SI_WNV, &CADSRApp::OnMenuSiWnv)
	ON_COMMAND(ID_MENU_SI_SI, &CADSRApp::OnMenuSiSi)
	ON_COMMAND(ID_MENU_GRAPH_WIN, &CADSRApp::OnMenuGraphCreat)
	ON_COMMAND(ID_MENU_TS_DS_IMPORT, &CADSRApp::OnMenuTsDsImport)
	ON_COMMAND(ID_MENU_TS_DrvGen, &CADSRApp::OnMenuTsDrvgen)
	ON_COMMAND(ID_MENU_TS_DrvUp, &CADSRApp::OnMenuTsDrvup)
	ON_COMMAND(ID_MENU_TS_DriveSys, &CADSRApp::OnMenuTsDrivesys)
	ON_COMMAND(ID_RESULT_SIGNALS, &CADSRApp::OnResultSignals)
	ON_COMMAND(ID_RESULT_HMODEL, &CADSRApp::OnResultHmodel)
	ON_COMMAND(ID_RESULT_SIGNALCOMPARISON, &CADSRApp::OnResultSignalcomparison)
	ON_COMMAND(ID_MENU_SIDATA, &CADSRApp::OnMenuSidata)
	ON_COMMAND(ID_TS_RESP, &CADSRApp::OnTsResp)
	ON_COMMAND(ID_MENU_TSDRIVE, &CADSRApp::OnMenuTsdrive)
	ON_UPDATE_COMMAND_UI(ID_MENU_SI_WNV, &CADSRApp::OnUpdateMenuSiWnv)
	ON_COMMAND(ID_MENU_SIRESP, &CADSRApp::OnMenuSiresp)
	ON_UPDATE_COMMAND_UI(ID_MENU_SI_WN, &CADSRApp::OnUpdateMenuSiWn)
	ON_UPDATE_COMMAND_UI(ID_MENU_SI_SI, &CADSRApp::OnUpdateMenuSiSi)
	ON_UPDATE_COMMAND_UI(ID_MENU_TS_DS_IMPORT, &CADSRApp::OnUpdateMenuTsDsImport)
	ON_UPDATE_COMMAND_UI(ID_MENU_TS_DrvGen, &CADSRApp::OnUpdateMenuTsDrvgen)
	ON_UPDATE_COMMAND_UI(ID_MENU_TS_DrvUp, &CADSRApp::OnUpdateMenuTsDrvup)
	ON_UPDATE_COMMAND_UI(ID_MENU_TS_DriveSys, &CADSRApp::OnUpdateMenuTsDrivesys)
	ON_UPDATE_COMMAND_UI(ID_MENU_GRAPH_WIN, &CADSRApp::OnUpdateMenuGraphWin)
	ON_UPDATE_COMMAND_UI(ID_RESULT_HMODEL, &CADSRApp::OnUpdateResultHmodel)
END_MESSAGE_MAP()


// CADSRApp 생성

CADSRApp::CADSRApp()
{

	m_bHiColorIcons				=	 TRUE;
	m_bIsNewProject				=	 FALSE;
	m_bIsSystemsetInitialized	=	 FALSE;

	ProgStates.bExcChSet=false;
	ProgStates.bFirstDrive=false;
	ProgStates.bRespChSet=false;
	ProgStates.bSIComplete=false;
	ProgStates.bSystemSet=false;
	ProgStates.bTargetImport=false;
	ProgStates.bTSComplete=false;
	ProgStates.bUpDrive=false;
	ProgStates.bWNGen=false;
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CADSRApp 개체입니다.

CADSRApp theApp;


// CADSRApp 초기화

BOOL CADSRApp::InitInstance()
{

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.

	CMultiDocTemplate* pNewDocTemplate; 
	pNewDocTemplate	= new CMultiDocTemplate(IDR_RESULTVIEW_TMPL,
		RUNTIME_CLASS(CResultVDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CResultView));
	AddDocTemplate(pNewDocTemplate);

	CMultiDocTemplate* pSecondDocTemplate;
	pSecondDocTemplate = new CMultiDocTemplate(IDR_REALVIEW_TMPL,
		RUNTIME_CLASS(CRealVDoc),
		RUNTIME_CLASS(CRealFrame),
		RUNTIME_CLASS(CRealView));
	AddDocTemplate(pSecondDocTemplate);
	

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.
	// 끌어서 놓기에 대한 열기를 활성화합니다.
	m_pMainWnd->DragAcceptFiles();

	// DDE Execute 열기를 활성화합니다.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.

/*
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
*/

	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(m_nCmdShow | SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	m_strvNIExciterChannel  = CNiDAQmxSystem::GetPhysicalChannels(DAQmxPhysicalChannelTypeAO, DAQmxPhysicalChannelAccessExternal);
	m_strvNIResponseChannel = CNiDAQmxSystem::GetPhysicalChannels(DAQmxPhysicalChannelTypeAI, DAQmxPhysicalChannelAccessExternal);
	m_numExciterChannels=int(m_strvNIExciterChannel.GetSize());
	m_numResponseChannels=int(m_strvNIResponseChannel.GetSize());
	return TRUE;
}




// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CADSRApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CADSRApp 사용자 지정 로드/저장 메서드

void CADSRApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CADSRApp::LoadCustomState()
{

}

void CADSRApp::SaveCustomState()
{
}

// CADSRApp 메시지 처리기
void CADSRApp::OnMenuSysSysset()
{
	// TODO: Add your command handler code here
	CSYSSetDig SYSSetDig;

	if ( SYSSetDig.DoModal()==IDOK)
	{
		ProgStates.bSystemSet=true;
		ProgStates.bExcChSet=false;
		ProgStates.bFirstDrive=false;
		ProgStates.bRespChSet=false;
		ProgStates.bSIComplete=false;
		ProgStates.bTargetImport=false;
		ProgStates.bTSComplete=false;
		ProgStates.bUpDrive=false;
		ProgStates.bWNGen=false;
	}
}

void CADSRApp::OnMenuSysExc()
{
	// TODO: Add your command handler code here
	CExcDig ExcDig;

	if ( ExcDig.DoModal()==IDOK)
	{
		ProgStates.bSystemSet=true;
		ProgStates.bExcChSet=true;
		ProgStates.bFirstDrive=false;
		ProgStates.bRespChSet=false;
		ProgStates.bSIComplete=false;
		ProgStates.bTargetImport=false;
		ProgStates.bTSComplete=false;
		ProgStates.bUpDrive=false;
		ProgStates.bWNGen=false;
	}

}

void CADSRApp::OnMenuSysResp()
{
	// TODO: Add your command handler code here
	CRespDig RespDig;

	if ( RespDig.DoModal()==IDOK)
	{
		ProgStates.bSystemSet=true;
		ProgStates.bExcChSet=true;
		ProgStates.bFirstDrive=false;
		ProgStates.bRespChSet=TRUE;
		ProgStates.bSIComplete=false;
		ProgStates.bTargetImport=false;
		ProgStates.bTSComplete=false;
		ProgStates.bUpDrive=false;
		ProgStates.bWNGen=false;
	}
}

void CADSRApp::OnMenuSiWn()
{
	// TODO: Add your command handler code here
	CWhiteDig WhiteDig;

	if ( WhiteDig.DoModal()==IDOK)
	{
	 System.SI.SIInitialization(System.SS);
	 System.SI.GenerateWhiteNoise();
	 m_SIWhiteNoiseTimeData=System.SI.GetWhiteNoise();
	 m_SIWhiteNoiseTimeDataNI.SetSize(m_SIWhiteNoiseTimeData.nrows()
									,m_SIWhiteNoiseTimeData.ncols()+1);// 마지막 데이타를 0으로 하기 위해 1포인트 크다.
	for (int i=0; i < m_SIWhiteNoiseTimeData.nrows(); i++)
		for (int j=0; j < m_SIWhiteNoiseTimeData.ncols(); j++ )
			m_SIWhiteNoiseTimeDataNI(i,j)=m_SIWhiteNoiseTimeData[i][j];

	//
	ProgStates.bSystemSet=true;
	ProgStates.bExcChSet=true;
	ProgStates.bFirstDrive=false;
	ProgStates.bRespChSet=TRUE;
	ProgStates.bSIComplete=false;
	ProgStates.bTargetImport=false;
	ProgStates.bTSComplete=false;
	ProgStates.bUpDrive=false;
	ProgStates.bWNGen=true;
	}
}

void CADSRApp::OnMenuSiSiv()
{
	// TODO: Add your command handler code here					
}
void CADSRApp::OnMenuSiWnv()
{
	// TODO: Add your command handler code here
	CResultVDig::m_bIsWhiteNoiseTimeView=TRUE;
	CResultVDig::m_bIsDesiredSignalTimeView=FALSE;
	
	// 대화상자를 생성한다.
	if (pResultVDlg == NULL)
	{
		pResultVDlg = new CResultVDig(1,m_pMainWnd);
		if (pResultVDlg->Create() == TRUE)
		{
			pResultVDlg->ShowWindow(SW_SHOW);
		}

	}
	else
	{
		//		pModelVDlg->SetActiveWindow();
	}

	// 화이트 노이즈가 생성되었는지 판단 / 안되었으면 루푸탈퇴
	if( CWhiteDig::m_bIsWhiteNoiseGenerated==FALSE ) return;

	// 메인 프레임에 메세지 전달
	m_pMainWnd->SendMessage(WM_HD_RTV
						   ,MAKEWPARAM(1,0)
						   ,MAKELPARAM(System.SS.FRONTEND.m_nInput_Channels,0));
	
}

void CADSRApp::Modalessdone()
{
	pResultVDlg  = NULL;
	pStartDlg    = NULL;
}

void CADSRApp::OnMenuSiSi()
{
	// TODO: Add your command handler code here
	if (pStartDlg == NULL)
	{
		pStartDlg = new CStartDAQDlg(m_pMainWnd,_SI);
		if (pStartDlg->Create() == TRUE){
			pStartDlg->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		//		pModelVDlg->SetActiveWindow();
	}

}

void CADSRApp::OnMenuGraphCreat()
{
	// TODO: Add your command handler code here
	CRealVDlg RealDlg;
	int tiResponse = RealDlg.DoModal();
	if ( tiResponse==IDOK)
	{
		
	}
}
void CADSRApp::OnMenuTsDsImport()
{
	// TODO: Add your command handler code here
	CDSignalsImport DSignImp;

	if ( DSignImp.DoModal()==IDOK)
	{

		// 임시 저장 변수 선언
		MatDoub t_matDesiredSignal;
		MatDoub matMatchedDesiredSignal;

		int nRespC=System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels;
		int nRespMatchedC=System.SS.FRONTEND.m_nResponse_Channels+DSignImp.m_nSubMatchedDesiredSignals;
		
		// 디자이어 시그널 자를 포인트(CUT)
		unsigned int Fp=int(DSignImp.m_dbDesiredFrom*System.SS.FRONTEND.m_nSamplingFreq);
		unsigned int Lp=int(DSignImp.m_dbDesiredTo*System.SS.FRONTEND.m_nSamplingFreq);

		// 시스템 SYSTEM.SS.TARGET에 저장할 변수들
		// LP를 오버랩배수에 딱 떨어지도록 약간 변경한다.
		int tempb=(Lp-Fp+1-System.SS.SPECTRAL.m_nBlocksize)/System.SS.SPECTRAL.m_nPshifted;
		Lp=System.SS.SPECTRAL.m_nBlocksize+System.SS.SPECTRAL.m_nPshifted*tempb+Fp-1;

		System.SS.TS.m_nNumberOfPointinDesiredSignals=Lp-Fp+1;
// 		CString tt;
// 		tt.Format("TargetPoint : %d",Lp-Fp+1);
// 		AfxMessageBox(tt);
		System.SS.TS.m_nNumberOfBlockSize=int(double(System.SS.TS.m_nNumberOfPointinDesiredSignals)
												 /double(System.SS.SPECTRAL.m_nBlocksize)+1);
		System.SS.TS.m_nNumberOfChannelOfDesiredSignals=nRespMatchedC;
		System.SS.TS.m_dbDesiredSignalsTime=double(System.SS.TS.m_nNumberOfPointinDesiredSignals)
												/System.SS.FRONTEND.m_nSamplingFreq;
		System.SS.TS.m_nSubMatchedDesiredSignals=DSignImp.m_nSubMatchedDesiredSignals;

		for (int i=0; i < System.SS.FRONTEND.m_nSlave_Channels; i++ )
		{
			System.SS.TS.m_bSubMatchedDesiredSignals[i]
			 = DSignImp.m_aryIsDesiredSignalExistInReationWithSubResponseChannel[i];
		}
		
		// 매트릭스 초기화
		matMatchedDesiredSignal.Assign(nRespMatchedC,System.SS.TS.m_nNumberOfPointinDesiredSignals,0.);
		
		// 디자이어 시그널 임폴트
		System.DataImportExport.GetDesiredSignals(Fp,Lp,t_matDesiredSignal);


		// 시그널 매칭
		 // 1. 주 응답 채널
 		for ( int i=0 ; i < System.SS.FRONTEND.m_nResponse_Channels; i++)
			for ( int j=0 ; j < System.SS.TS.m_nNumberOfPointinDesiredSignals; j++ )
 				matMatchedDesiredSignal[i][j]=DSignImp.m_anSignDesiredSignals[i]
  											*t_matDesiredSignal[DSignImp.m_anMatchedDesiredSignals[i]][j];
		 // 2. 부 응답 채널
		int isub=0;
		int inR=System.SS.FRONTEND.m_nResponse_Channels;
		for ( int i=0 ; i < System.SS.FRONTEND.m_nSlave_Channels; i++)
		{
			if ( System.SS.TS.m_bSubMatchedDesiredSignals[i]==true )
			{
				for ( int j=0 ; j < System.SS.TS.m_nNumberOfPointinDesiredSignals ; j++ )
				{
					matMatchedDesiredSignal[inR+isub][j]=DSignImp.m_anSignDesiredSignals[inR+i]
						*t_matDesiredSignal[DSignImp.m_anMatchedDesiredSignals[inR+i]][j];
				}
				isub +=1;
			}
			else
			{
				
			}
		}

	   // 이곳에 TS초기화 문구
 		System.SI.CalculateINVH();
 		System.TS.TSInitialization(System.SS);
 		System.TS.ImportInverseModel(System.SI.GetInvHModel());
 		System.TS.ImportDesiredSignals(matMatchedDesiredSignal);

		//
		ProgStates.bTargetImport=true;
		ProgStates.bTSComplete=false;
		ProgStates.bFirstDrive=false;
		ProgStates.bUpDrive=false;

	}// if ( DSignImp.DoModal()==IDOK)
}

void CADSRApp::OnMenuTsDrvgen()
{
	// TODO: Add your command handler code here
	CDrvGen DrvGen(TRUE);
	if (DrvGen.DoModal()==IDOK)
	{

		System.TS.GenerateFirstDriveSignals(DrvGen.m_vecOutWeightLevel
										   ,DrvGen.m_vecFirstDriveLevel);

		// APP에 데이타 저장, MatDoub 와 NIMatrix
		m_TSDriveTimeData=System.TS.GetDriveSignals();
		m_TSDriveTimeDataNI.SetSize(m_TSDriveTimeData.nrows()
							       ,m_TSDriveTimeData.ncols()+1);
		for (int i=0; i < m_TSDriveTimeData.nrows(); i++)
			for (int j=0; j < m_TSDriveTimeData.ncols(); j++ )
				m_TSDriveTimeDataNI(i,j)=m_TSDriveTimeData[i][j];

		//

		ProgStates.bTSComplete=false;
		ProgStates.bFirstDrive=true;
		ProgStates.bUpDrive=false;
	}
}

void CADSRApp::OnMenuTsDrvup()
{
	// TODO: Add your command handler code here
	CDrvGen DrvGen(FALSE);
	if (DrvGen.DoModal()==IDOK)
	{
		System.TS.GenerateUpdatedDriveSignals(DrvGen.m_vecOutWeightLevel
										     ,DrvGen.m_vecErrorLevel);

		// APP에 데이타 저장, MatDoub 와 NIMatrix
		m_TSDriveTimeData=System.TS.GetDriveSignals();
		m_TSDriveTimeDataNI.SetSize(m_TSDriveTimeData.nrows()
									,m_TSDriveTimeData.ncols()+1);
		for (int i=0; i < m_TSDriveTimeData.nrows(); i++)
			for (int j=0; j < m_TSDriveTimeData.ncols(); j++ )
				m_TSDriveTimeDataNI(i,j)=m_TSDriveTimeData[i][j];

		//
		ProgStates.bTSComplete=false;
		ProgStates.bFirstDrive=true;
		ProgStates.bUpDrive=true;
	}
}

void CADSRApp::OnMenuTsDrivesys()
{
	// TODO: Add your command handler code here
	if (pStartDlg == NULL)
	{
		pStartDlg = new CStartDAQDlg(m_pMainWnd,_TS);
		if (pStartDlg->Create() == TRUE){
			pStartDlg->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		//		pModelVDlg->SetActiveWindow();
	}
	
}

void CADSRApp::OnResultSignals()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CResultVDig::m_bIsWhiteNoiseTimeView=TRUE;
	CResultVDig::m_bIsDesiredSignalTimeView=FALSE;

	// 대화상자를 생성한다.
	if (pResultVDlg == NULL)
	{
		pResultVDlg = new CResultVDig(1,m_pMainWnd);
		if (pResultVDlg->Create() == TRUE)
		{
			pResultVDlg->ShowWindow(SW_SHOW);
		}

	}
	else
	{
		//		pModelVDlg->SetActiveWindow();
	}
}

void CADSRApp::OnResultHmodel()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CResultVDig::m_bIsWhiteNoiseTimeView=TRUE;
	CResultVDig::m_bIsDesiredSignalTimeView=FALSE;

	// 대화상자를 생성한다.
	if (pResultVDlg == NULL)
	{
		pResultVDlg = new CResultVDig(2,m_pMainWnd);
		if (pResultVDlg->Create() == TRUE)
		{
			pResultVDlg->ShowWindow(SW_SHOW);
		}

	}
	else
	{
		//		pModelVDlg->SetActiveWindow();
	}
}

void CADSRApp::OnResultSignalcomparison()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CResultVDig::m_bIsWhiteNoiseTimeView=TRUE;
	CResultVDig::m_bIsDesiredSignalTimeView=FALSE;

	// 대화상자를 생성한다.
	if (pResultVDlg == NULL)
	{
		pResultVDlg = new CResultVDig(3,m_pMainWnd);
		if (pResultVDlg->Create() == TRUE)
		{
			pResultVDlg->ShowWindow(SW_SHOW);
		}

	}
	else
	{
		//		pModelVDlg->SetActiveWindow();
	}
}

void CADSRApp::OnMenuSidata()
{
	// TODO: Add your command handler code here
	System.DataImportExport.SaveSIData(System.SI.m_WHN_driver,System.SI.m_Response);
}

void CADSRApp::OnTsResp()
{
	// TODO: Add your command handler code here
	System.DataImportExport.SaveTsResp(System.TS.GetResponseSignals());
}

void CADSRApp::OnMenuTsdrive()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	System.DataImportExport.SaveTsDrive(System.TS.GetDriveSignals());
}



void CADSRApp::OnMenuSiresp()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	System.DataImportExport.SaveSIResp(System.SI.m_Response);
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void CADSRApp::OnUpdateMenuSysExc(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(ProgStates.bSystemSet);
}

void CADSRApp::OnUpdateMenuSysResp(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(ProgStates.bSystemSet);
}

void CADSRApp::OnUpdateMenuSiWn(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (ProgStates.bExcChSet==true && ProgStates.bRespChSet==true )
		  pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);

}

void CADSRApp::OnUpdateMenuSiSi(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(ProgStates.bWNGen);
}

void CADSRApp::OnUpdateMenuSiWnv(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(ProgStates.bWNGen);
}

void CADSRApp::OnUpdateMenuTsDsImport(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(ProgStates.bSIComplete);
}

void CADSRApp::OnUpdateMenuTsDrvgen(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(ProgStates.bTargetImport);
}

void CADSRApp::OnUpdateMenuTsDrvup(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(ProgStates.bTSComplete);
}

void CADSRApp::OnUpdateMenuTsDrivesys(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(ProgStates.bFirstDrive==true||ProgStates.bUpDrive==true)
	{	
		pCmdUI->Enable(true);
	}
	else
	{
 		pCmdUI->Enable(FALSE);
	}
}

void CADSRApp::OnUpdateMenuGraphWin(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(CSYSSetDig::m_bIsInitialized);
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void CADSRApp::OnUpdateResultHmodel(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(FALSE);
}
