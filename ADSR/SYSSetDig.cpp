// SYSSetDig.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "SYSSetDig.h"
#include <cmath>


// CSYSSetDig dialog

bool CSYSSetDig::m_bIsInitialized=FALSE;

IMPLEMENT_DYNAMIC(CSYSSetDig, CDialog)

CSYSSetDig::CSYSSetDig(CWnd* pParent /*=NULL*/)
	: CDialog(CSYSSetDig::IDD, pParent)
	, m_nInput(1)
	, m_nResp(1)
	, m_nSubResp(0)
	, m_nBlockSize(2)
	, m_nCyclicAvg(0)
	, m_nWindow(0)
	, m_nOverlap(0)
	, m_nSamplingFreq(2)
	, m_nStartFreq(1)
	, m_nStopFreq(100)
{
	// ���� ����
	// ������Ʈ�� �ҷ������� �ҷ��� ����Ÿ���� ������ ����
	if (theApp.m_bIsNewProject != TRUE)
	{
	}

}

CSYSSetDig::~CSYSSetDig()
{
}

void CSYSSetDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_nInputCh, m_nInput);
	DDV_MinMaxInt(pDX, m_nInput, 1, 12);
	DDX_Text(pDX, IDC_EDIT_nOutputCh, m_nResp);
	DDV_MinMaxInt(pDX, m_nResp, 1, 16);
	DDX_Text(pDX, IDC_EDIT_nSlaveCh, m_nSubResp);
	DDV_MinMaxInt(pDX, m_nSubResp, 0, 16);
	DDX_CBIndex(pDX, IDC_COMBO_BS, m_nBlockSize);
	DDX_CBIndex(pDX, IDC_COMBO_CycAvg, m_nCyclicAvg);
	DDX_CBIndex(pDX, IDC_COMBO_Window, m_nWindow);
	DDX_CBIndex(pDX, IDC_COMBO_Overlap, m_nOverlap);
	DDX_CBIndex(pDX, IDC_COMBO_FS, m_nSamplingFreq);
	DDX_Text(pDX, IDC_EDIT_StartFre, m_nStartFreq);
	DDX_Text(pDX, IDC_EDIT_Stopfre, m_nStopFreq);
}


BEGIN_MESSAGE_MAP(CSYSSetDig, CDialog)
	ON_BN_CLICKED(IDOK, &CSYSSetDig::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RECONNECT_HW, &CSYSSetDig::OnBnClickedButtonReconnectHw)
	ON_EN_KILLFOCUS(IDC_EDIT_nSlaveCh, &CSYSSetDig::OnEnKillfocusEditnslavech)
	ON_EN_KILLFOCUS(IDC_EDIT_nOutputCh, &CSYSSetDig::OnEnKillfocusEditnoutputch)
END_MESSAGE_MAP()


// CSYSSetDig message handlers


BOOL CSYSSetDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// ����ü���� �ҷ��´�.

	if ( m_bIsInitialized==TRUE )
	{

		m_nSamplingFreq	=		int(log(double(System.SS.FRONTEND.m_nSamplingFreq))/log(2.)-8);
		m_nStartFreq	=		System.SS.SPECTRAL.m_nStartFreq;
		m_nStopFreq  	=		System.SS.SPECTRAL.m_nStopFreq;
		m_nInput		=		System.SS.FRONTEND.m_nInput_Channels;		
		m_nResp			=		System.SS.FRONTEND.m_nResponse_Channels;		
		m_nSubResp		=		System.SS.FRONTEND.m_nSlave_Channels;
		m_nBlockSize	=		int(log(double(System.SS.SPECTRAL.m_nBlocksize))/log(2.)-8);
		m_nCyclicAvg	=		System.SS.SPECTRAL.m_nCyclicAveg-1;
		m_nWindow		=		System.SS.SPECTRAL.m_nWindow;	
		m_nOverlap		=		System.SS.SPECTRAL.m_nPercentofOverlap/25;	
		
	}
	
	HowManyHWExist();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSYSSetDig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// �Էµ� ���� ������ ������ �Ǵ�
	if ( int(pow(2.,m_nSamplingFreq+8)) >= m_nStopFreq*2 )
		if( m_nSubResp+m_nResp <= MAX_OUT )
			if ( m_nStopFreq > m_nStartFreq ) 
				if ( m_nInput <= theApp.m_numExciterChannels ) 
					if ( m_nResp <= theApp.m_numResponseChannels ) {}
					else {AfxMessageBox(_T("���� ä�μ����� �߸��Ǿ����ϴ�.")); return;}
				else {AfxMessageBox(_T("���� ä�� ������ �߸��Ǿ����ϴ�.")); return;}
			else {AfxMessageBox(_T("Stop Fre�� Start Fre���� Ŀ���մϴ�.")); return;}
		else {AfxMessageBox(_T("���� ä�ΰ� �� ���� ä�� ���� 16ä���� �ѱ� �� �����ϴ�.")); return;}
	else {AfxMessageBox(_T("Stop Fre�� ���ø� ���ļ��� �� ���Ͽ��� �մϴ�.")); return;}

	// �Էµ� ���� ������ ���ٸ� ����

	// ����
	System.SS.FRONTEND.m_nInput_Channels				=	m_nInput;
	System.SS.FRONTEND.m_nResponse_Channels				=	m_nResp;
	System.SS.FRONTEND.m_nSlave_Channels				=	m_nSubResp;
 	System.SS.FRONTEND.m_nSamplingFreq					=	int(pow(2.,m_nSamplingFreq+8));

	System.SS.SPECTRAL.SetSpectralAnalysisParam(    m_nStartFreq,
													m_nStopFreq,
													int(pow(2.,m_nBlockSize+8)),
													m_nCyclicAvg+1,
													m_nWindow,
													m_nOverlap*25,
													System.SS.FRONTEND.m_nSamplingFreq );

	// �Ϸ�
	m_bIsInitialized	=	TRUE;

	OnOK();
}



