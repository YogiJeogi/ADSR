// StartSI.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "StartDAQ.h"
#include "MainFrm.h"


// CStartDAQDlg dialog

IMPLEMENT_DYNAMIC(CStartDAQDlg, CDialog)

CStartDAQDlg::CStartDAQDlg(CWnd* pParent /*=NULL*/,bool _IsSI /*=TRUE*/)
	: CDialog(CStartDAQDlg::IDD, pParent)
	, m_bIsSI(_IsSI)
{
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CStartDAQDlg::IDD;

}

CStartDAQDlg::~CStartDAQDlg()
{
	
}

void CStartDAQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartDAQDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DAQ_Start, &CStartDAQDlg::OnBnClickedButtonDAQStart)
	ON_BN_CLICKED(IDC_BUTTON_DAQ_Stop, &CStartDAQDlg::OnBnClickedButtonDAQStop)
	ON_BN_CLICKED(IDCANCEL, &CStartDAQDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL CStartDAQDlg::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CStartDAQDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CStartDAQDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame* pMainFrame=static_cast<CMainFrame*> (m_pParent);
	pMainFrame->DAQStop();
	pMainFrame->DAQCleanUp();
	theApp.Modalessdone();
	DestroyWindow();
//	CDialog::OnCancel();
}

// CStartDAQDlg message handlers

void CStartDAQDlg::OnBnClickedButtonDAQStart()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame=static_cast<CMainFrame*> (m_pParent);

	if ( m_bIsSI==TRUE) pMainFrame->DAQSIInitialization();
	else pMainFrame->DAQTSInitialization();

	CButton* pcButton;
	pcButton=(CButton*)GetDlgItem(IDC_BUTTON_DAQ_Stop);
	pcButton->EnableWindow(true);
	pcButton=(CButton*)GetDlgItem(IDC_BUTTON_DAQ_Start);
	pcButton->EnableWindow(false);
}

void CStartDAQDlg::OnBnClickedButtonDAQStop()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pMainFrame=static_cast<CMainFrame*> (m_pParent);
	pMainFrame->DAQStop();
	CButton* pcButton=(CButton*)GetDlgItem(IDC_BUTTON_DAQ_Start);
	pcButton->EnableWindow(true);
}

BOOL CStartDAQDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( m_bIsSI==true) SetWindowText("System Identification");
	else SetWindowText("Target Simulation");

	CButton* pcButton;
	pcButton=(CButton*)GetDlgItem(IDC_BUTTON_DAQ_Stop);
	pcButton->EnableWindow(false);
	pcButton=(CButton*)GetDlgItem(IDC_BUTTON_DAQ_Start);
	pcButton->EnableWindow(true);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStartDAQDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (System.SI.m_bIsHReady==true) ProgStates.bSIComplete=true;
	if (System.TS.m_fIsRespDataFull==true) ProgStates.bTSComplete=true;

	OnCancel();
}
