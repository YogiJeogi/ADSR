// WhiteDig.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "WhiteDig.h"


// CWhiteDig dialog

IMPLEMENT_DYNAMIC(CWhiteDig, CDialog)

CWhiteDig::CWhiteDig(CWnd* pParent /*=NULL*/)
	: CDialog(CWhiteDig::IDD, pParent)
// 	, m_strTime(_T("10"))
// 	, m_strAlpha(_T("1"))
// 	, m_strSTD(_T("2"))
// 	, m_nWhiteNoiseBorderFreq(0)
, m_nStartFreq(1)
, m_nBorderFreq(50)
, m_nStopFreq(100)
, m_dbSTD(1)
, m_dbAlpha(1)
, m_dbTime(20)
{
}

CWhiteDig::~CWhiteDig()
{
}

void CWhiteDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_WN_StartFre, m_nStartFreq);
	DDX_Text(pDX, IDC_EDIT_WN_BorderFre, m_nBorderFreq);
	DDX_Text(pDX, IDC_EDIT_WN_StopFre, m_nStopFreq);
	DDX_Text(pDX, IDC_EDIT_WN_STD, m_dbSTD);
	DDX_Text(pDX, IDC_EDIT_WN_Alpha, m_dbAlpha);
	DDX_Text(pDX, IDC_EDIT_WN_Time, m_dbTime);
}


BEGIN_MESSAGE_MAP(CWhiteDig, CDialog)
	ON_BN_CLICKED(IDOK, &CWhiteDig::OnBnClickedOk)
END_MESSAGE_MAP()


// CWhiteDig message handlers

BOOL CWhiteDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_nStartFreq=System.SS.SPECTRAL.m_nStartFreq;
	m_nStopFreq=System.SS.SPECTRAL.m_nStopFreq;
	m_nBorderFreq=(int) m_nStopFreq/m_nStartFreq;  // 처음 반 값으로 보더 주파수 설정

	/* =========================================================
	 *	기존에 저장된것이 있으면 불러온다.
	 * =========================================================*/	
	if ( m_bIsWhiteNoiseGenerated == TRUE )
	{

		m_dbAlpha=System.SS.SI.m_dbWhiteNoiseAlpha;
		m_dbSTD=System.SS.SI.m_dbWhiteNoiseSTD;	
		m_dbTime=System.SS.SI.m_dbWhiteNoiseTime;	
		m_nBorderFreq=System.SS.SI.m_nWhiteNoiseBorderFreq;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWhiteDig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if ( m_nBorderFreq >= System.SS.SPECTRAL.m_nStartFreq)
		if ( m_nBorderFreq <= System.SS.SPECTRAL.m_nStopFreq ) {}
		else {AfxMessageBox("Border Frequency 설정이 잘못되었습니다."); return;}
	else {AfxMessageBox("Border Frequency 설정이 잘못되었습니다."); return;}


	// 변수 저장
	System.SS.SI.SetSystemIdentificationParam(  m_dbTime,
																	m_nBorderFreq,
																	m_dbAlpha,
																	m_dbSTD,
																	System.SS.SPECTRAL.m_nBlocksize,
																	System.SS.FRONTEND.m_nSamplingFreq );

	m_bIsWhiteNoiseGenerated=TRUE;
	OnOK();
}

BOOL CWhiteDig::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		// 여기에 엔터키 기능 작성
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

bool CWhiteDig::m_bIsWhiteNoiseGenerated=FALSE;
