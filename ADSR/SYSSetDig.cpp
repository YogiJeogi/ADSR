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
	// 구현 예정
	// 프로젝트를 불러왔으면 불러온 데이타에서 변수값 대입
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
	// 구조체에서 불러온다.

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

	// 입력된 값에 문제가 없는지 판단
	if ( int(pow(2.,m_nSamplingFreq+8)) >= m_nStopFreq*2 )
		if( m_nSubResp+m_nResp <= MAX_OUT )
			if ( m_nStopFreq > m_nStartFreq ) 
				if ( m_nInput <= theApp.m_numExciterChannels ) 
					if ( m_nResp <= theApp.m_numResponseChannels ) {}
					else {AfxMessageBox(_T("응답 채널설정이 잘못되었습니다.")); return;}
				else {AfxMessageBox(_T("가진 채널 설정이 잘못되었습니다.")); return;}
			else {AfxMessageBox(_T("Stop Fre는 Start Fre보다 커야합니다.")); return;}
		else {AfxMessageBox(_T("응답 채널과 부 응답 채널 수가 16채널을 넘길 수 없습니다.")); return;}
	else {AfxMessageBox(_T("Stop Fre는 샘플링 주파수의 반 이하여야 합니다.")); return;}

	// 입력된 값에 문제가 없다면 저장

	// 저장
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

	// 완료
	m_bIsInitialized	=	TRUE;

	OnOK();
}



void CSYSSetDig::HowManyHWExist()
{
	// 하드웨어 설정값을 읽어서 화면에 표시한다.
	CStatic* tstaticinput  =(CStatic*)GetDlgItem(IDC_STATIC_INC);
	CStatic* tstaticoutput =(CStatic*)GetDlgItem(IDC_STATIC_OUTC);	
	CStatic* tstaticoutsub =(CStatic*)GetDlgItem(IDC_STATIC_OUTS);
	CEdit*   teditcoutout  =(CEdit*)GetDlgItem(IDC_EDIT_nOutputCh);

	char tch[3];
	// 가진 채널
	_itoa_s(theApp.m_numExciterChannels,tch,3,10);
	tstaticinput->SetWindowText(tch);
	// 응답 채널
	_itoa_s(theApp.m_numResponseChannels,tch,3,10);
	tstaticoutput->SetWindowText(tch);
	// 섭응답 채널
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
	

	// 엔터키 기능 제거 루트
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		// 여기에 엔터키 기능 작성
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