void CSYSSetDig::HowManyHWExist()
{
	// �ϵ���� �������� �о ȭ�鿡 ǥ���Ѵ�.
	CStatic* tstaticinput  =(CStatic*)GetDlgItem(IDC_STATIC_INC);
	CStatic* tstaticoutput =(CStatic*)GetDlgItem(IDC_STATIC_OUTC);	
	CStatic* tstaticoutsub =(CStatic*)GetDlgItem(IDC_STATIC_OUTS);
	CEdit*   teditcoutout  =(CEdit*)GetDlgItem(IDC_EDIT_nOutputCh);

	char tch[3];
	// ���� ä��
	_itoa_s(theApp.m_numExciterChannels,tch,3,10);
	tstaticinput->SetWindowText(tch);
	// ���� ä��
	_itoa_s(theApp.m_numResponseChannels,tch,3,10);
	tstaticoutput->SetWindowText(tch);
	// ������ ä��
	CString tstr; 
	int tnumber;
	teditcoutout->GetWindowText(tstr);
	tnumber=_tstoi(tstr);
	_itoa_s(theApp.m_numResponseChannels-tnumber,tch,3,10);
	tstaticoutsub->SetWindowText(tch);

}


void CSYSSetDig::OnBnClickedButtonReconnectHw()
{
	// TODO: Add your control notification handler code here
	HowManyHWExist();
	theApp.m_strvNIExciterChannel  = CNiDAQmxSystem::GetPhysicalChannels(DAQmxPhysicalChannelTypeAO, DAQmxPhysicalChannelAccessExternal);
	theApp.m_strvNIResponseChannel = CNiDAQmxSystem::GetPhysicalChannels(DAQmxPhysicalChannelTypeAI, DAQmxPhysicalChannelAccessExternal);
	theApp.m_numExciterChannels=int(theApp.m_strvNIExciterChannel.GetSize());
	theApp.m_numResponseChannels=int(theApp.m_strvNIResponseChannel.GetSize());
}

void CSYSSetDig::OnEnKillfocusEditnslavech()
{
	// TODO: Add your control notification handler code here
/*
	CEdit* tEditOutputC=(CEdit*)GetDlgItem(IDC_EDIT_nOutputCh);
	CEdit* tEditOutsubC=(CEdit*)GetDlgItem(IDC_EDIT_nSlaveCh);
	CString tstr; 
	int tnumber;
	tEditOutputC->GetWindowText(tstr);
	tnumber=_tstoi(tstr);
	tEditOutsubC->GetWindowText(tstr);
	tnumber=tnumber+_tstoi(tstr);

	if( tnumber > theApp.m_numResponseChannels)
		tEditOutsubC->SetFocus();
*/


}

void CSYSSetDig::OnEnKillfocusEditnoutputch()
{
	// TODO: Add your control notification handler code here
	CEdit* teditcoutout=(CEdit*)GetDlgItem(IDC_EDIT_nOutputCh);
	CStatic* tstaticoutsub=(CStatic*)GetDlgItem(IDC_STATIC_OUTS);

	char tch[3];
	CString tstr; 
	int tnumber;
	teditcoutout->GetWindowText(tstr);
	tnumber=_tstoi(tstr);
	_itoa_s(theApp.m_numResponseChannels-tnumber,tch,3,10);
	tstaticoutsub->SetWindowText(tch);
}

BOOL CSYSSetDig::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	

	// ����Ű ��� ���� ��Ʈ
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		// ���⿡ ����Ű ��� �ۼ�
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

