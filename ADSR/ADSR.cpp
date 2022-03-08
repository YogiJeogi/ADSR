
// ADSR.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
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


// CADSRApp ����

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
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CADSRApp ��ü�Դϴ�.

CADSRApp theApp;


// CADSRApp �ʱ�ȭ

BOOL CADSRApp::InitInstance()
{

	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.

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
	

	// �� MDI ������ â�� ����ϴ�.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  MDI ���� ���α׷������� m_pMainWnd�� ������ �� �ٷ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	// ��� ���⿡ ���� ���⸦ Ȱ��ȭ�մϴ�.
	m_pMainWnd->DragAcceptFiles();

	// DDE Execute ���⸦ Ȱ��ȭ�մϴ�.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.

/*
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
*/

	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pMainFrame->ShowWindow(m_nCmdShow | SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	m_strvNIExciterChannel  = CNiDAQmxSystem::GetPhysicalChannels(DAQmxPhysicalChannelTypeAO, DAQmxPhysicalChannelAccessExternal);
	m_strvNIResponseChannel = CNiDAQmxSystem::GetPhysicalChannels(DAQmxPhysicalChannelTypeAI, DAQmxPhysicalChannelAccessExternal);
	m_numExciterChannels=int(m_strvNIExciterChannel.GetSize());
	m_numResponseChannels=int(m_strvNIResponseChannel.GetSize());
	return TRUE;
}




// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CADSRApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CADSRApp ����� ���� �ε�/���� �޼���

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

// CADSRApp �޽��� ó����
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
									,m_SIWhiteNoiseTimeData.ncols()+1);// ������ ����Ÿ�� 0���� �ϱ� ���� 1����Ʈ ũ��.
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
	
	// ��ȭ���ڸ� �����Ѵ�.
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

	// ȭ��Ʈ ����� �����Ǿ����� �Ǵ� / �ȵǾ����� ��ǪŻ��
	if( CWhiteDig::m_bIsWhiteNoiseGenerated==FALSE ) return;

	// ���� �����ӿ� �޼��� ����
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

		// �ӽ� ���� ���� ����
		MatDoub t_matDesiredSignal;
		MatDoub matMatchedDesiredSignal;

		int nRespC=System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels;
		int nRespMatchedC=System.SS.FRONTEND.m_nResponse_Channels+DSignImp.m_nSubMatchedDesiredSignals;
		
		// �����̾� �ñ׳� �ڸ� ����Ʈ(CUT)
		unsigned int Fp=int(DSignImp.m_dbDesiredFrom*System.SS.FRONTEND.m_nSamplingFreq);
		unsigned int Lp=int(DSignImp.m_dbDesiredTo*System.SS.FRONTEND.m_nSamplingFreq);

		// �ý��� SYSTEM.SS.TARGET�� ������ ������
		// LP�� ����������� �� ���������� �ణ �����Ѵ�.
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
		
		// ��Ʈ���� �ʱ�ȭ
		matMatchedDesiredSignal.Assign(nRespMatchedC,System.SS.TS.m_nNumberOfPointinDesiredSignals,0.);
		
		// �����̾� �ñ׳� ����Ʈ
		System.DataImportExport.GetDesiredSignals(Fp,Lp,t_matDesiredSignal);


		// �ñ׳� ��Ī
		 // 1. �� ���� ä��
 		for ( int i=0 ; i < System.SS.FRONTEND.m_nResponse_Channels; i++)
			for ( int j=0 ; j < System.SS.TS.m_nNumberOfPointinDesiredSignals; j++ )
 				matMatchedDesiredSignal[i][j]=DSignImp.m_anSignDesiredSignals[i]
  											*t_matDesiredSignal[DSignImp.m_anMatchedDesiredSignals[i]][j];
		 // 2. �� ���� ä��
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

	   // �̰��� TS�ʱ�ȭ ����
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

		// APP�� ����Ÿ ����, MatDoub �� NIMatrix
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

		// APP�� ����Ÿ ����, MatDoub �� NIMatrix
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

	// ��ȭ���ڸ� �����Ѵ�.
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

	// ��ȭ���ڸ� �����Ѵ�.
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

	// ��ȭ���ڸ� �����Ѵ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	System.DataImportExport.SaveTsDrive(System.TS.GetDriveSignals());
}



void CADSRApp::OnMenuSiresp()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	if (ProgStates.bExcChSet==true && ProgStates.bRespChSet==true )
		  pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);

}

void CADSRApp::OnUpdateMenuSiSi(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(ProgStates.bWNGen);
}

void CADSRApp::OnUpdateMenuSiWnv(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(ProgStates.bWNGen);
}

void CADSRApp::OnUpdateMenuTsDsImport(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(ProgStates.bSIComplete);
}

void CADSRApp::OnUpdateMenuTsDrvgen(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(ProgStates.bTargetImport);
}

void CADSRApp::OnUpdateMenuTsDrvup(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(ProgStates.bTSComplete);
}

void CADSRApp::OnUpdateMenuTsDrivesys(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(CSYSSetDig::m_bIsInitialized);
}
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
void CADSRApp::OnUpdateResultHmodel(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(FALSE);
}
